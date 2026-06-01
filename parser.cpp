#include "parser.h"


Parser::Parser(const std::vector<Token>& toks)
    : tokens(toks),
      position(0),
      currentToken(tokens[0]) {
}

\\Pomera parser na sledeci token iz lexer-a
void Parser::advance() {
    position++;

    if (position < tokens.size())
        currentToken = tokens[position];
}

std::unique_ptr<Expr> Parser::parseFactor() {

    if (currentToken.type == TokenType::NUMBER) {
        int value = std::stoi(currentToken.value);
        advance();
        return std::make_unique<NumberExpr>(value);
    }

    if (currentToken.type == TokenType::IDENTIFIER) {
    std::string name = currentToken.value;
    advance();
    return std::make_unique<VariableExpr>(name);
    }

    if (currentToken.type == TokenType::LPAREN) {
    advance();
    auto expr = parseExpression();
    if (currentToken.type != TokenType::RPAREN) {
        std::cerr << "Expected ')'" << std::endl;
        return nullptr;
    }

    advance();
    return expr;
}
    return nullptr;
}

\\postavlja mnozenje i deljenje kao starije operacije
std::unique_ptr<Expr> Parser::parseTerm() {

    auto left = parseFactor();

    while (
        currentToken.type == TokenType::STAR ||
        currentToken.type == TokenType::SLASH
    ) {
        char op =
            currentToken.type == TokenType::STAR
            ? '*'
            : '/';

        advance();
        auto right = parseFactor();

        left = std::make_unique<BinaryExpr>(
            op,
            std::move(left),
            std::move(right)
        );
    }

    return left;
}

\\ Parsuje sabiranje i oduzimanje,
\\I postavlja ih ispod mnozenje/deljenja u AST-u
std::unique_ptr<Expr> Parser::parseExpression() {

    auto left = parseTerm();

    while (
        currentToken.type == TokenType::PLUS ||
        currentToken.type == TokenType::MINUS
    ) {
        char op =
            currentToken.type == TokenType::PLUS
            ? '+'
            : '-';

        advance();
        auto right = parseTerm();
        left = std::make_unique<BinaryExpr>(
            op,
            std::move(left),
            std::move(right)
        );
    }

    return left;
}

std::unique_ptr<Stmt> Parser::parse() {
    return parseStatement();
}

\\Parsuje deklaraciju promenjivih
std::unique_ptr<Stmt> Parser::parseVarDecl() {

    advance();

    if (currentToken.type != TokenType::IDENTIFIER) {
        std::cerr
            << "Expected identifier after let"
            << std::endl;
        return nullptr;
    }

    std::string name = currentToken.value;
    advance();

    if (currentToken.type != TokenType::EQUAL) {
        std::cerr
            << "Expected '='"
            << std::endl;
        return nullptr;
    }

    advance();
    auto initializer = parseExpression();

    if (currentToken.type != TokenType::SEMICOLON) {
        std::cerr
            << "Expected ';'"
            << std::endl;
        return nullptr;
    }

    advance();

    return std::make_unique<VarDeclStmt>(
        name,
        std::move(initializer)
    );
}

\\parsuje statement
std::unique_ptr<Stmt> Parser::parseStatement() {

    if (currentToken.type == TokenType::LET)
        return parseVarDecl();

    return nullptr;
}