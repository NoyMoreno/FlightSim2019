//
// Created by noy on ٢٣‏/١٢‏/٢٠١٩.
//

#ifndef PROJECT1_PRINTCOMMAND_H
#define PROJECT1_PRINTCOMMAND_H


#include <iostream>
#include "Command.h"

class BlockCommand;

using namespace std;
class PrintCommand : public Command {
private:
    BlockCommand *block;
public:
    PrintCommand(BlockCommand *_block);
    int execute(vector<string> commands, int ind);
};


#endif //PROJECT1_PRINTCOMMAND_H
