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

    int visit(AST* node) {
        if (dynamic_cast<BinOp*>(node)) {
            return visit_BinOp(dynamic_cast<BinOp*>(node));
        } else if (dynamic_cast<Num*>(node)) {
            return visit_Num(dynamic_cast<Num*>(node));
        } else if (dynamic_cast<UnaryOp*>(node)) {
            return visit_UnaryOp(dynamic_cast<UnaryOp*>(node));
        } else {
            throw runtime_error("Unknown AST node");
        }
    }

    int interpret() {
        AST* tree = parser.parse();
        return visit(tree);
    }
};

#endif

int main() {
    Scanner scanner("2--2*(2+3)*3+4");
    Parser parser(scanner);
    Interpreter interpreter(parser);
    int result = interpreter.interpret();
    cout << "Result: " << result << endl;
    return 0;
}