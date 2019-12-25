//
// Created by noy on ٢٣‏/١٢‏/٢٠١٩.
//

#ifndef PROJECT1_SLEEPCOMMAND_H
#define PROJECT1_SLEEPCOMMAND_H


#include "Command.h"

class SleepCommand : public Command {
public:
    SleepCommand();
    int execute(std::vector<std::string> commands, int pos);
};


#endif //PROJECT1_SLEEPCOMMAND_H
