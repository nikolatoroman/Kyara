#include "lexer.h"
#include <cctype>

Lexer::Lexer(const std::string& src)
    : source(src), position(0) {

    currentChar = position < source.size()
        ? source[position]
        : '\0';
}

//Pomera se da cita sledeci karakter
void Lexer::advance() {
    position++;

    if (position < source.size())
        currentChar = source[position];
    else
        currentChar = '\0';
}

//Proverava dali postoji sledeci karakter
char Lexer::peek() {
    if (position + 1 < source.size())
        return source[position + 1];

    return '\0';
}

//Preskace razmake
void Lexer::skipWhitespace() {
    while (std::isspace(currentChar))
        advance();
}

//Citanje brojeva
Token Lexer::number() {
    std::string result;

    while (std::isdigit(currentChar)) {
        result += currentChar;
        advance();
    }

    return Token(TokenType::NUMBER, result);
}

//Citanje kljucnih reci
Token Lexer::identifier() {
    std::string result;

    while (std::isalnum(currentChar) || currentChar == '_') {
        result += currentChar;
        advance();
    }

    if (result == "let")
        return Token(TokenType::LET);

    if (result == "fn")
        return Token(TokenType::FN);

    if (result == "return")
        return Token(TokenType::RETURN);

    if (result == "if")
        return Token(TokenType::IF);

    if (result == "else")
        return Token(TokenType::ELSE);

    if (result == "while")
        return Token(TokenType::WHILE);

    return Token(TokenType::IDENTIFIER, result);
}


std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (currentChar != '\0') {

        if (std::isspace(currentChar)) {
            skipWhitespace();
            continue;
        }

        if (std::isdigit(currentChar)) {
            tokens.push_back(number());
            continue;
        }

        if (std::isalpha(currentChar) || currentChar == '_') {
            tokens.push_back(identifier());
            continue;
        }

        //Citanje tokeni koji su jedan karakter
        //Kao sto su operatori i zagrade
        switch (currentChar) {

            case '+':
                tokens.push_back(Token(TokenType::PLUS));
                break;

            case '-':
                tokens.push_back(Token(TokenType::MINUS));
                break;

            case '*':
                tokens.push_back(Token(TokenType::STAR));
                break;

            case '/':
                tokens.push_back(Token(TokenType::SLASH));
                break;

            case '=':
                tokens.push_back(Token(TokenType::EQUAL));
                break;

            case '(':
                tokens.push_back(Token(TokenType::LPAREN));
                break;

            case ')':
                tokens.push_back(Token(TokenType::RPAREN));
                break;

            case '{':
                tokens.push_back(Token(TokenType::LBRACE));
                break;

            case '}':
                tokens.push_back(Token(TokenType::RBRACE));
                break;

            case ',':
                tokens.push_back(Token(TokenType::COMMA));
                break;

            case ';':
                tokens.push_back(Token(TokenType::SEMICOLON));
                break;


            default:
                tokens.push_back(Token(
                    TokenType::INVALID,
                    std::string(1, currentChar)
                ));
                break;
        }

        advance();
    }

    tokens.push_back(Token(TokenType::EOF_TOKEN));

    return tokens;
}
