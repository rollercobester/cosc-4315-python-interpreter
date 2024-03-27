#ifndef PARSER_CPP
#define PARSER_CPP

#include <iostream>
#include <stdexcept>
#include "ast.cpp"
#include "scanner.cpp"
#include "token.cpp"

class Parser {
    
  private:
    Scanner scanner;
    Token current_token;

  public:
    Parser(Scanner &_) : scanner(_), current_token(scanner.get_next_token()) {}

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

    AST* factor() {
        Token token = current_token;
        if (token.type == Token::PLUS) {
            eat(Token::PLUS);
            return new UnaryOp(token, factor());
        } else if (token.type == Token::MINUS) {
            eat(Token::MINUS);
            return new UnaryOp(token, factor());
        } else if (token.type == Token::INT) {
            eat(Token::INT);
            return new Num(token);
        } else if (token.type == Token::L_PAREN) {
            eat(Token::L_PAREN);
            AST* node = expr();
            eat(Token::R_PAREN);
            return node;
        } else {
            error();
            return nullptr;
        }
    }

    AST* term() {
        AST* node = factor();
        while (current_token.type == Token::TIMES || current_token.type == Token::DIVIDE) {
            Token operator_token = current_token;
            eat(operator_token.type);
            node = new BinOp(node, operator_token, factor());
        }
        return node;
    }

    AST* expr() {
        AST* node = term();
        while (current_token.type == Token::PLUS || current_token.type == Token::MINUS) {
            Token operator_token = current_token;
            eat(operator_token.type);
            node = new BinOp(node, operator_token, term());
        }
        return node;
    }

    AST* compound_statement() {
        Compound* root = new Compound();
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

    AST* statement() {
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
        AST* left = variable();
        Token token = current_token;
        eat(Token::ASSIGN);
        AST* right = expr();
        return new Assign(left, token, right);
    }

    AST* variable() {
        AST* node = new Variable(current_token);
        eat(Token::ID);
        return node;
    }

    AST* empty() {
        AST* node = new NoOp();
        return node;
    }

    AST* parse() {
        return expr();
    }
};

#endif