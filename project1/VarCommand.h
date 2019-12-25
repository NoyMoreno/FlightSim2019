//
// Created by noy on ٢١‏/١٢‏/٢٠١٩.
//

#ifndef PROJECT1_VARCOMMAND_H
#define PROJECT1_VARCOMMAND_H


#include <string>
#include "Command.h"
#include "DataVars.h"

using namespace std;

class VarCommand : public Command {
public:
    VarCommand(string varName);
    int execute(std::vector<std::string> commands, int pos);
    void setValue(float val);
    float getValue();
    string getName();

private:
    DataVars data;
    float value;
    string var_name;
    string sim;
    string bind;
};


#endif //PROJECT1_VARCOMMAND_H
