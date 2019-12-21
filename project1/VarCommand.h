//
// Created by noy on ٢١‏/١٢‏/٢٠١٩.
//

#ifndef PROJECT1_VARCOMMAND_H
#define PROJECT1_VARCOMMAND_H


#include <string>

class VarCommand {
public:
    VarCommand(const std::string& all_data_var);
    void  setSim();
    void setValue();
    void getValue();
    void getVar_name();
private:
    int value;
    std::string sim;
    std:: string var_name;


};


#endif //PROJECT1_VARCOMMAND_H
