#pragma once

#include <vector>
#include <memory>

#include "token.h"
#include "ast.h"

//Parser header
class Parser {
public:
    Parser(const std::vector<Token>& tokens);

    Program parse();

private:
    std::vector<Token> tokens;
    size_t position;
    Token currentToken;

    void advance();

    std::unique_ptr<Expr> parseComparison();
    std::unique_ptr<Expr> parseAddition();
    std::unique_ptr<Stmt> parseWhile();
    std::unique_ptr<Stmt> parseIf();
    std::vector<std::unique_ptr<Expr>> parseArguments();
    std::vector<std::string> parseParameters();
    std::unique_ptr<Stmt> parseFunctionDecl();
    std::vector<std::unique_ptr<Stmt>> parseBlock();
    std::unique_ptr<Stmt> parseReturn();
    std::unique_ptr<Stmt> parseAssignment();
    std::unique_ptr<Expr> parseExpression();
    std::unique_ptr<Expr> parseTerm();
    std::unique_ptr<Expr> parseFactor();
    std::unique_ptr<Stmt> parseStatement();
    std::unique_ptr<Stmt> parseVarDecl();
};
