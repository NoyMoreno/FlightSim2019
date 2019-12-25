//
// Created by noy on ١٨‏/١٢‏/٢٠١٩.
//

#ifndef PROJECT1_COMMAND_H
#define PROJECT1_COMMAND_H

#include <string>
#include <vector>

class Command {
public:
    virtual int execute(std::vector<std::string> commands, int pos) = 0;
};

#endif //PROJECT1_COMMAND_H
