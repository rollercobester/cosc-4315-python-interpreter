#ifndef INTERPRETER_CPP
#define INTERPRETER_CPP

#include <iostream>
#include <fstream>
#include <stdexcept>
#include "ast.cpp"
#include "parser.cpp"
#include "scanner.cpp"
#include "scope_table.cpp"
#include "token.cpp"

using namespace std;

class Interpreter {
  private:
    Parser parser;
    ScopeTable scope_table;

  public:
    Interpreter(Parser& _) : parser(_), scope_table(ScopeTable()) {}

    /* Handles boolean operations */
    AST* compute_BoolOp(AST* first, Token op, AST* second = nullptr) {
        // Unary operations
        bool val1 = dynamic_cast<BoolNode*>(first)->value;
        if (second == nullptr) {
            if (op.type == Token::NOT) return new BoolNode(!val1);
            throw runtime_error("Invalid operation");
        }

        // Binary operations
        bool val2 = dynamic_cast<BoolNode*>(second)->value;
        if (op.type == Token::OR)         return new BoolNode(val1 || val2);
        if (op.type == Token::AND)        return new BoolNode(val1 && val2);
        if (op.type == Token::EQUALS)     return new BoolNode(val1 == val2);
        if (op.type == Token::NOT_EQUALS) return new BoolNode(val1 != val2);
        throw runtime_error("Invalid operation");
    }

    /* Handles integer operations */
    AST* compute_IntOp(AST* first, Token op, AST* second = nullptr) {
        // Unary operations
        int val1 = dynamic_cast<IntNode*>(first)->value;
        if (second == nullptr) {
            if (op.type == Token::PLUS)  return new IntNode(+val1);
            if (op.type == Token::MINUS) return new IntNode(-val1);
            throw runtime_error("Invalid operation");
        }

        // Binary operations
        int val2 = dynamic_cast<IntNode*>(second)->value;
        if (op.type == Token::PLUS)                return new IntNode(val1 + val2);
        if (op.type == Token::MINUS)               return new IntNode(val1 - val2);
        if (op.type == Token::TIMES)               return new IntNode(val1 * val2);
        if (op.type == Token::DIVIDE)              return new IntNode(val1 / val2);
        if (op.type == Token::EQUALS)              return new BoolNode(val1 == val2);
        if (op.type == Token::NOT_EQUALS)          return new BoolNode(val1 != val2);
        if (op.type == Token::LESS_THAN)           return new BoolNode(val1 <  val2);
        if (op.type == Token::GREATER_THAN)        return new BoolNode(val1 >  val2);
        if (op.type == Token::LESS_THAN_EQUALS)    return new BoolNode(val1 <= val2);
        if (op.type == Token::GREATER_THAN_EQUALS) return new BoolNode(val1 >= val2);
        throw runtime_error("Invalid operation");
    }

    /* Handles string operations */
    AST* compute_StringOp(AST* first, Token op, AST* second) {
        string text1 = dynamic_cast<StringNode*>(first)->text;
        string text2 = dynamic_cast<StringNode*>(second)->text;
        if (op.type == Token::PLUS)                return new StringNode(text1 + text2);
        if (op.type == Token::EQUALS)              return new BoolNode(text1 == text2);
        if (op.type == Token::NOT_EQUALS)          return new BoolNode(text1 != text2);
        if (op.type == Token::LESS_THAN)           return new BoolNode(text1 <  text2);
        if (op.type == Token::GREATER_THAN)        return new BoolNode(text1 >  text2);
        if (op.type == Token::LESS_THAN_EQUALS)    return new BoolNode(text1 <= text2);
        if (op.type == Token::GREATER_THAN_EQUALS) return new BoolNode(text1 >= text2);
        throw runtime_error("Invalid operation");
    }

    /* Handles two-operand operations */
    AST* visit_BinaryOp(BinaryOpNode* node) {
        AST* left = visit(node->left);
        AST* right = visit(node->right);
        if (dynamic_cast<BoolNode*>(left) && dynamic_cast<BoolNode*>(right))
            return compute_BoolOp(left, node->op, right);
        if (dynamic_cast<IntNode*>(left) && dynamic_cast<IntNode*>(right))
            return compute_IntOp(left, node->op, right);
        if (dynamic_cast<StringNode*>(left) && dynamic_cast<StringNode*>(right))
            return compute_StringOp(left, node->op, right);
        throw runtime_error("Invalid operand type");
    }

    /* Handles one-operand operations */
    AST* visit_UnaryOp(UnaryOpNode* node) {
        AST* value = visit(node->expr);
        if (dynamic_cast<BoolNode*>(value))
            return compute_BoolOp(value, node->op);
        if (dynamic_cast<IntNode*>(value))
            return compute_IntOp(value, node->op);
        else throw runtime_error("Invalid operand type");
    }

    AST* visit_Variable(VariableNode* node) {
        AST* value = scope_table.get(node->id);
        if (value != nullptr) return value;
        else throw runtime_error("NameError: \"" + node->id + "\"");
    }

