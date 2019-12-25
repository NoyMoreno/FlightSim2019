//
// Created by noy on ٢٣‏/١٢‏/٢٠١٩.
//

#ifndef PROJECT1_DATAVARS_H
#define PROJECT1_DATAVARS_H
#include <vector>
#include <string>
#include <unordered_map>
#include "Command.h"

using namespace std;
class DataVars : public Command {
public:
    DataVars();
    int execute(std::vector<std::string> commands, int pos);
    static unordered_map<string, pair<string, string>> dataMap;
    static string bind;
    // unordered_map<varName, pair<sim, bind>> getSymbolTable()
    unordered_map<string, pair<string, string>> getDataMap();

};


#endif //PROJECT1_DATAVARS_H
