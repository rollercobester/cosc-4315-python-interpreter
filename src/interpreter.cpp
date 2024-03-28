#ifndef INTERPRETER_CPP
#define INTERPRETER_CPP

#include <iostream>
#include <stdexcept>
#include "ast.cpp"
#include "parser.cpp"
#include "scanner.cpp"
#include "token.cpp"

using namespace std;

class Interpreter {
    
  private:
    Parser parser;
    unordered_map<string, int> GLOBAL_SCOPE;

  public:
    Interpreter(Parser& _) : parser(_) {}

    int visit_BinOp(BinOp* node) {
        if (node->op.type == Token::PLUS) {
            return visit(node->left) + visit(node->right);
        } else if (node->op.type == Token::MINUS) {
            return visit(node->left) - visit(node->right);
        } else if (node->op.type == Token::TIMES) {
            return visit(node->left) * visit(node->right);
        } else if (node->op.type == Token::DIVIDE) {
            return visit(node->left) / visit(node->right);
        }
        throw runtime_error("Invalid operation");
    }

    int visit_Num(Num* node) {
        return node->value;
    }

    int visit_UnaryOp(UnaryOp* node) {
        if (node->op.type == Token::PLUS) {
            return +visit(node->expr);
        } else if (node->op.type == Token::MINUS){
            return -visit(node->expr);
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

    int visit_Variable(Variable* node) {
        string var_name = node->value;
        if (GLOBAL_SCOPE.find(var_name) != GLOBAL_SCOPE.end()) {
            return GLOBAL_SCOPE.at(var_name);;
        } else {
            throw runtime_error(string("NameError: ") + var_name);
        }
    }

    int visit(AST* node) {
        if (dynamic_cast<BinOp*>(node)) {
            return visit_BinOp(dynamic_cast<BinOp*>(node));
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
            visit_Variable(dynamic_cast<Variable*>(node));
        } else {
            throw runtime_error("Unknown AST node");
        }
        return 0;
    }

    int interpret() {
        AST* tree = parser.parse();
        visit(tree);
        for (const auto& pair : GLOBAL_SCOPE) {
            cout << pair.first << ": " << pair.second << endl;
        }
        return 0;
    }
};

#endif

int main() {
    Scanner scanner("a = 2\na =  3 + 2\njohn = 35\nhubert = 3 * 5");
    Parser parser(scanner);
    Interpreter interpreter(parser);
    interpreter.interpret();
    return 0;
}