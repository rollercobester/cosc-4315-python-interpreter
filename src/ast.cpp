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

class Variable : public AST {
  public:
    Token token;
    string value;

    Variable(Token token) : token(token), value(token.value) {}
};

class Assign : public AST {
  public:
    Variable* left;
    Token op;
    AST* right;

    Assign(Variable* left, Token op, AST* right) : left(left), op(op), right(right) {}
};


class NoOp : public AST {
  public:
    ;
};

#endif