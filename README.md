# Noy (& Shaltiel's) Flight Simulator

### Goals to reach:

- Reading from socket and storing in map

- Changing active variable in flight

- Lexer - breaking up into commands

- Handling all commands

- Handling loops


### Done so far:

- Basic outline for Command class

- Initial outline for command to read from socket

### To do:

- Make sure reading from socket works (Dec 19th):

  * Run sample version of code, choose one variable and print out every time it changes
  * Connect via telnet to the simulator, change the variable
  * Make sure the change is acknowledged
  * Think how this would work in a background thread - don't implement yet, but think about how this would work in the background while the rest of the code executes.
  
- Make sure writing to socket works (Dec 21st, midnight):

  * Run simulator, and connect via telnet in temrinal. 
  * Get a variable, see it's value (e.g., "GET /instrumentation/altimeter/indicated-altitude-ft")
  * Run code, set the value to a different value
  * Make sure it's acknowledged (run the GET again)
  
- Create a Lexer 

  * New class
  * Function to read from file
  * Given a line, break up into all the "commands" - Function calls, Keywords, and Parameters
  ** Start thinking about loops, but don't spend time on it 
  * Return the list of commands
  
- Start implementing the actual commands

  * One at a time, start with the way they are ordered in the fly.txt file
  * Handle "vars" - you can have three different types:
  
    * 1] accept_sim - represented with the "<-" operator: has a "key" member (e.g., "/../.../throttle"), function "get_value()" which looks it up in the map stored in "reading from socket" section. 
      - Think about locks
    * 2] update_sim - represented with the "->" operator: has a "key" member, and a function "set_value()"
      - What do you think this function does? Which of the steps until now helps with that? Write up an explanation and send me.
    * 3] value_sim - represented with the "=" operator, just stores a float.
      - How would you merge this with the above classes using inheritance? 3 lists? Figure out how this works with the symbol table.
      
  * "execute()" - figure out the parameters, the indicies, how it works
  * Functions - e.g., Print, Sleep - figure out how to make them more generic.
  * Assignment - when encountering the "=" sign not preceed by "var" - what are the options? what should you do with each of the 3 "sims"?
  * Conditions - slightly more complicated, start giving it more thought. For now, don't try parsing them.
     - Loops, again, this for the future - just think about it
 
#### Make sure it all works. Run it without conditions and loops, and make sure it all works.
 
 - Conditions and Loops:
   * Items to think about: 
    -  Scopes - how would you figure out where the scope begins/ends?
      * Pay attention to the possiblity for nested scopes (two if's inside oen another)
      * If the condition isn't met, you need to skip to the end.
    - With loops - you need to be able to jump to the top of the loop at any point (continue), or exit (break)
    - Conditional properties:
      * All the different comparer operators - "<", "<=", etc.
      * When using Vars - some way to be able to tell when you are accessing a value_sim, or an accept_sim, (can you have an accept_sim in a condition? Explain why.), or an actual value?
   
   


