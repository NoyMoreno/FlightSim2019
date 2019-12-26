//
// Created by noy on
//
#include "BlockCommand.h"
#include "CreateVarCommand.h"
#include "PrintCommand.h"

int BlockCommand::execute(std::vector<std::string> commands, int pos) {
    curCommands = *baseCommands;
    curValues = *baseValues;

    curCommands["if"] = new BlockCommand(IF, &curCommands, &curValues);
    curCommands["while"] = new BlockCommand(WHILE, &curCommands, &curValues);
    curCommands["var"] = new CreateVarCommand(this);
    curCommands["Print"] = new PrintCommand(this);

    unsigned int end_ind = commands.size();

    if (type == IF || type == WHILE) {
        // Find the end
        int totalOpen = 0;
        for (end_ind = pos; end_ind < commands.size(); end_ind++) {
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
            string sop;
            while (commands[start_ind].compare("{") != 0) {
                if (operators.find(commands[start_ind]) != operators.end()) {
                    op = operators[commands[start_ind]];
                    sop = commands[start_ind];
                }
                else {
                    if (op == NULL) l += commands[start_ind];
                    else r += commands[start_ind];
                }
                start_ind++;
            }
            condition_matched = op(interpretExpression(l), interpretExpression(r));
            start_ind++;
        }

        if (condition_matched) {
            for (unsigned int ind = start_ind; ind < end_ind; ind++) {
                // from here noy add
                string s = commands[ind];
                Command *c = curCommands[s];
                ind += c->execute(commands, ind + 1);
            }
        }

        if (!condition_matched || type != WHILE)
            break;
    }
    return end_ind - pos + 1;
}
void BlockCommand::addVar(string key, VarCommand *c) {
    curCommands[key] = c;
    if (c->has_value())
        curValues[key] = c;
}