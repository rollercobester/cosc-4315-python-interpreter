#ifndef TOKEN_CPP
#define TOKEN_CPP

#include <string>
#include <unordered_map>

using namespace std;

struct Token {
    enum TokenType { 
        // boolean operators
        EQUALS, NOT_EQUALS, LESS_THAN_EQUALS, GREATER_THAN_EQUALS, LESS_THAN, GREATER_THAN, EXCLAMATION,

        // assignment operators
        PLUS_EQUALS, MINUS_EQUALS, TIMES_EQUALS, DIVIDE_EQUALS, ASSIGN,

        // mathematical operators
        PLUS, MINUS, TIMES, DIVIDE,

        // punctuation
        COMMA, DOT, SEMICOLON, COLON, L_PAREN, R_PAREN, END_LINE, EOF_TOKEN,

        // variables
        INT, ID,

        // keywords
        DEF, IF, ELIF, ELSE, RETURN, NOT
    };
    TokenType type;
    string value;

    Token() : type(EOF_TOKEN), value("") {}
    Token(TokenType t, string v) : type(t), value(v) {}
    Token(const Token& token) : type(token.type), value(token.value) {}
};

const unordered_map<string, Token::TokenType> keywords = {
    {"def", Token::DEF},
    {"if", Token::TokenType::IF},
    {"elif", Token::TokenType::ELIF},
    {"else", Token::TokenType::ELSE},
    {"return", Token::TokenType::RETURN}
};

#endif