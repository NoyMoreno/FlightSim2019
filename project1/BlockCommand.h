
#include <vector>
#include <unordered_map>
#include <map>
#include "ex1.h"
#include <regex>
#include "Command.h"

class VarCommand;

enum BLOCK_TYPE { NONE, IF, WHILE };

using namespace std;

class BlockCommand : Command {
private:
	BLOCK_TYPE type;
	unordered_map<string, Command *> *baseCommands;
	unordered_map<string, Expression *> *baseValues;
	unordered_map<string, Command *> curCommands;
	unordered_map<string, Expression *> curValues;
public:
	BlockCommand(BLOCK_TYPE _type, unordered_map<string, Command *> *_baseCommands, unordered_map<string, Expression *> *_baseValues) : type(_type), baseCommands(_baseCommands), baseValues(_baseValues) {}
	~BlockCommand() {}
	int execute(std::vector<std::string> commands, int pos);
	void addVar(string key, VarCommand *c);
	double interpretExpression(string exp) {
		map<string, double> expValues;
        for (auto &tp : curValues)
            expValues[tp.first] = tp.second->calculate();
		Interpreter ip;
		ip.setVariables(expValues);
		return ip.interpret(regex_replace(exp, regex(" "), ""))->calculate();
	}
	
};

