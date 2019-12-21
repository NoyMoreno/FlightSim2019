//
// Created by noy on ١٩‏/١٢‏/٢٠١٩.
//

#ifndef PROJECT1_OPENSERVERCOMMAND_H
#define PROJECT1_OPENSERVERCOMMAND_H

#include <string>
#include <bits/stdc++.h>
using namespace std;

class OpenServerCommand {
private:
    unsigned int m_port;
    unordered_map <std::string, double> allAcceptingVars;
public:
    OpenServerCommand(unsigned int port);
    int execute();

};


#endif //PROJECT1_OPENSERVERCOMMAND_H
