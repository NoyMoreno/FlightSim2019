//
// Created by noy on ١٧‏/١٢‏/٢٠١٩.
//

#ifndef EX3_VAR_H
#define EX3_VAR_H
#include <iostream>
#include <string.h>
#include "unordered_map"
using namespace std;


class Var {
    std::string identifier, scope, type;
    char* sim;
    int lineNo;
    Var* next;

    void print()
    {
        cout << "Identifier's Name:" << identifier
            // << "\nType:" << type
             << "\nScope: " << scope
             << "\nLine Number: " << lineNo << endl;
    }
    friend class SymbolTable;
};

class SymboleTable{


};

#endif //EX3_VAR_H
