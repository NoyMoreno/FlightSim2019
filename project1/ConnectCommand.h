//
// Created by noy on ٢٠‏/١٢‏/٢٠١٩.
//

#ifndef PROJECT1_CONNECTCOMMAND_H
#define PROJECT1_CONNECTCOMMAND_H


#include "Command.h"
#include <string>

class ConnectCommand : public Command {
public:
	static void update_value(string key, double val);
private:
	static int clientSocket;
public:
    ConnectCommand();
    virtual ~ConnectCommand() {}
    int execute(vector<string> commands, int ind);
};


#endif //PROJECT1_CONNECTCOMMAND_H
