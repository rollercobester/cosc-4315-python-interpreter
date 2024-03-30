#ifndef PARSER_CPP
#define PARSER_CPP

#include <iostream>
#include <stack>
#include <stdexcept>
#include "ast.cpp"
#include "scanner.cpp"
#include "token.cpp"

class Parser {
    
  private:
    Scanner scanner;
    Token current_token;
    stack<int> indent_level;
    int debug_depth = 0;


  public:
    Parser(Scanner &_) : scanner(_), current_token(scanner.get_next_token()) {
        indent_level.push(0);
    }

    void error() {
        throw runtime_error("Invalid syntax");
    }

    void eat(Token::TokenType type) {
        if (current_token.type == type) {
            current_token = scanner.get_next_token();
        } else {
            error();
        }
    }

    void parse_indent() {
        if (current_token.type == Token::INDENT) {
            int indent = stoi(current_token.value);
            while (indent < indent_level.top()) {
                indent_level.pop();
            }
            if (indent == indent_level.top()) {
                eat(Token::INDENT);
                return;
            }
        }
        error();
    }

    AST* factor() {
        debugPrint("<factor>");
        Token token = current_token;
        AST* node;
        if (token.type == Token::EXCLAMATION || token.type == Token::NOT) {
            eat(token.type);
            node = new UnaryOpNode(token, math_expr());
        } else if (token.type == Token::PLUS) {
            eat(Token::PLUS);
            node = new UnaryOpNode(token, factor());
        } else if (token.type == Token::MINUS) {
            eat(Token::MINUS);
            node = new UnaryOpNode(token, factor());
        } else if (token.type == Token::BOOL) {
            eat(Token::BOOL);
            node = new BoolNode(token);
        } else if (token.type == Token::INT) {
            eat(Token::INT);
            node = new IntNode(token);
        } else if (token.type == Token::L_PAREN) {
            eat(Token::L_PAREN);
            node = math_expr();
            eat(Token::R_PAREN);
        } else {
            eat(Token::ID);
            node = new VariableNode(token);
        }
        debugPrint("</factor>");
        return node;
    }

    AST* term() {
        debugPrint("<term>");
        AST* node = factor();
        while (current_token.type == Token::TIMES || current_token.type == Token::DIVIDE) {
            Token operator_token = current_token;
            eat(operator_token.type);
            node = new BinOpNode(node, operator_token, factor());
        }
        debugPrint("</term>");
        return node;
    }

    AST* math_expr() {
        debugPrint("<math_expr>");
        AST* node = term();
        while (current_token.type == Token::PLUS || current_token.type == Token::MINUS) {
            Token operator_token = current_token;
            eat(operator_token.type);
            node = new BinOpNode(node, operator_token, term());
        }
        debugPrint("</math_expr>");
        return node;
    }

    AST* expr() {
        debugPrint("<expr>");
        AST* node = math_expr();
        //AST* node = expr();?
        while (current_token.type == Token::EQUALS
            || current_token.type == Token::NOT_EQUALS
            || current_token.type == Token::LESS_THAN
            || current_token.type == Token::GREATER_THAN 
            || current_token.type == Token::LESS_THAN_EQUALS
            || current_token.type == Token::GREATER_THAN_EQUALS) {
            
            Token operator_token = current_token;
            eat(operator_token.type);
            //node = new BinOpNode(node, operator_token, expr());?
            node = new BinOpNode(node, operator_token, math_expr());
        }
        debugPrint("</expr>");
        return node;
    }

    AST* block() {
        debugPrint("<block>");
        BlockNode* root = new BlockNode();
        int current_indent = indent_level.top();
        root->children.push_back(statement());
        while (indent_level.top() == current_indent && current_token.type == Token::END_LINE) {
            eat(Token::END_LINE);
            root->children.push_back(statement());
        }
        if (current_token.type == Token::ID) {
            error();
        }
        debugPrint("</block>");
        return root;
    }

    AST* else_statement() {
        debugPrint("<else>");
        AST* node;
        if (current_token.type == Token::ELSE) {
            eat(Token::ELSE);
            eat(Token::COLON);
            eat(Token::END_LINE);
            node = block();
        } else {
            node = empty();
        }
        debugPrint("</else>");
        return node;
    }

    AST* if_statement() {
        debugPrint("<if>");
        eat(Token::IF);
        AST* condition = expr();
        eat(Token::COLON);
        eat(Token::END_LINE);
        int next_line_indent = stoi(current_token.value);
        if (next_line_indent <= indent_level.top())
            error();
        else indent_level.push(next_line_indent);
        AST* if_body = block();
        AST* else_body = else_statement();
        debugPrint("</if>");
        return new ConditionalNode(condition, if_body, else_body);
    }

    AST* statement() {
        parse_indent();
        debugPrint("<statement>");
        AST* node;
        if (current_token.type == Token::IF)
            node = if_statement();
        if (current_token.type == Token::ID)
            node = assignment_statement();
        debugPrint("</statement>");
        if (node != nullptr) return node;
        else return empty();
    }

    AST* assignment_statement() {
        debugPrint("<assign>");
        VariableNode* left = variable();
        Token token = current_token;
        eat(Token::ASSIGN);
        AST* right = expr();
        debugPrint("</assign>");
        return new AssignNode(left, token, right);
    }

    VariableNode* variable() {
        debugPrint("<var>");
        VariableNode* node = new VariableNode(current_token);
        eat(Token::ID);
        debugPrint("</var>");
        return node;
    }

    AST* empty() {
        debugPrint("<empty>");
        AST* node = new NoOp();
        debugPrint("</empty>");
        return node;
    }

    AST* program() {
        debugPrint("<program>");
        AST* node = block();
        if (current_token.type != Token::EOF_TOKEN) {
            error();
        }
        debugPrint("</program>");
        return node;
    }

    void debugPrint(string text) {
        if (text[1] == '/') debug_depth--;
        for (int i = 0; i < debug_depth; i++) {
            cout << " ";
        }
        if (text[1] != '/') debug_depth++;
        cout << text << endl;
    }
};

#endif