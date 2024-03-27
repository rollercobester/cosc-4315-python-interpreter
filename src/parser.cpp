#ifndef PARSER_CPP
#define PARSER_CPP

#include <iostream>
#include <stdexcept>
#include "ast.cpp"
#include "lexer.cpp"
#include "token.cpp"

class Parser {
    
  private:
    Lexer lexer;
    Token current_token;

  public:
    Parser(Lexer &_) : lexer(_), current_token(lexer.get_next_token()) {}

    void error() {
        throw runtime_error("Invalid syntax");
    }

    void eat(Token::TokenType type) {
        if (current_token.type == type) {
            current_token = lexer.get_next_token();
        } else {
            error();
        }
    }

    AST* factor() {
        Token token = current_token;
        if (token.type == Token::INT) {
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
        while (current_token.type == Token::MUL || current_token.type == Token::DIV) {
            Token operator_token = current_token;
            if (operator_token.type == Token::MUL) {
                eat(Token::MUL);
            } else {
                eat(Token::DIV);
            }
            node = new BinOp(node, operator_token, factor());
        }
        return node;
    }

    AST* expr() {
        AST* node = term();
        while (current_token.type == Token::ADD || current_token.type == Token::SUB) {
            Token operator_token = current_token;
            if (operator_token.type == Token::ADD) {
                eat(Token::ADD);
            } else {
                eat(Token::SUB);
            }
            node = new BinOp(node, operator_token, term());
        }
        return node;
    }

    AST* parse() {
        return expr();
    }
};

#endif