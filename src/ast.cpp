#ifndef AST_CPP
#define AST_CPP

#include <vector>
#include "token.cpp"

class AST {
  public:
    virtual ~AST() {}
};

class ConditionalNode : public AST {
  public:
    AST* condition;
    AST* if_body;
    AST* else_body;

    ConditionalNode(AST* condition, AST* if_body, AST* else_body) : condition(condition), if_body(if_body), else_body(else_body) {}
};

class UnaryOpNode : public AST {
  public:
    Token op;
    AST* expr;
    UnaryOpNode(Token op, AST* expr) : op(op), expr(expr) {}
};

class BinOpNode : public AST {
  public:
    AST* left;
    Token op;
    AST* right;
    BinOpNode(AST* left, Token op, AST* right) : left(left), op(op), right(right) {}
};

class StringNode : public AST {
  public:
    Token token;
    string value;
    StringNode(Token token) : token(token), value(token.value) {}
};

class BoolNode : public AST {
  public:
    Token token;
    bool value;
    BoolNode(Token token) : token(token), value(token.value == "True") {}
};

class IntNode : public AST {
  public:
    Token token;
    int value;
    IntNode(Token token) : token(token), value(stoi(token.value)) {}
};

class BlockNode : public AST {
  public:
    vector<AST*> children;
    BlockNode() {}
};

class FunctionNode : public AST {
  public:
    Token token;
    vector<AST*> parameters;
    FunctionNode(Token token) : token(token) {}
};

class VariableNode : public AST {
  public:
    Token token;
    string value;
    VariableNode(Token token) : token(token), value(token.value) {}
};

class AssignNode : public AST {
  public:
    VariableNode* left;
    Token op;
    AST* right;
    AssignNode(VariableNode* left, Token op, AST* right) : left(left), op(op), right(right) {}
};


class NoOp : public AST {
  public:
    ;
};

#endif