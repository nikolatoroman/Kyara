#pragma once

#include <vector>
#include <memory>

#include "token.h"
#include "ast.h"

\\ Parser header
class Parser {
public:
    Parser(const std::vector<Token>& tokens);

    std::unique_ptr<Stmt> parse();

private:
    std::vector<Token> tokens;
    size_t position;
    Token currentToken;

    void advance();

    std::unique_ptr<Expr> parseExpression();
    std::unique_ptr<Expr> parseTerm();
    std::unique_ptr<Expr> parseFactor();
    std::unique_ptr<Stmt> parseStatement();
    std::unique_ptr<Stmt> parseVarDecl();
};