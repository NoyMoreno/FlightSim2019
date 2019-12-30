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
    while (!readFile.eof()) {
        std::string line;
        std::getline(readFile, line);
        // if start with "" or end
        bool fInQuote = false;
		bool fEquals = false;
        int prevPos = 0;
        int len = line.length();
        for (int pos = 0; pos <= len; pos++) {
			if (fEquals && pos != len) continue;
            if (fInQuote){
                if (line[pos] == '"') fInQuote = false;
                else continue;
            }
            else if (line[pos] == '"') {
                fInQuote = true;
                continue;
            }
            // Separators - space, parens, comma
            if (pos == len || line[pos] == '\t' || line[pos] == ' ' || line[pos] == '(' || line[pos] == ')' || line[pos] == ',') {
				if (prevPos != pos) {
					string s = line.substr(prevPos, pos - prevPos);
					lexerString.push_back(s);
					if (s.compare("=") == 0)
						fEquals = true;
				}
                prevPos = pos + 1;
                continue;
            }
        }
    }
    readFile.close();
}

std::vector<std::string> Lexer::getLexerString() {
    return lexerString;
}
