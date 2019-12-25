#include <iostream>
#include "OpenServerCommand.h"
#include "ConnectCommand.h"
#include "Lexer.h"
#include "VarCommand.h"
#include "PrintCommand.h"
#include "CreateVarCommand.h"
#include "SleepCommand.h"
#include "BlockCommand.h"

#include <vector>
using namespace std;
// map between sim to his var obj
unordered_map <string, VarCommand*> fromServer;
// map between var name to his var obj
unordered_map <string, VarCommand*> toServer;
int clientSocket;
int serverSocket;
int main(int argc, char *argv[]) {
    /*if (argc != 2) {
        cout << "Usage: ./a.out commands_filename" << endl;
        return 0;
    }*/
    unordered_map <string, Command *> allCommands;
    // define commands
    allCommands["Print"] = new PrintCommand();
    allCommands["openDataServer"] = new OpenServerCommand();
    allCommands["connectControlClient"] = new ConnectCommand();
	allCommands["Sleep"] = new SleepCommand();

	unordered_map<string, Expression *> allValues;

	Lexer l("../fly.txt");
	l.readFromFile();
	// parser
	vector<string> strings_To_Parser = l.getLexerString();

	BlockCommand command(NONE, &allCommands, &allValues);
	command.execute(strings_To_Parser, 0);
    return 0;
}
