# Noy & Shaltiel's Flight Simulator

Our first checkpoint for the Flight Simulator project, for course 89-210. 

## Usage

After compiling the code, it can be run as follows:

```bash
./a.out fly.txt
```
Where ```fly.txt``` is an input file with commands on flying a plane. 
In order to fully run a plane, it is important to run the command which connects it to the simulator before running the actual simulator. 

The code allows for a variety of commands, including ```Print```, ```Sleep```, ```While```, and more. 

Any command that was demonstrated in the sample ```fly.txt``` is supported in our code.
> Function definitions are not supported yet

## How it works

The process of handling a new "language" is built up of multiple stages.

******
### Part 1: The Lexer
Given an input file, we read it in line by line, and break it up into _"lexical items"_. We return a list of commands, where each command is either a keyword for a command (aka ```var``` or function name), a variable, a parameter, etc. Then, we defined each keyword/command to know how many arguments it requires, and then whenever we parse a keyword/command, it just tells us how much further to jump forward in our list until the next command.
> Important to note: "Expressions" (e.g., x + y) and items in quotation marks (strings) we don't break, and we keep the entirely as one _"lexical item"_.

For example:
```
Print("Hello")
Sleep(100)
```
This will get broken down into 4 items:
```Print, "Hello", Sleep, 100```.

```Print``` is a function name that accepts one argument. Therefore, when we execute the Print command, it returns the value ```1``` telling us to jump forward 1 item in our list, since that was a parameter for the function. Afterwards, we follow through to Sleep and so on.

******
### Part 2: Commands
We support a closed list of keywords/commands, so we pre-define a Command class for each keyword. Each Command defines its own `execute` function, accepts all the subsequent _"lexical items"_ and processes all the relevant items. 

We store a map between every command string and the relevant command object, for an easy lookup.

We support the definition of new variables, and they themselves can be treated as keywords (for example, `x = 2` - here we have a locally defined variable `x` which is the "keyword" of the command), so our `CreateVarCommand` class will have access to our commands table, and be able to add new items to it.

*******

### Part 3: Server Commands

We have two server commands - one which is used for sending commands, and one for receiving. Whenever one of them is run, the execute function runs as a **Blocking Call**, where it will keep the main thread from continuing to execute until we establish a working connection with the simulator.


********

### Part 4: BlockCommand

This command is meant to run a block of code (encased in `{}`), and execute is as needed. This is used whenever we have a ```While``` or an `If`. The command looks for a condition, tries to execute it, and if the final answer is true, then it runs the whole block of code.

In order to not duplicate code, we treated the initial list of commands (aka, the whole file) as a single block. So, `BlockCommand` is the only code that has a list of commands which can execute a list of _"lexical items"_. Whenever we encounter a new block, we just create a new `BlockCommand` object and run it on the new block. In order to support different variables within the scopes, every time we run a `BlockCommand` it copies in all our current commands (variables included), and then create a new local map which then can get new variables, but will be destroyed at the end of the block.

> We should note, the commands are all stores via pointers, so when we update the value of certain variables in an enclosed block, they are updated in the larger block as well.

********

### Part 5: VarCommand

A `VarCommand` can only be generated via the `CreateVarCommand` command. The `CreateVarCommand` is called with the keyword `var`, and it creates a new variable which is then added to our list of commands.

We have 3 types of `VarCommand`:
 * `Update_Var` = This Variable doesn't contain any value itself, but rather has a "set" option, where when it is assigned a new value it transfers the value to the connected server, and updates it there. This is symbolized via the `->` operator during the `Create` process.
 * `Accept_Var` = This Variable cannot be assigned a local value, but rather is a link to a certain value from the server. Whenever we are requesting the value of the object, we look it up in the `map` which is updated from the server every `0.1` seconds. This is symbolized via the `<-` operator during the `Create` process.
 * `Value_Var` = This variable is a regular variable. It contains a numeric value, and allows assignment and retrieving.




