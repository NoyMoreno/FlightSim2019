
#include <vector>
#include <unordered_map>
#include <map>
#include "ex1.h"
#include <regex>
#include "Command.h"
#include "CreateVarCommand.h"


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
	int execute(std::vector<std::string> commands, int pos) {
		curCommands = *baseCommands;
		curValues = *baseValues;

		curCommands["if"] = new BlockCommand(IF, &curCommands, &curValues);
		curCommands["while"] = new BlockCommand(WHILE, &curCommands, &curValues);
		curCommands["var"] = new CreateVarCommand(this);

		unsigned int end_ind = commands.size();

		if (type == IF || type == WHILE) {
			// Find the end
			int totalOpen = 0;
			for (end_ind = pos; end_ind < end_ind; end_ind++) {
				if (commands[end_ind].compare("{") == 0)
					totalOpen++;
				if (commands[end_ind].compare("}") == 0) {
					totalOpen--;
					if (!totalOpen) break;
				}
			}// find end brackets
		}

		map<string, std::function<bool(double, double)>> operators;
		operators["=="] = [](double a, double b) { return a == b; };
		operators["!="] = [](double a, double b) { return a != b; };
		operators["<="] = [](double a, double b) { return a <= b; };
		operators[">="] = [](double a, double b) { return a >= b; };
		operators["<"] = [](double a, double b) { return a < b; };
		operators[">"] = [](double a, double b) { return a > b; };


		// when we get to if/while, look for condition
		// for now, run on everything
		while (1) {
			bool condition_matched = true;
			// First, check the condition applies
			unsigned int start_ind = pos;
			if (type != NONE) {
				// split into two parts
				string l = "", r = "";
				std::function<bool(double, double)> op;
				while (commands[start_ind].compare("{") != 0) {
					if (operators.find(commands[start_ind]) != operators.end())
						op = operators[commands[start_ind]];
					else {
						if (op == NULL) l += commands[start_ind];
						else r += commands[start_ind];
					}
				}
				condition_matched = op(interpretExpression(l), interpretExpression(r));
				start_ind++;
			}
			
			if (condition_matched) {
				for (unsigned int ind = start_ind; ind < end_ind;) {
					// from here noy add
					string s = commands[ind];
					Command *c = curCommands[s];
					ind += c->execute(commands, ind + 1);
				}
			}

			if (!condition_matched || type != WHILE)
				break;
		}
		return end_ind - pos;
	}
	void addVar(string key, VarCommand *c) {
		curCommands[key] = c;
		if (c->has_value())
			curValues[key] = c;
	}
	double interpretExpression(string exp) {
		map<string, double> expValues;
		for (auto tp : curValues)
			expValues[tp.first] = tp.second->calculate();

		Interpreter ip;
		ip.setVariables(expValues);
		return ip.interpret(regex_replace(exp, regex(" "), ""))->calculate();
	}
	
};

