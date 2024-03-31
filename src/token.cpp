#ifndef TOKEN_CPP
#define TOKEN_CPP

#include <string>
#include <unordered_map>

using namespace std;

struct Token {
    enum TokenType {

        // punctuation
        COMMA, DOT, SEMICOLON, COLON, DOUBLE_QUOTE, L_PAREN, R_PAREN, END_LINE, EOF_TOKEN,

        // boolean operators
        EQUALS, NOT_EQUALS, LESS_THAN_EQUALS, GREATER_THAN_EQUALS, LESS_THAN, GREATER_THAN,

        // assignment operators
        PLUS_EQUALS, MINUS_EQUALS, TIMES_EQUALS, DIVIDE_EQUALS, ASSIGN,

        // mathematical operators
        PLUS, MINUS, TIMES, DIVIDE,

        // dynamic value tokens
        BOOL, INT, STRING, ID, FUNCTION, INDENT,

        // keywords
        DEF, IF, ELIF, ELSE, RETURN, NOT, OR, AND
    };
    TokenType type;
    string value;

    Token() : type(EOF_TOKEN), value("") {}
    Token(TokenType t, string v) : type(t), value(v) {}
    Token(const Token& token) : type(token.type), value(token.value) {}
};

const unordered_map<string, Token::TokenType> keywords = {
    {"def",    Token::DEF},
    {"if",     Token::IF},
    {"elif",   Token::ELIF},
    {"else",   Token::ELSE},
    {"return", Token::RETURN},
    {"not",    Token::NOT},
    {"or",     Token::OR},
    {"and",    Token::AND},
    {"print",  Token::FUNCTION},
    {"True",   Token::BOOL},
    {"False",  Token::BOOL},
};

#endif