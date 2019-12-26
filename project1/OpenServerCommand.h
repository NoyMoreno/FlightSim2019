//#define WINDOWS_USE

//
// Created by noy on ١٩‏/١٢‏/٢٠١٩.
//

#ifndef PROJECT1_OPENSERVERCOMMAND_H
#define PROJECT1_OPENSERVERCOMMAND_H

#include <string>
#include <unordered_map>
#include <mutex>
#include "Command.h"

using namespace std;

class OpenServerCommand : public Command {
private:
	static unordered_map<std::string, double> allAcceptingVars;
	static std::mutex acceptVarMapLock;
public:
	static double getValue(string key) {
		acceptVarMapLock.lock();
		double ret = allAcceptingVars[key];
		acceptVarMapLock.unlock();
		return ret;
	}
    OpenServerCommand();
    int execute(vector<string> commands, int ind);
};


#endif //PROJECT1_OPENSERVERCOMMAND_H
