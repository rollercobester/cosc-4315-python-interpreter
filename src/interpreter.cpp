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

    void visit_Block(BlockNode* node) {
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

    void visit_Conditional(ConditionalNode* node) {
        bool condition = dynamic_cast<VarBool*>(visit(node->condition))->value;
        if (condition == true)
            visit(node->if_body);
        else
            visit(node->else_body);
    }

    Var* visit(AST* node_) {
        if (ConditionalNode* node = dynamic_cast<ConditionalNode*>(node_))
            visit_Conditional(node);
        else if (VariableNode* node = dynamic_cast<VariableNode*>(node_))
            return visit_Variable(node);
        else if (UnaryOpNode* node = dynamic_cast<UnaryOpNode*>(node_))
            return visit_UnaryOp(node);
        else if (AssignNode* node = dynamic_cast<AssignNode*>(node_))
            visit_Assign(node);
        else if (BlockNode* node = dynamic_cast<BlockNode*>(node_))
            visit_Block(node);
        else if (BinOpNode* node = dynamic_cast<BinOpNode*>(node_))
            return visit_BinOp(node);
        else if (BoolNode* node = dynamic_cast<BoolNode*>(node_))
            return visit_Bool(node);
        else if (IntNode* node = dynamic_cast<IntNode*>(node_))
            return visit_Num(node);
        else if (NoOp* node = dynamic_cast<NoOp*>(node_))
            visit_NoOp(node);
        else throw runtime_error("Unknown AST node");
        return 0;
    }

    int interpret() {
        AST* tree = parser.program();
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
    string text= "a = 2\nif a == 3:\n a = 3\nc = 3==1+2\nc= !c\nhubert = 3 * 5";
    cout << "Evaluating file:" << endl << endl;
    cout << "-------------------------------" << endl;
    cout << text << endl;
    cout << "-------------------------------" << endl << endl;
    Scanner scanner(text);
    Parser parser(scanner);
    Interpreter interpreter(parser);
    interpreter.interpret();
    return 0;
}