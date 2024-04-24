#ifndef AST_CPP
#define AST_CPP

#include <vector>
#include "token.cpp"

class AST {
  public:
    virtual ~AST() {}
};

class BlockNode : public AST {
  public:
    vector<AST*> children;
    BlockNode() {}
};

class FunctionNode : public AST {
  public:
    string id;
    BlockNode* function_body = nullptr;
    vector<string> parameters;
    FunctionNode(string name) : id(name) {}
    int get_num_parameters() { return parameters.size(); }
};

class FunctionCallNode : public AST {
  public:
    string id;
    vector<AST*> parameters;
    FunctionCallNode(string name) : id(name) {}
    int get_num_parameters() { return parameters.size(); }
};

class ReturnNode : public AST {
  public:
    AST* value;
    ReturnNode(AST* v) : value(v) {}
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

class BinaryOpNode : public AST {
  public:
    AST* left;
    Token op;
    AST* right;
    BinaryOpNode(AST* left, Token op, AST* right) : left(left), op(op), right(right) {}
};

class StringNode : public AST {
  public:
    string text;
    StringNode(string t) : text(t) {}
};

class BoolNode : public AST {
  public:
    bool value;
    BoolNode(bool v) : value(v) {}
};

class IntNode : public AST {
  public:
    int value;
    IntNode(int v) : value(v) {}
};

class VariableNode : public AST {
  public:
    string id;
    VariableNode(string name) : id(name) {}
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