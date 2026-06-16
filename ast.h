#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <vector>

//Abstract Syntax tree klase


//Klasa za statement-e
class Stmt {
public:
    virtual ~Stmt() = default;

    virtual void print(int indent = 0) = 0;
};


//Klasa za izraze
class Expr {
public:
    virtual ~Expr() = default;

    virtual void print(int indent = 0) = 0;
};

//Numericki izrazi
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


//Izrazi sa promenjivama
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


//Binarni izrazi
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

//Klasa koja omogucava deklaraciju promenjivim
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


//klasa za dodelu vrednosti
class AssignStmt : public Stmt {
public:
    std::string name;
    std::unique_ptr<Expr> value;

    AssignStmt(
        const std::string& n,
        std::unique_ptr<Expr> v
    )
        : name(n),
          value(std::move(v)) {}

    void print(int indent = 0) override {

        for (int i = 0; i < indent; i++)
            std::cout << " ";

        std::cout
            << "AssignStmt("
            << name
            << ")"
            << std::endl;

        value->print(indent + 2);
    }
};


 // Klasa koja omogucava da se kompajluje vise izraza od jednom
class Program {
public:
    std::vector<std::unique_ptr<Stmt>> statements;

    void print() {

        std::cout << "Program" << std::endl;

        for (auto& stmt : statements) {
            stmt->print(2);
        }
    }
};

// Klasa za Return statement
class ReturnStmt : public Stmt {
public:
    std::unique_ptr<Expr> value;

    ReturnStmt(std::unique_ptr<Expr> val)
        : value(std::move(val)) {}

    void print(int indent = 0) override {

        for (int i = 0; i < indent; i++)
            std::cout << " ";

        std::cout << "ReturnStmt" << std::endl;

        value->print(indent + 2);
    }
};

// Klasa za deklarisanje funkcija, sa parametrima
class FunctionDecl : public Stmt {
public:
    std::string name;
    std::vector<std::string> parameters;
    std::vector<std::unique_ptr<Stmt>> body;

    FunctionDecl(
        const std::string& n,
        std::vector<std::string> params,
        std::vector<std::unique_ptr<Stmt>> b
    )
        : name(n),
          parameters(std::move(params)),
          body(std::move(b)) {}

    void print(int indent = 0) override {

        for (int i = 0; i < indent; i++)
            std::cout << " ";

        std::cout << "FunctionDecl(" << name << ")" << std::endl;

        for (int i = 0; i < indent + 2; i++)
            std::cout << " ";

        std::cout << "Parameters:";

        for (const auto& param : parameters) {
            std::cout << " " << param;
        }

        std::cout << std::endl;

        for (auto& stmt : body) {
            stmt->print(indent + 2);
        }
    }
};


// Klasa koja omogucava IF grananje
class IfStmt : public Stmt {
public:
    std::unique_ptr<Expr> condition;
    std::vector<std::unique_ptr<Stmt>> thenBody;
    std::vector<std::unique_ptr<Stmt>> elseBody;

    IfStmt(
        std::unique_ptr<Expr> cond,
        std::vector<std::unique_ptr<Stmt>> thenStmts,
        std::vector<std::unique_ptr<Stmt>> elseStmts
    )
        : condition(std::move(cond)),
          thenBody(std::move(thenStmts)),
          elseBody(std::move(elseStmts)) {}

    void print(int indent = 0) override {

        for (int i = 0; i < indent; i++)
            std::cout << " ";

        std::cout << "IfStmt" << std::endl;

        for (int i = 0; i < indent + 2; i++)
            std::cout << " ";

        std::cout << "Condition:" << std::endl;

        condition->print(indent + 4);

        for (int i = 0; i < indent + 2; i++)
            std::cout << " ";

        std::cout << "Then:" << std::endl;

        for (auto& stmt : thenBody)
            stmt->print(indent + 4);

        if (!elseBody.empty()) {
            for (int i = 0; i < indent + 2; i++)
                std::cout << " ";

            std::cout << "Else:" << std::endl;

            for (auto& stmt : elseBody)
                stmt->print(indent + 4);
        }
    }
};

// Klasa koja omogucava poziv funkcije, npr x = zbir(a, b);
class CallExpr : public Expr {
public:
    std::string callee;
    std::vector<std::unique_ptr<Expr>> arguments;

    CallExpr(
        const std::string& c,
        std::vector<std::unique_ptr<Expr>> args
    )
        : callee(c),
          arguments(std::move(args)) {}

    void print(int indent = 0) override {

        for (int i = 0; i < indent; i++)
            std::cout << " ";

        std::cout << "CallExpr(" << callee << ")" << std::endl;

        for (auto& arg : arguments) {
            arg->print(indent + 2);
        }
    }
};

