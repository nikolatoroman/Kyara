#pragma once

#include <memory>
#include <string>
#include <iostream>

\\ Abstract Syntax tree klase

\\Klasa za statement-e
class Stmt {
public:
    virtual ~Stmt() = default;

    virtual void print(int indent = 0) = 0;
};


\\Klasa za izraze
class Expr {
public:
    virtual ~Expr() = default;

    virtual void print(int indent = 0) = 0;
};

\\Numericki izrazi
class NumberExpr : public Expr {
public:
    int value;

    NumberExpr(int val)
        : value(val) {}


    void print(int indent = 0) override {

    for (int i = 0; i < indent; i++)
        std::cout << " ";

    std::cout
        << "NumberExpr("
        << value
        << ")"
        << std::endl;
    }  
};


\\Izrazi sa promenjivama
class VariableExpr : public Expr {
public:
    std::string name;

    VariableExpr(const std::string& n)
        : name(n) {}

    void print(int indent = 0) override {

        for (int i = 0; i < indent; i++)
            std::cout << " ";

        std::cout
            << "VariableExpr("
            << name
            << ")"
            << std::endl;
    }
};


\\Binarni izrazi
class BinaryExpr : public Expr {
public:
    char op;

    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;

    BinaryExpr(
        char oper,
        std::unique_ptr<Expr> lhs,
        std::unique_ptr<Expr> rhs
    )
        : op(oper),
          left(std::move(lhs)),
          right(std::move(rhs)) {}

    void print(int indent = 0) override {

    for (int i = 0; i < indent; i++)
        std::cout << " ";

    std::cout
        << "BinaryExpr("
        << op
        << ")"
        << std::endl;

    left->print(indent + 2);
    right->print(indent + 2);
    }
};

\\Klasa koja omogucava deklaraciju promenjivim
class VarDeclStmt : public Stmt {
public:
    std::string name;

    std::unique_ptr<Expr> initializer;

    VarDeclStmt(
        const std::string& n,
        std::unique_ptr<Expr> init
    )
        : name(n),
          initializer(std::move(init)) {}

    void print(int indent = 0) override {

        for (int i = 0; i < indent; i++)
            std::cout << " ";

        std::cout
            << "VarDeclStmt("
            << name
            << ")"
            << std::endl;

        initializer->print(indent + 2);
    }
};
