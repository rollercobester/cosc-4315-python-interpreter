#ifndef TOKEN_CPP  // Header guard to prevent multiple inclusions
#define TOKEN_CPP

#include <string>

using namespace std;

struct Token {
    enum TokenType { INT, ADD, SUB, MUL, DIV, L_PAREN, R_PAREN,EOF_TOKEN };
    TokenType type;
    string value;

    Token() : type(EOF_TOKEN), value("") {}
    Token(TokenType t, string v) :  type(t), value(v) {}
    Token(const Token& token) : type(token.type), value(token.value) {}
};

#endif