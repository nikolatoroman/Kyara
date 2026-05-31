#pragma once

#include <string>
#include <vector>
#include "token.h"

//Definisemo klasu lexera
class Lexer {
public:
    Lexer(const std::string& source);

    std::vector<Token> tokenize();


private:
    std::string source;
    size_t position;
    char currentChar;

    //Funkcije koje koristi nas lexer
    void advance();
    void skipWhitespace();

    Token number();
    Token identifier();

    char peek();
};
