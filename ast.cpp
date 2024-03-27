#ifndef AST_CPP
#define AST_CPP

#include "token.cpp"

class AST {
  public:
    virtual ~AST() {}
};

// Class representing binary operations
class BinOp : public AST {
  public:
    AST* left;
    Token op;
    AST* right;

    BinOp(AST* left, Token op, AST* right) : left(left), op(op), right(right) {}
};

// Class representing numeric literals
class Num : public AST {
  public:
    Token token;
    int value;

    Num(Token token) : token(token), value(stoi(token.value)) {}
};

#endif