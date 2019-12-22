#include <iostream>
#include "OpenServerCommand.h"
#include "ConnectCommand.h"
#include "Lexer.h"
#include "VarCommand.h"
#include <vector>
using namespace std;
#define PORT "5400"


void parser(vector<string> vector);
string removeSpaces(string str);
const string find_var_name(string basicString);

int main(int argc, char *argv[]) {
    /*if (argc != 2) {
        cout << "Usage: ./a.out commands_filename" << endl;
        return 0;
    }*/
    //unordered_map <string, Command> allCommands;

    unordered_map <string, VarCommand> symTable;
    //map for while scope
    unordered_map <string, VarCommand> symTableWhile;
    // put each line from file in vector.
    Lexer l("../fly.txt");
    l.readFromFile();
    vector<string> strings_To_Parser = l.getLexerString();
    parser(strings_To_Parser);
    //OpenServerCommand op(5400);
    //ConnectCommand cc(5402);
    //op.execute();
    //cc.execute();
}

void parser(vector<string> strings_to_parser) {
    vector<string>:: iterator itr = strings_to_parser.begin();
    vector<string>:: iterator itrEnd = strings_to_parser.end();
    // for while loop
    bool isWhileCommand = false;
    vector<string> while_commands_as_str;
    // for if loop
    vector<string> if_commands_as_str;
    bool isIfCommand;
    for(; itr != itrEnd; ++itr) {
        string original_str = *itr;
        // first, erase space
        string s = removeSpaces(original_str);
        unsigned int indexParameterStart = s.find('(') + 1;
        unsigned int indexParameterEnd = s.find(')');
        string str_param = s.substr(indexParameterStart, indexParameterEnd - indexParameterStart);
       if(s.find("openDataServer") != string::npos) {
               unsigned int port = atoi(str_param.c_str());
               cout << "port" << port << endl;
       }
       else if(s.find("connectControlClient") != string::npos) {
           // first, erase space
           string str_without_space = removeSpaces(s);
           unsigned int first_param = s.find(",");
           string localHost = s.substr(indexParameterStart, first_param - indexParameterStart);
           cout << "local host: " << localHost <<endl;
           string str_port = s.substr(first_param + 1, indexParameterEnd - first_param -1);
           unsigned int port = atoi(str_port.c_str());
           cout<< "port: " << port << endl;
       }
       else if(s.find("var") != string::npos) {
           unsigned int start_var_name = s.find("var");
           // now find the full var name
           string str = s.substr(start_var_name + 3);
           const string var_name = find_var_name(str);
           cout << "var name: " << var_name <<endl;
           // now check it's assignment
           if (var_name.find("=") != string::npos) {
               unsigned int i = var_name.find('=');
               const string cur_var_name = var_name.substr(0, i);
               cout << "cur var name: " << cur_var_name <<endl;
               const string assignmentFor = var_name.substr(i + 1);
               cout << "assignment for: " << assignmentFor << endl;
               //now make the assignment
           }
       }
       else if(s.find("Sleep") != string::npos) {
           unsigned int sleep_time = atoi(str_param.c_str());
           cout<< "time sleep: " << sleep_time <<endl;
       }
       else if(s.find("Print") != string::npos) {
           // print command for strings
           if (s.find('"') != string::npos) {
               // without '"' chars', here we need with space
               indexParameterStart = original_str.find_first_of('"') + 1;
               indexParameterEnd = original_str.find_last_of('"');
               string str_to_print = original_str.substr(indexParameterStart, indexParameterEnd - indexParameterStart);
               cout << "str to print: " << str_to_print << endl;
           }
           // print command ,for some variable
           else {

           }
       }
       else if(s.find("While")) {
           isWhileCommand = true;
           while_commands_as_str.push_back(s);
       }
       else if (s.find("}")) {
           if (isWhileCommand) {
               while_commands_as_str.push_back(s);
               isWhileCommand = false;
           }
           if (isIfCommand) {
               if_commands_as_str.push_back(s);
               isIfCommand = false;
           }
       }
       else if (isWhileCommand) {
           while_commands_as_str.push_back(s);
       }
       else if(s.find("if")) {
           isIfCommand = true;
           if_commands_as_str.push_back(s);
       }
       else {
           //function to map every value to his var
       }

    }

}
// function to get variable name
const string find_var_name(string str) {
    int i = 0;
    string finalString;
    while (i < str.length()) {
        if(str[i] == '-' || str [i] == '<') {
            return finalString;
        }
        // add this char to variable
        finalString += str.substr(i, 1);
        i++;
    }
    return  finalString;
}

// Function to remove all spaces from a given string
string removeSpaces(string str)
{
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    return str;
}
