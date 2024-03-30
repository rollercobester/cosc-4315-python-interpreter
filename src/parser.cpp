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
        cout << "$14" << endl;
        if (current_token.type == Token::INDENT) {
            //int prev_indent = indent_level.top();
            int indent = stoi(current_token.value);
            while (indent < indent_level.top()) {
                indent_level.pop();
            }
            if (indent == indent_level.top()) {
                eat(Token::INDENT);
                return;
            }
            /*while (indent < indent_level.top()) {
                indent_level.pop();
            }
            if (indent == indent_level.top()) return;*/
        }
        error();
    }

    AST* factor() {
        cout << "$5" << endl;
        Token token = current_token;
        if (token.type == Token::EXCLAMATION || token.type == Token::NOT) {
            eat(token.type);
            return new UnaryOpNode(token, expr());
        } else if (token.type == Token::PLUS) {
            eat(Token::PLUS);
            return new UnaryOpNode(token, factor());
        } else if (token.type == Token::MINUS) {
            eat(Token::MINUS);
            return new UnaryOpNode(token, factor());
        } else if (token.type == Token::BOOL) {
            eat(Token::BOOL);
            return new BoolNode(token);
        } else if (token.type == Token::INT) {
            cout << "$5.2" << endl;
            eat(Token::INT);
            cout << "$5.3" << endl;
            return new IntNode(token);
        } else if (token.type == Token::L_PAREN) {
            eat(Token::L_PAREN);
            AST* node = expr();
            eat(Token::R_PAREN);
            return node;
        } else {
            cout << "$5.1" << endl;
            eat(Token::ID);
            return new VariableNode(token);
        }
    }

    AST* term() {
        cout << "$4" << endl;
        AST* node = factor();
        cout << "$6" << endl;
        while (current_token.type == Token::TIMES || current_token.type == Token::DIVIDE) {
            Token operator_token = current_token;
            eat(operator_token.type);
            node = new BinOpNode(node, operator_token, factor());
        }
        return node;
    }

    AST* expr() {
        cout << "$3" << endl;
        AST* node = term();
        cout << "$7" << endl;
        while (current_token.type == Token::PLUS || current_token.type == Token::MINUS) {
            Token operator_token = current_token;
            eat(operator_token.type);
            node = new BinOpNode(node, operator_token, term());
        }
        return node;
    }

    AST* bool_expr() {
        cout << "$2" << endl;
        AST* node = expr();
        //AST* node = bool_expr();?
        cout << "$8" << endl;
        while (current_token.type == Token::EQUALS
            || current_token.type == Token::NOT_EQUALS
            || current_token.type == Token::LESS_THAN
            || current_token.type == Token::GREATER_THAN 
            || current_token.type == Token::LESS_THAN_EQUALS
            || current_token.type == Token::GREATER_THAN_EQUALS) {
            cout << "$8.1" << endl;
            Token operator_token = current_token;
            eat(operator_token.type);
            //node = new BinOpNode(node, operator_token, bool_expr());?
            node = new BinOpNode(node, operator_token, expr());
        }
        return node;
    }

    AST* compound_statement() {
        CompoundNode* root = new CompoundNode();
        cout << "$11" << endl;
        root->children.push_back(statement());
        while (current_token.type == Token::END_LINE) {
            eat(Token::END_LINE);
            root->children.push_back(statement());
        }
        if (current_token.type == Token::ID) {
            error();
        }
        return root;
    }

    AST* else_statement() {
        if (current_token.type == Token::ELSE) {
            eat(Token::ELSE);
            eat(Token::COLON);
            eat(Token::END_LINE);
            return compound_statement();
        } else {
            return empty();
        }
    }

    AST* if_statement() {
        cout << "$1" << endl;
        eat(Token::IF);
        AST* condition = bool_expr();
        cout << "$9" << endl;
        eat(Token::COLON);
        eat(Token::END_LINE);
        int next_line_indent = stoi(current_token.value);
        cout << "$9.1 " << next_line_indent << " " << indent_level.top() << endl; 
        if (next_line_indent <= indent_level.top())
            error();
        else indent_level.push(next_line_indent);
        cout << "$10" << endl;
        AST* if_body = compound_statement();
        AST* else_body = else_statement();
        return new ConditionalNode(condition, if_body, else_body);
    }

    AST* statement() {
        cout << "$12" << endl;
        parse_indent();
        cout << "$13" << endl;
        if (current_token.type == Token::IF)
            return if_statement();
        if (current_token.type == Token::ID)
            return  assignment_statement();
        return  empty();
    }

    AST* assignment_statement() {
        VariableNode* left = variable();
        Token token = current_token;
        eat(Token::ASSIGN);
        AST* right = bool_expr();
        return new AssignNode(left, token, right);
    }

    VariableNode* variable() {
        VariableNode* node = new VariableNode(current_token);
        eat(Token::ID);
        return node;
    }

    AST* empty() {
        AST* node = new NoOp();
        return node;
    }

    AST* parse() {
        AST* node = compound_statement();
        if (current_token.type != Token::EOF_TOKEN) {
            error();
        }
        return node;
    }
};

#endif