    void visit_PrintFunction(FunctionCallNode* node) {
        string result = "";
        for (AST* param : node->parameters) {
            AST* var =  visit(param);
            if (StringNode* varString = dynamic_cast<StringNode*>(var)) {
                result += varString->text;
            } else if (BoolNode* varBool = dynamic_cast<BoolNode*>(var)) {
                result += (varBool->value ? "True" : "False");
            } else if (IntNode* varInt = dynamic_cast<IntNode*>(var)) {
                result += to_string(varInt->value);
            }
            result += " ";
        }
        result[result.length()-1] = '\n';
        cout << result;
    }

    AST* visit_FunctionCall(FunctionCallNode* node) {
        if (node->id == "print") {
            visit_PrintFunction(node);
        } else if (FunctionNode* function = dynamic_cast<FunctionNode*>(scope_table.get(node->id))) {
            if (function->get_num_parameters() == node->get_num_parameters()) {
                scope_table.increase_scope();
                for (int i = 0; i < function->get_num_parameters(); i++) {
                    string parameter_id = function->parameters.at(i);
                    AST* parameter_value = visit(node->parameters.at(i));
                    scope_table.set(parameter_id, parameter_value);
                }
                AST* result = visit_Block(dynamic_cast<BlockNode*>(function->function_body));
                scope_table.decrease_scope();
                return result;
            } else throw runtime_error("Invalid number of parameters");
        } else throw runtime_error("Invalid function");
    }

    void visit_FunctionDefinition(FunctionNode* node) {
        scope_table.set(node->id, node);
    }

    AST* visit_Return(ReturnNode* node) {
        return visit(node->value);
    }

    AST* visit_Conditional(ConditionalNode* node) {
        bool condition = dynamic_cast<BoolNode*>(visit(node->condition))->value;
        return (condition ? visit(node->if_body) : visit(node->else_body));
    }

    void visit_Assign(AssignNode* node) {
        scope_table.set(node->left->id, visit(node->right));
    }

    AST* visit_Block(BlockNode* node) {
        for (AST* child : node->children) {
            if (dynamic_cast<FunctionCallNode*>(child)) {
                visit(child);
            } else {
                AST* result = visit(child);
                if (result != nullptr) {
                    return result;
                }
            }
        }
        return nullptr;
    }

    AST* visit(AST* node_) {
        if      (FunctionCallNode* node = dynamic_cast<FunctionCallNode*>(node_)) return visit_FunctionCall(node);
        else if (ConditionalNode* node = dynamic_cast<ConditionalNode*>(node_))   return visit_Conditional(node);
        else if (VariableNode* node = dynamic_cast<VariableNode*>(node_))         return visit_Variable(node);
        else if (BinaryOpNode* node = dynamic_cast<BinaryOpNode*>(node_))         return visit_BinaryOp(node);
        else if (UnaryOpNode* node = dynamic_cast<UnaryOpNode*>(node_))           return visit_UnaryOp(node);
        else if (ReturnNode* node = dynamic_cast<ReturnNode*>(node_))             return visit_Return(node); 
        else if (BlockNode* node = dynamic_cast<BlockNode*>(node_))               return visit_Block(node);
        else if (StringNode* node = dynamic_cast<StringNode*>(node_))             return node;
        else if (BoolNode* node = dynamic_cast<BoolNode*>(node_))                 return node;
        else if (IntNode* node = dynamic_cast<IntNode*>(node_))                   return node;
        else if (NoOp* node = dynamic_cast<NoOp*>(node_))                         return nullptr;
        else if (FunctionNode* node = dynamic_cast<FunctionNode*>(node_))         visit_FunctionDefinition(node);
        else if (AssignNode* node = dynamic_cast<AssignNode*>(node_))             visit_Assign(node);
        else throw runtime_error("Unknown AST node");
        return 0;
    }

    int interpret() {
        AST* tree = parser.program();
        if (parser.DEBUG_MODE) {
            cout << endl << "Program output:" << endl;
            cout << "-------------------------------" << endl;
            visit(tree);
            cout << "-------------------------------" << endl << endl;
        } else visit(tree);
        return 0;
    }
};

string readFileIntoString(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filePath << endl;
        return "";
    }
    string content;
    string line;
    while (getline(file, line)) {
        content += line + "\n";
    }
    file.close();
    return content;
}

#endif

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <file_path>" << endl;
        return 1;
    }

    string filePath = argv[1];
    string fileContent = readFileIntoString(filePath);

    Scanner scanner(fileContent);
    Parser parser(scanner);
    Interpreter interpreter(parser);

    if (parser.DEBUG_MODE) {
        cout << endl << "Evaluating file:" << endl;
        cout << "-------------------------------" << endl;
        cout << fileContent << endl;
        cout << "-------------------------------" << endl << endl;
    }

    interpreter.interpret();
    return 0;
}