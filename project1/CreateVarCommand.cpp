//
// Created by noy on 
//
#include "CreateVarCommand.h"
#include "BlockCommand.h"

int CreateVarCommand::execute(std::vector<std::string> commands, int pos) {
	// First is the name
	string name = commands[pos];
	// Next, is the type
	VAR_TYPE type = VALUE;
	// What type?
	if (commands[pos + 1].compare("->") == 0)
		type = UPDATE;
	if (commands[pos + 1].compare("<-") == 0)
		type = ACCEPT;

	// Create the command, and set the info
	VarCommand *c = new VarCommand(name, type, block);
	block->addVar(name, c);
	if (type == VALUE) {
		c->setValue(block->interpretExpression(commands[pos + 2]));
		return 3;
	}
	else {
		if (commands[pos + 2].compare("sim") != 0)
			throw "Must have sim";
		c->setSim(commands[pos + 3].substr(1, commands[pos + 3].length() - 2));
		return 4;
	}
}
