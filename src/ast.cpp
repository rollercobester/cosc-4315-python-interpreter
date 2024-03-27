#ifndef AST_CPP
#define AST_CPP

#include <vector>
#include "token.cpp"

class AST {
  public:
    virtual ~AST() {}
};

class UnaryOp : public AST {
  public:
    Token op;
    AST* expr;

    UnaryOp(Token op, AST* expr) : op(op), expr(expr) {}
};

class BinOp : public AST {
  public:
    AST* left;
    Token op;
    AST* right;

    BinOp(AST* left, Token op, AST* right) : left(left), op(op), right(right) {}
};

class Num : public AST {
  public:
    Token token;
    int value;

    Num(Token token) : token(token), value(stoi(token.value)) {}
};

class Compound : public AST {
  public:
    vector<AST*> children;
    Compound() {}
};

class Assign : public AST {
  public:
    AST* left;
    Token op;
    AST* right;

    Assign(AST* left, Token op, AST* right) : left(left), op(op), right(right) {}
};

class Variable : public AST {
  public:
    Token token;
    int value;

    Variable(Token token) : token(token), value(stoi(token.value)) {}
};

class NoOp : public AST {
  public:
    ;
};

#endif