//
// Created by noy on ٢١‏/١٢‏/٢٠١٩.
//

#ifndef PROJECT1_VARCOMMAND_H
#define PROJECT1_VARCOMMAND_H


#include <string>
using namespace std;

class VarCommand {
public:
    VarCommand(const std::string& all_data_var);
    void  setSim(string sim);
    void setValue(float val);
    const string getSim();
    float getValue();
    const string getVar_name();
private:
    int value;
    const string sim;
    const string var_name;


};


#endif //PROJECT1_VARCOMMAND_H
