//
// Created by noy on ٢٣‏/١٢‏/٢٠١٩.
//

#include "DataVars.h"
#include "VarCommand.h"

extern unordered_map<string, VarCommand*>  toServer;
DataVars :: DataVars() {}
int DataVars::execute(std::vector<std::string> commands, int pos) {
    // two options:1. var *name var* *->/<-* *sim... 2. var *name var* = name var else
    string var_name = commands[pos];
    string bind = commands[pos + 1];
    // option 2
    if (bind.compare("=")) {
        // assigment
        string assigmentFor = commands[pos + 2];
        toServer[var_name] = toServer[assigmentFor];
    }
    else { // option 1
        string sim = commands[pos + 2];
        dataMap[var_name] = {sim, bind};
    }
    return 3;

}
unordered_map<string, pair<string, string>> DataVars:: getDataMap(){
    return dataMap;
}