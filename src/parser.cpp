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
        //exit(1);
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
            int prev_indent = indent_level.top();
            int indent = stoi(current_token.value);
            if (indent == prev_indent) {
                eat(Token::INDENT);
                return;
            }
        }
        error();
    }

    AST* factor() {
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
            eat(Token::INT);
            return new IntNode(token);
        } else if (token.type == Token::L_PAREN) {
            eat(Token::L_PAREN);
            AST* node = expr();
            eat(Token::R_PAREN);
            return node;
        } else {
            eat(Token::ID);
            return new VariableNode(token);
        }
    }

    AST* term() {
        AST* node = factor();
        while (current_token.type == Token::TIMES || current_token.type == Token::DIVIDE) {
            Token operator_token = current_token;
            eat(operator_token.type);
            node = new BinOpNode(node, operator_token, factor());
        }
        return node;
    }

    AST* expr() {
        AST* node = term();
        while (current_token.type == Token::PLUS || current_token.type == Token::MINUS) {
            Token operator_token = current_token;
            eat(operator_token.type);
            node = new BinOpNode(node, operator_token, term());
        }
        return node;
    }

    AST* bool_expr() {
        AST* node = expr();
        while (current_token.type == Token::EQUALS
            || current_token.type == Token::NOT_EQUALS
            || current_token.type == Token::LESS_THAN
            || current_token.type == Token::GREATER_THAN 
            || current_token.type == Token::LESS_THAN_EQUALS
            || current_token.type == Token::GREATER_THAN_EQUALS) {
                
            Token operator_token = current_token;
            eat(operator_token.type);
            node = new BinOpNode(node, operator_token, expr());
        }
        return node;
    }

    AST* compound_statement() {
        CompoundNode* root = new CompoundNode();
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
        eat(Token::ELSE);
        eat(Token::COLON);
        eat(Token::END_LINE);
        return compound_statement();
    }

    AST* if_statement() {
        AST* condition = bool_expr();
        eat(Token::IF);
        eat(Token::COLON);
        eat(Token::END_LINE);
        AST* if_body = compound_statement();
        AST* else_body = else_statement();
        return new ConditionalNode(condition, if_body, else_body);
    }

    AST* statement() {
        //parse_indent();
        AST* node;
        if (current_token.type == Token::DEF) {
            node = compound_statement();
        } else if (current_token.type == Token::ID) {
            node = assignment_statement();
        } else {
            node = empty();
        }
        return node;
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