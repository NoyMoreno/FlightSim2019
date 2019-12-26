//
// Created by noy on ٢٣‏/١٢‏/٢٠١٩.
//

#include "PrintCommand.h"
#include "BlockCommand.h"

PrintCommand::PrintCommand(BlockCommand *_block) : block(_block) {}
int PrintCommand:: execute(vector<string> commands, int ind) {
    string s  = commands[ind];
    if (s[0] == '"')
        s = s.substr(1, s.length() - 2);
    else s = to_string(block->interpretExpression(s));
    // takes one argument, and prints it
    cout << s << endl;
    return 1;
}