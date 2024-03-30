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

    ConditionalNode(AST* condition, AST* if_body, AST* else_body) {
      this->condition = condition;
      this->if_body = if_body;
      this->else_body = else_body;
    }
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

class CompoundNode : public AST {
  public:
    vector<AST*> children;
    CompoundNode() {}
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