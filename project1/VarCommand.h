//
// Created by noy on ٢١‏/١٢‏/٢٠١٩.
//

#ifndef PROJECT1_VARCOMMAND_H
#define PROJECT1_VARCOMMAND_H

#include <string>
#include "Command.h"
#include "Expression.h"
#include "OpenServerCommand.h"
class BlockCommand;

enum VAR_TYPE { ACCEPT, UPDATE, VALUE };

using namespace std;

class VarCommand : public Command, public Expression {
private:
	BlockCommand *block;
	double value;
	string var_name;
	string sim;
	VAR_TYPE var_type;
public:
	VarCommand(string varName, VAR_TYPE _type, BlockCommand *_block) : var_name(varName), var_type(_type), block(_block) { };
    int execute(std::vector<std::string> commands, int pos);
	void setValue(double d) {
		value = d;
	}
	void setSim(string _sim) {
		sim = _sim;
	};
	double calculate() {
		if (var_type == VALUE)
			return value;
		if (var_type == ACCEPT)
			return OpenServerCommand::getValue(sim);
		if (var_type == UPDATE)
			throw "Cant get value for update var";
	}
	bool has_value() {
		return var_type != UPDATE;
	}
};


#endif //PROJECT1_VARCOMMAND_H
