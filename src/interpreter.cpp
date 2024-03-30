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

    VarBool* visit_Bool(BoolNode* node) {
        return new VarBool(node->value);
    }

    VarInt* visit_Num(IntNode* node) {
        return new VarInt(node->value);
    }

    /* Handles two-operand boolean operations */
    Var* compute_BoolBinOp(Var* left, Token op, Var* right) {
        bool val1 = dynamic_cast<VarBool*>(left)->value;
        bool val2 = dynamic_cast<VarBool*>(right)->value;
        if (op.type == Token::EQUALS)              return new VarBool(val1 == val2);
        if (op.type == Token::NOT_EQUALS)          return new VarBool(val1 != val2);
        else throw runtime_error("Invalid operation");
    }

    /* Handles two-operand numeric operations */
    Var* compute_NumBinOp(Var* left, Token op, Var* right) {
        int val1 = dynamic_cast<VarInt*>(left)->value;
        int val2 = dynamic_cast<VarInt*>(right)->value;
        if (op.type == Token::PLUS)                return new VarInt(val1 + val2);
        if (op.type == Token::MINUS)               return new VarInt(val1 - val2);
        if (op.type == Token::TIMES)               return new VarInt(val1 * val2);
        if (op.type == Token::DIVIDE)              return new VarInt(val1 / val2);
        if (op.type == Token::EQUALS)              return new VarBool(val1 == val2);
        if (op.type == Token::NOT_EQUALS)          return new VarBool(val1 != val2);
        if (op.type == Token::LESS_THAN)           return new VarBool(val1 <  val2);
        if (op.type == Token::GREATER_THAN)        return new VarBool(val1 >  val2);
        if (op.type == Token::LESS_THAN_EQUALS)    return new VarBool(val1 <= val2);
        if (op.type == Token::GREATER_THAN_EQUALS) return new VarBool(val1 >= val2);
        else throw runtime_error("Invalid operation");
    }

    /* Handles two-operand operations */
    Var* visit_BinOp(BinOpNode* node) {
        Var* left = visit(node->left);
        Var* right = visit(node->right);
        if (dynamic_cast<VarBool*>(left) && dynamic_cast<VarBool*>(right))
            return compute_BoolBinOp(left, node->op, right);
        if (dynamic_cast<VarInt*>(left) && dynamic_cast<VarInt*>(right))
            return compute_NumBinOp(left, node->op, right);
        else throw runtime_error("Invalid operand type");
    }

    /* Handles one-operand boolean operations */
    VarBool* compute_BoolUnOp(Var* value, Token op) {
        bool val = dynamic_cast<VarBool*>(value)->value;
        if (op.type == Token::NOT || op.type == Token::EXCLAMATION)
            return new VarBool(!val);
        else throw runtime_error("Invalid operation");
    }

    /* Handles one-operand numeric operations */
    VarInt* compute_NumUnOp(Var* value, Token op) {
        int val = dynamic_cast<VarInt*>(value)->value;
        if (op.type == Token::PLUS)  return new VarInt(+val);
        if (op.type == Token::MINUS) return new VarInt(-val);
        else throw runtime_error("Invalid operation");
    }

    /* Handles one-operand operations */
    Var* visit_UnaryOp(UnaryOpNode* node) {
        Var* value = visit(node->expr);
        if (dynamic_cast<VarBool*>(value))
            return compute_BoolUnOp(value, node->op);
        if (dynamic_cast<VarInt*>(value))
            return compute_NumUnOp(value, node->op);
        else throw runtime_error("Invalid operand type");
    }

    void visit_Compound(CompoundNode* node) {
        for (AST* child : node->children) {
            visit(child);
        }
    }

    void visit_NoOp(NoOp* node) {}

    void visit_Assign(AssignNode* node) {
        string var_name = node->left->value;
        if (GLOBAL_SCOPE.find(var_name) == GLOBAL_SCOPE.end()) {
            GLOBAL_SCOPE.insert({var_name, visit(node->right)});
        } else {
            GLOBAL_SCOPE[var_name] = visit(node->right);
        }
    }

    Var* visit_Variable(VariableNode* node) {
        string var_name = node->value;
        if (GLOBAL_SCOPE.find(var_name) != GLOBAL_SCOPE.end()) {
            return GLOBAL_SCOPE[var_name];
        } else {
            throw runtime_error(string("NameError: ") + var_name);
        }
    }

    Var* visit(AST* node) {
        if (dynamic_cast<BinOpNode*>(node)) {
            return visit_BinOp(dynamic_cast<BinOpNode*>(node));
        } else if (dynamic_cast<BoolNode*>(node)) {
            return visit_Bool(dynamic_cast<BoolNode*>(node));
        } else if (dynamic_cast<IntNode*>(node)) {
            return visit_Num(dynamic_cast<IntNode*>(node));
        } else if (dynamic_cast<UnaryOpNode*>(node)) {
            return visit_UnaryOp(dynamic_cast<UnaryOpNode*>(node));
        } else if (dynamic_cast<CompoundNode*>(node)) {
            visit_Compound(dynamic_cast<CompoundNode*>(node));
        } else if (dynamic_cast<NoOp*>(node)) {
            visit_NoOp(dynamic_cast<NoOp*>(node));
        } else if (dynamic_cast<AssignNode*>(node)) {
            visit_Assign(dynamic_cast<AssignNode*>(node));
        } else if (dynamic_cast<VariableNode*>(node)) {
            return visit_Variable(dynamic_cast<VariableNode*>(node));
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
        }
        return 0;
    }
};

#endif

int main() {
    Scanner scanner("a = 2\nif a == 2:\n a = 3\nc = 3==1+2\nc= !c\nhubert = 3 * 5");
    Parser parser(scanner);
    Interpreter interpreter(parser);
    interpreter.interpret();
    return 0;
}