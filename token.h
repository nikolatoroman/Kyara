#pragma once

#include <string>

//Ovde se definisu kljucne reci i operatori jezika

enum class TokenType {
    // Specijalni karakteri
    EOF_TOKEN,
    INVALID,

    // Literali
    IDENTIFIER,
    NUMBER,

    // Kljucne Reci
    LET,
    FN,
    RETURN,
    IF,
    ELSE,
    WHILE,

    // Operatori
    PLUS,
    MINUS,
    STAR,
    SLASH,
    EQUAL,

    // Simboli
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    COMMA,
    SEMICOLON
};

//Definisemo strukturu tokena
struct Token {
    TokenType type;
    std::string value;

    Token(TokenType t, const std::string& v = "")
        : type(t), value(v) {}
};