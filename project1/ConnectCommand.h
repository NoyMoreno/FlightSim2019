//
// Created by noy on ٢٠‏/١٢‏/٢٠١٩.
//

#ifndef PROJECT1_CONNECTCOMMAND_H
#define PROJECT1_CONNECTCOMMAND_H


#include "Command.h"
#include <string>

class ConnectCommand : public Command {
public:
	static void update_value(string key, double val) {
#ifndef WINDOWS_USE
		string stringToSet = "set" + sim + val + "\r\n";
		int is_sent;
		is_sent = send(clientSocket, stringToSet, stringToSet.length(), 0);
		if (is_sent == -1) {
			std::cout << "Error sending message" << std::endl;
		}
		else {
			std::cout << "Message sent to server" << std::endl;
		}
#endif
	}
private:
	static int clientSocket;
public:
    ConnectCommand();
    virtual ~ConnectCommand() {}
    int execute(vector<string> commands, int ind);
};


#endif //PROJECT1_CONNECTCOMMAND_H
