//
// Created by noy on ١٩‏/١٢‏/٢٠١٩.
//

#ifndef PROJECT1_LEXER_H
#define PROJECT1_LEXER_H
#define MAX_STRING_SIZE 100
#include <vector>
#include <string>

class Lexer {
private:
    std::string file_name;
    std::vector<std::string> lexerString;
public:
    Lexer(std::string file_name);
    void  readFromFile();
    std:: vector<std::string> getLexerString();
};


#endif //PROJECT1_LEXER_H
