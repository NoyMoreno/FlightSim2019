#include <iostream>
#include "OpenServerCommand.h"
#include "ConnectCommand.h"
#include "Lexer.h"
#include "VarCommand.h"
#include "PrintCommand.h"
#include "SleepCommand.h"
#include "DataVars.h"
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
    allCommands["openServerCommand"] = new OpenServerCommand();
    allCommands["connectCommand"] = new ConnectCommand();
    allCommands["var"] = new DataVars();
    allCommands["Sleep"] = new SleepCommand();


    Lexer l("../fly2.txt");
    l.readFromFile();
    // parser
    vector<string> strings_To_Parser = l.getLexerString();
    for (int ind = 0; ind < strings_To_Parser.size(); ind++){
        // from here noy add
        string s = strings_To_Parser[ind];
        if ((s.compare("Print") != 0) && (s.compare("openDataServer") != 0) &&
                (s.compare("Sleep") != 0) &&
                        (s.compare("var") != 0) && (s.compare("connectCommand") != 0)){
            allCommands[s] = new VarCommand(s);
        }
        Command *c = allCommands[strings_To_Parser[ind]];
        ind += c->execute(strings_To_Parser, ind + 1);
    }
    return 0;
}
