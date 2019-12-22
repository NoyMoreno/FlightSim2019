//
// Created by noy on ١٩‏/١٢‏/٢٠١٩.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include "Lexer.h"
using namespace std;
Lexer::Lexer(std::string file) : file_name(file){}

void Lexer::readFromFile() {
    std::ifstream readFile(file_name);
    if (readFile.is_open() == 0){
        std::cout << "opening file failed" << std::endl;
        return;
    }
    int i = 0;
    while (!readFile.eof()) {
        std::string line;
        std::getline(readFile, line);
        lexerString.push_back(line);
       // std::cout<<line << std::endl;
    }
    readFile.close();
}

std::vector<std::string> Lexer::getLexerString() {
    return lexerString;
}
