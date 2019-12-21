#include <iostream>
#include "OpenServerCommand.h"
#include "ConnectCommand.h"
#include "Lexer.h"
#include "VarCommand.h"
#include <vector>
using namespace std;
#define PORT "5400"


void parser(vector<string> vector);

int main(int argc, char *argv[]) {
    /*if (argc != 2) {
        cout << "Usage: ./a.out commands_filename" << endl;
        return 0;
    }*/
    unordered_map <string, Command> allCommands;

    unordered_map <string, VarCommand> symTable;
    //map for while scope
    unordered_map <string, VarCommand> symTableWhile;
    // put each line from file in vector.
    Lexer l("../fly.txt");
    l.readFromFile();
    vector<string> strings_To_Parser = l.getLexerString();
    parser(strings_To_Parser);
    ConnectCommand op(5400);
    op.execute();


}

void parser(vector<string> strings_to_parser) {
    vector<string>:: iterator itr = strings_to_parser.begin();
    vector<string>:: iterator itrEnd = strings_to_parser.end();
    for(; itr != itrEnd; ++itr) {
       cout << *itr << endl;
       string s = *itr;
       if(s.find("openDataServer")) {

       }
       else if(s.find("connectControlClient")) {

        }
       else if(s.find("var")) {

       }
       else if(s.find("Sleep")) {

       }
       else if(s.find("Print")) {

       }
       else if(s.find("While")) {
           // here we send by reference the itr so we will increase until the end of the loop

       }
       else {
           //function to map every value to his var
       }

    }

}
