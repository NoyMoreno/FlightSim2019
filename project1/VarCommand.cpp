//#define WINDOWS_USE
//
// Created by noy on 
//

#include <cstring>
#include <iostream>
#ifndef WINDOWS_USE
#include <sys/socket.h>
#endif
#include "VarCommand.h"
#include "OpenServerCommand.h"
#include "ConnectCommand.h"
#include "BlockCommand.h"
// <sim, var>
int VarCommand::execute(std::vector<std::string> commands, int pos) {
    if (var_type == UPDATE){
		// Commands 0 is equals
		if (commands[pos].compare("="))
			throw "Must have equals";
		ConnectCommand::update_value(sim, block->interpretExpression(commands[pos + 1]));
    }
    if (var_type == ACCEPT){
		throw "Can't run execute on accept var";
    }
	if (var_type == VALUE) {
		double newVal = stod(commands[pos + 1]);
		value = newVal;
	}
	return 2;
}
