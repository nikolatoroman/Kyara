#include "parser.h"


Parser::Parser(const std::vector<Token>& toks)
    : tokens(toks),
      position(0),
      currentToken(tokens[0]) {
}

//Pomera parser na sledeci token iz lexer-a
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

    if (currentToken.type == TokenType::LPAREN) {

        advance();

        auto arguments = parseArguments();

        if (currentToken.type != TokenType::RPAREN) {
            std::cerr << "Expected ')' after function arguments" << std::endl;
            return nullptr;
        }

        advance();

        return std::make_unique<CallExpr>(
            name,
            std::move(arguments)
        );
    }

    return std::make_unique<VariableExpr>(name);
}
    return nullptr;
}

//postavlja mnozenje i deljenje kao starije operacije
std::unique_ptr<Expr> Parser::parseTerm() {

    auto left = parseFactor();

    while (
        currentToken.type == TokenType::STAR ||
        currentToken.type == TokenType::SLASH
    ) {
        std::string op =
            currentToken.type == TokenType::STAR
            ? "*"
            : "/";

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

std::unique_ptr<Expr> Parser::parseExpression() {
    return parseComparison();
}


//Parsuje sabiranje i oduzimanje
std::unique_ptr<Expr> Parser::parseAddition() {

    auto left = parseTerm();

    while (
        currentToken.type == TokenType::PLUS ||
        currentToken.type == TokenType::MINUS
    ) {
        std::string op =
            currentToken.type == TokenType::PLUS
            ? "+"
            : "-";

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

//parsuje operatore logike i poredjenja
std::unique_ptr<Expr> Parser::parseComparison() {

    auto left = parseAddition();

    while (
        currentToken.type == TokenType::EQUAL_EQUAL ||
        currentToken.type == TokenType::BANG_EQUAL ||
        currentToken.type == TokenType::LESS ||
        currentToken.type == TokenType::LESS_EQUAL ||
        currentToken.type == TokenType::GREATER ||
        currentToken.type == TokenType::GREATER_EQUAL
    ) {
        std::string op;

        switch (currentToken.type) {
            case TokenType::EQUAL_EQUAL: op = "=="; break;
            case TokenType::BANG_EQUAL: op = "!="; break;
            case TokenType::LESS: op = "<"; break;
            case TokenType::LESS_EQUAL: op = "<="; break;
            case TokenType::GREATER: op = ">"; break;
            case TokenType::GREATER_EQUAL: op = ">="; break;
            default: op = "?"; break;
        }

        advance();

        auto right = parseAddition();

        left = std::make_unique<BinaryExpr>(
            op,
            std::move(left),
            std::move(right)
        );
    }

    return left;
}

// Kompajluje vise programskih izraza odjednom
Program Parser::parse() {

    Program program;

    while (currentToken.type != TokenType::EOF_TOKEN) {

        auto stmt = parseStatement();

        if (stmt) {
            program.statements.push_back(std::move(stmt));
        }
        else {
            break;
        }
    }

    return program;
}



//Parsuje deklaraciju promenjivih
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

//parsuje statement
std::unique_ptr<Stmt> Parser::parseStatement() {

    if (currentToken.type == TokenType::FN)
        return parseFunctionDecl();

    if (currentToken.type == TokenType::LET)
        return parseVarDecl();

    if (currentToken.type == TokenType::RETURN)
        return parseReturn();

    if (currentToken.type == TokenType::IDENTIFIER)
        return parseAssignment();

    if (currentToken.type == TokenType::IF)
    return parseIf();

    if (currentToken.type == TokenType::WHILE)
    return parseWhile();

    return nullptr;
}


//Omogucava dodelu vrednosti, npr x = y+1
std::unique_ptr<Stmt> Parser::parseAssignment() {

    std::string name = currentToken.value;

    advance();

    if (currentToken.type != TokenType::EQUAL) {

        std::cerr << "Expected '='" << std::endl;
        return nullptr;
    }

    advance();

    auto value = parseExpression();

    if (currentToken.type != TokenType::SEMICOLON) {

        std::cerr << "Expected ';'" << std::endl;
        return nullptr;
    }

    advance();

    return std::make_unique<AssignStmt>(
        name,
        std::move(value)
    );
}

//Parsuje Return statement
std::unique_ptr<Stmt> Parser::parseReturn() {

    advance(); // preskace 'return'

    auto value = parseExpression();

    if (currentToken.type != TokenType::SEMICOLON) {
        std::cerr << "Expected ';' after return value" << std::endl;
        return nullptr;
    }

    advance();

    return std::make_unique<ReturnStmt>(
        std::move(value)
    );
}


//Parsuje blok koda funkcije
std::vector<std::unique_ptr<Stmt>> Parser::parseBlock() {

    std::vector<std::unique_ptr<Stmt>> statements;

    if (currentToken.type != TokenType::LBRACE) {
        std::cerr << "Expected '{'" << std::endl;
        return statements;
    }

    advance();

    while (
        currentToken.type != TokenType::RBRACE &&
        currentToken.type != TokenType::EOF_TOKEN
    ) {
        auto stmt = parseStatement();

        if (stmt)
            statements.push_back(std::move(stmt));
        else
            break;
    }

    if (currentToken.type != TokenType::RBRACE) {
        std::cerr << "Expected '}'" << std::endl;
        return statements;
    }

    advance();

    return statements;
}


//Parsuje header funkcije
std::unique_ptr<Stmt> Parser::parseFunctionDecl() {

    advance(); // skip fn

    if (currentToken.type != TokenType::IDENTIFIER) {
        std::cerr << "Expected function name" << std::endl;
        return nullptr;
    }

    std::string name = currentToken.value;

    advance();

    if (currentToken.type != TokenType::LPAREN) {
        std::cerr << "Expected '('" << std::endl;
        return nullptr;
    }

    advance();

    auto parameters = parseParameters();

    if (currentToken.type != TokenType::RPAREN) {
        std::cerr << "Expected ')'" << std::endl;
        return nullptr;
    }

    advance();

    auto body = parseBlock();

    return std::make_unique<FunctionDecl>(
        name,
        std::move(parameters),
        std::move(body)
    );
}

//Parsuje parametre funkcije
std::vector<std::string> Parser::parseParameters() {

    std::vector<std::string> parameters;

    if (currentToken.type == TokenType::RPAREN)
        return parameters;

    while (true) {

        if (currentToken.type != TokenType::IDENTIFIER) {
            std::cerr << "Expected parameter name" << std::endl;
            return parameters;
        }

        parameters.push_back(currentToken.value);

        advance();

        if (currentToken.type == TokenType::COMMA) {
            advance();
            continue;
        }

        break;
    }

    return parameters;
}


//Implementacija klase koja omogucava assigment funkcija 
std::vector<std::unique_ptr<Expr>> Parser::parseArguments() {

    std::vector<std::unique_ptr<Expr>> arguments;

    if (currentToken.type == TokenType::RPAREN)
        return arguments;

    while (true) {

        auto arg = parseExpression();

        if (arg)
            arguments.push_back(std::move(arg));
        else
            return arguments;

        if (currentToken.type == TokenType::COMMA) {
            advance();
            continue;
        }

        break;
    }

    return arguments;
}

//Implementacija klase za grananje
std::unique_ptr<Stmt> Parser::parseIf() {

    advance(); // skip if

    auto condition = parseExpression();

    auto thenBody = parseBlock();

    std::vector<std::unique_ptr<Stmt>> elseBody;

    if (currentToken.type == TokenType::ELSE) {
        advance();
        elseBody = parseBlock();
    }

    return std::make_unique<IfStmt>(
        std::move(condition),
        std::move(thenBody),
        std::move(elseBody)
    );
}

//Implementacija While petlje
std::unique_ptr<Stmt> Parser::parseWhile() {

    advance(); // skip while

    auto condition = parseExpression();

    auto body = parseBlock();

    return std::make_unique<WhileStmt>(
        std::move(condition),
        std::move(body)
    );
}
