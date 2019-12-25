//
// Created by noy on ١٩‏/١٢‏/٢٠١٩.
//

#ifndef PROJECT1_OPENSERVERCOMMAND_H
#define PROJECT1_OPENSERVERCOMMAND_H

#include <string>
#include <bits/stdc++.h>
#include "Command.h"

using namespace std;

class OpenServerCommand : public Command {
private:
    unordered_map <std::string, double> allAcceptingVars;
    std::mutex acceptVarMapLock;
public:
    OpenServerCommand();
    int execute(vector<string> commands, int ind);

};


#endif //PROJECT1_OPENSERVERCOMMAND_H
