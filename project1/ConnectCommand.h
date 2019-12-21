//
// Created by noy on ٢٠‏/١٢‏/٢٠١٩.
//

#ifndef PROJECT1_CONNECTCOMMAND_H
#define PROJECT1_CONNECTCOMMAND_H


#include "Command.h"
#include <string>

class ConnectCommand : Command {
public:
    ConnectCommand(unsigned int port);
    int execute();
private:
    unsigned int m_port;
};


#endif //PROJECT1_CONNECTCOMMAND_H
