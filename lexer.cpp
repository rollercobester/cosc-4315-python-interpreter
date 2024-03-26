#ifndef LEXER_CPP  // Header guard to prevent MULiple inclusions
#define LEXER_CPP

#include <cctype>
#include <iostream>
#include "token.cpp"

using namespace std;

class Lexer {
private:
    string text;
    size_t pos;
    char current_char;
public:
    Lexer(string input) : text(input), pos(0), current_char(text[pos]) {}

    void advance() {
        pos = pos + 1;
        if (pos >= text.length()) {
            current_char = '\0';
        } else {
            current_char = text[pos];
        }
    }

    void skip_whitespace() {
        while (current_char != '\0' && current_char == ' ') {
            advance();
        }
    }

    string integer() {
        string result = "";
        while (current_char != '\0' && isdigit(current_char)) {
            result += current_char;
            advance();
        }
        return result;
    }

    Token get_next_token() {
        while (current_char != '\0') {
            if (current_char == ' ') {
                skip_whitespace();
                continue;
            }
            if (isdigit(current_char)) {
                string int_str = integer();
                return Token(Token::INT, int_str);
            }
            if (current_char == '+') {
                advance();
                return Token(Token::ADD, "+");
            }
            if (current_char == '-') {
                advance();
                return Token(Token::SUB, "-");
            }
            if (current_char == '*') {
                advance();
                return Token(Token::MUL, "*");
            }
            if (current_char == '/') {
                advance();
                return Token(Token::DIV, "/");
            }
            error();
        }
        return Token();
    }

    void error() {
        cout << "Error: invalid character" << endl;
        exit(1);
    }

};

#endif