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

enum VAR_TYPE_e { ACCEPT, UPDATE, VALUE } typedef VAR_TYPE;

using namespace std;

class VarCommand : public Command, public Expression {
private:
	BlockCommand *block;
	double value;
	string var_name;
	string sim;
	VAR_TYPE var_type;
public:
	VarCommand(string varName, VAR_TYPE _type, BlockCommand *_block) : block(_block), var_name(varName), var_type(_type) { };
    int execute(std::vector<std::string> commands, int pos);
	void setValue(double d) {
		value = d;
	};
	void setSim(string _sim) {
		sim = _sim;
	};
	double calculate() {
	    // Figure out where we are getting the value from
		if (var_type == VALUE)
			return value;
		if (var_type == ACCEPT)
			return OpenServerCommand::getValue(sim);
		if (var_type == UPDATE)
			throw "Cant get value for update var";
		return 0;
	};
	bool has_value() {
		return var_type != UPDATE;
	};
};


#endif //PROJECT1_VARCOMMAND_H
