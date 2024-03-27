#ifndef SCANNER_CPP
#define SCANNER_CPP

#include <cctype>
#include <iostream>
#include <stdexcept>
#include "token.cpp"

using namespace std;

class Scanner {

  private:
    string text;
    size_t pos;
    char current_char;

  public:
    Scanner(string input) : text(input), pos(0), current_char(text[pos]) {}

    void error() {
        throw runtime_error("Invalid character");
    }

    void advance() {
        pos = pos + 1;
        if (pos >= text.length()) {
            current_char = '\0';
        } else {
            current_char = text[pos];
        }
    }

    char peek() {
        if (pos >= text.length() - 1) {
            return '\0';
        } else {
            return text[pos + 1];
        }
    }

    void skip_whitespace() {
        while (current_char != '\0' && current_char == ' ') {
            advance();
        }
    }

    Token integer() {
        string result = "";
        while (current_char != '\0' && isdigit(current_char)) {
            result += current_char;
            advance();
        }
        return Token(Token::INT, result);
    }

    Token id() {
        string result = "";
        while (current_char != '\0' && isalnum(current_char)) {
            result += current_char;
            advance();
        }
        return Token(keywords.at(result), result);
    }

    Token get_next_token() {
        while (current_char != '\0') {
            if (current_char == ' ') {
                skip_whitespace();
                continue;
            } else if (isdigit(current_char)) {
                return integer();
            } else if (current_char == '=' && peek() == '=') {
                advance();
                advance();
                return Token(Token::EQUALS, "==");
            } else if (current_char == '!' && peek() == '=') {
                advance();
                advance();
                return Token(Token::NOT_EQUALS, "!=");
            } else if (current_char == '<' && peek() == '=') {
                advance();
                advance();
                return Token(Token::LESS_THAN_EQUALS, "<=");
            } else if (current_char == '>' && peek() == '=') {
                advance();
                advance();
                return Token(Token::GREATER_THAN_EQUALS, ">=");
            } else if (current_char == '+' && peek() == '=') {
                advance();
                advance();
                return Token(Token::PLUS_EQUALS, "+=");
            } else if (current_char == '-' && peek() == '=') {
                advance();
                advance();
                return Token(Token::MINUS_EQUALS, "-=");
            } else if (current_char == '*' && peek() == '=') {
                advance();
                advance();
                return Token(Token::TIMES_EQUALS, "*=");
            } else if (current_char == '/' && peek() == '=') {
                advance();
                advance();
                return Token(Token::DIVIDE_EQUALS, "/=");
            } else if (current_char == '=') {
                advance();
                return Token(Token::ASSIGN, "=");
            } else if (current_char == '!') {
                advance();
                return Token(Token::EXCLAMATION, "!");
            } else if (current_char == '<') {
                advance();
                return Token(Token::LESS_THAN, "<");
            } else if (current_char == '>') {
                advance();
                return Token(Token::GREATER_THAN, ">");
            } else if (current_char == '+') {
                advance();
                return Token(Token::PLUS, "+");
            } else if (current_char == '-') {
                advance();
                return Token(Token::MINUS, "-");
            } else if (current_char == '*') {
                advance();
                return Token(Token::TIMES, "*");
            } else if (current_char == '/') {
                advance();
                return Token(Token::DIVIDE, "/");
            } else if (current_char == '(') {
                advance();
                return Token(Token::L_PAREN, "(");
            } else if (current_char == ')') {
                advance();
                return Token(Token::R_PAREN, ")");
            }
            error();
        }
        return Token();
    }
};

#endif