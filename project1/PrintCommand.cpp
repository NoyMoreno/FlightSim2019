//
// Created by noy on ٢٣‏/١٢‏/٢٠١٩.
//

#include "PrintCommand.h"
PrintCommand::PrintCommand() {}
int PrintCommand:: execute(vector<string> commands, int ind) {
    string s  = commands[ind];
    if (s[0] == '"')
        s = s.substr(1, s.length() - 2);
    // takes one argument, and prints it
    cout << s << endl;
    return 1;
}