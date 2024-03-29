#ifndef INTERPRETER_CPP
#define INTERPRETER_CPP

#include <iostream>
#include <stdexcept>
#include "ast.cpp"
#include "parser.cpp"
#include "scanner.cpp"
#include "token.cpp"
#include "var.cpp"

using namespace std;

class Interpreter {
    
  private:
    Parser parser;
    unordered_map<string, Var*> GLOBAL_SCOPE;

  public:
    Interpreter(Parser& _) : parser(_) {}

    Var* visit_BinOp(BinOp* node) {
        int left = dynamic_cast<VarInt*>(visit(node->left))->value;
        int right = dynamic_cast<VarInt*>(visit(node->right))->value;
        if (node->op.type == Token::PLUS) {
            return new VarInt(left + right);
        } else if (node->op.type == Token::MINUS) {
            return new VarInt(left - right);
        } else if (node->op.type == Token::TIMES) {
            return new VarInt(left * right);
        } else if (node->op.type == Token::DIVIDE) {
            return new VarInt(left / right);
        }
        throw runtime_error("Invalid operation");
    }

    Var* visit_Bool(Bool* node) {
        return new VarBool(node->value);
    }

    Var* visit_Num(Num* node) {
        return new VarInt(node->value);
    }

    Var* visit_UnaryOp(UnaryOp* node) {
        int value = dynamic_cast<VarInt*>(visit(node->expr))->value;
        if (node->op.type == Token::PLUS) {
            return new VarInt(+value);
        } else if (node->op.type == Token::MINUS) {
            return new VarInt(-value);
        }
        throw runtime_error("Invalid operation");
    }

    void visit_Compound(Compound* node) {
        for (AST* child : node->children) {
            visit(child);
        }
    }

    void visit_NoOp(NoOp* node) {}

    void visit_Assign(Assign* node) {
        string var_name = node->left->value;
        if (GLOBAL_SCOPE.find(var_name) == GLOBAL_SCOPE.end()) {
            GLOBAL_SCOPE.insert({var_name, visit(node->right)});
        } else {
            GLOBAL_SCOPE[var_name] = visit(node->right);
        }
    }

    Var* visit_Variable(Variable* node) {
        string var_name = node->value;
        if (GLOBAL_SCOPE.find(var_name) != GLOBAL_SCOPE.end()) {
            return GLOBAL_SCOPE[var_name];
        } else {
            throw runtime_error(string("NameError: ") + var_name);
        }
    }

    Var* visit(AST* node) {
        if (dynamic_cast<BinOp*>(node)) {
            return visit_BinOp(dynamic_cast<BinOp*>(node));
        } else if (dynamic_cast<Bool*>(node)) {
            return visit_Bool(dynamic_cast<Bool*>(node));
        } else if (dynamic_cast<Num*>(node)) {
            return visit_Num(dynamic_cast<Num*>(node));
        } else if (dynamic_cast<UnaryOp*>(node)) {
            return visit_UnaryOp(dynamic_cast<UnaryOp*>(node));
        } else if (dynamic_cast<Compound*>(node)) {
            visit_Compound(dynamic_cast<Compound*>(node));
        } else if (dynamic_cast<NoOp*>(node)) {
            visit_NoOp(dynamic_cast<NoOp*>(node));
        } else if (dynamic_cast<Assign*>(node)) {
            visit_Assign(dynamic_cast<Assign*>(node));
        } else if (dynamic_cast<Variable*>(node)) {
            return visit_Variable(dynamic_cast<Variable*>(node));
        } else {
            throw runtime_error("Unknown AST node");
        }
        return 0;
    }

    int interpret() {
        AST* tree = parser.parse();
        visit(tree);
        for (const auto& pair : GLOBAL_SCOPE) {
            if (dynamic_cast<VarBool*>(pair.second)) {
                cout << pair.first << ": " << dynamic_cast<VarBool*>(pair.second)->value << endl;
            } else if (dynamic_cast<VarInt*>(pair.second)) {
                cout << pair.first << ": " << dynamic_cast<VarInt*>(pair.second)->value << endl;
            }
            //cout << pair.first << ": " << pair.second->value << endl;
        }
        return 0;
    }
};

#endif

int main() {
    Scanner scanner("a = 2\"\"\"test\"\"\"\na = a + a * a\njohn = False\nhubert = 3 * 5");
    Parser parser(scanner);
    Interpreter interpreter(parser);
    interpreter.interpret();
    return 0;
}