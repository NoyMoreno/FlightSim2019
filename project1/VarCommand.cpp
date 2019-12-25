//
// Created by noy on ٢١‏/١٢‏/٢٠١٩.
//

#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include "VarCommand.h"
// <sim, var>
extern unordered_map<string, VarCommand*> toServer;
extern int clientSocket;
extern int serverSocket;
VarCommand::VarCommand(string varName) {
    this->var_name = var_name;
    // unordered_map<varName, pair<sim, bind>> getSymbolTable()
    unordered_map<string, pair<string,string>> dataMap = data.getDataMap();
    this->sim = dataMap[varName].first;
    this->bind = dataMap[varName].second;
    // update value - could be a number, or anther var and we need to get his value or expression
    //***********************************************************************do it
    // noy, update the toServer map
    toServer[varName] = this;
    //end noy
}
int VarCommand::execute(std::vector<std::string> commands, int pos) {
    if (bind.compare("->") == 0){
        // convert value to string
        char valueStr[30];
        gcvt(value, 6, valueStr);
       string stringToSet  = "set" + sim + valueStr + "\r\n";
        int is_sent;
        is_sent = send(clientSocket, stringToSet, strlen(stringToSet.c_str()), 0);
        if (is_sent == -1) {
            std::cout<<"Error sending message"<<std::endl;
        } else {
            std::cout<<"Hello message sent to server" <<std::endl;
        }
    }
    if (bind.compare("<-") == 0){
        // function run server
    }

}
void VarCommand::setValue(float val) {
    this->value = val;
}

float VarCommand::getValue() {
    return this->value;
}

string VarCommand::getName() {
    return this->var_name;
}
