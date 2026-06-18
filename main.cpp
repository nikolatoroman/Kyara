#include <iostream>
#include "lexer.h"
#include "parser.h"


std::string tokenTypeToString(TokenType type) {
    switch(type) {
        case TokenType::LET: return "LET";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::PLUS: return "PLUS";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::EOF_TOKEN: return "EOF";
        case TokenType::WHILE: return "WHILE";
        case TokenType::ELSE: return "ELSE";
        case TokenType::IF: return "IF";
        case TokenType::RETURN: return "RETURN";
        case TokenType::FN: return "FN";
        case TokenType::STAR: return "STAR";
        case TokenType::SLASH: return "SLASH";
        case TokenType::LPAREN: return "LPAREN";
        case TokenType::RPAREN: return "RPAREN";
        case TokenType::LBRACE: return "LBRACE";
        case TokenType::RBRACE: return "RBRACE";
        case TokenType::COMMA: return "COMMA";
        case TokenType::MINUS: return "MINUS";
        case TokenType::EQUAL_EQUAL: return "EQUAL_EQUAL";
        case TokenType::BANG_EQUAL: return "BANG_EQUAL";
        case TokenType::LESS: return "LESS";
        case TokenType::LESS_EQUAL: return "LESS_EQUAL";
        case TokenType::GREATER: return "GREATER";
        case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
        default: return "OTHER";
    }
}

//Ovde testiramo lexer da li radi

int main() {

    /*Deo za testiranje lexera
    //std::string source = "5 + 3 * 2";

   

    Lexer lexer(source);

    auto tokens = lexer.tokenize();

    //Stampamo tokene, tj njihovu vrstu kako ih jezik vidi
    for (const auto& token : tokens) {

        std::cout
            << tokenTypeToString(token.type);

        if (!token.value.empty())
            std::cout << "(" << token.value << ")";

        std::cout << std::endl;
    }*/

    //Deo za testiranje parsera
  std::string source =
    "fn main() {"
    "let x = 5;"
    "if x >= 3 {"
    "return 1;"
    "} else {"
    "return 0;"
    "}"
    "}";
    
    Lexer lexer(source);

    auto tokens = lexer.tokenize();

    Parser parser(tokens);

    Program program = parser.parse();

    program.print();
    return 0;
}
