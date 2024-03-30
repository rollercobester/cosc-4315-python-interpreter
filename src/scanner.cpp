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
    int pos;
    bool next_token_is_indent = true;
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

    char peek(int n = 1) {
        if (pos + n >= text.length()) {
            return '\0';
        } else {
            return text[pos + n];
        }
    }

    char peek_behind(int n = 1) {
        if (pos - n < 0) {
            return '\0';
        } else {
            return text[pos - n];
        }
    }

    Token skip_indent() {
        int indent = 0;
        while (current_char == ' ') {
            advance();
            indent++;
        }
        //cout << indent <cout< endl;
        return Token(Token::INDENT, to_string(indent));
    }

    void skip_whitespace() {
        while (current_char != '\0' && current_char == ' ') {
            advance();
        }
    }

    void skip_comment() {
        while (current_char != '\0' && current_char != '\n') {
            advance();
        }
    }

    void skip_multiline_comment() {
        advance();
        advance();
        advance();
        while (current_char != '\0') {
            if (current_char == '\"' && peek() == '\"' && peek(2) == '\"') {
                advance();
                advance();
                advance();
                break;
            }
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
        if (keywords.find(result) != keywords.end()) {
            return Token(keywords.at(result), result);
        } else {
            return Token(Token::ID, result);
        }
    }

    Token get_next_token() {
        while (current_char != '\0') {
            cout << current_char << endl;
            if (next_token_is_indent) {
                next_token_is_indent = false;
                return skip_indent();
            } else if (current_char == '\n') {
                next_token_is_indent = true;
                advance();
                return Token(Token::END_LINE, "\n");
            } else if (current_char == ' ') {
                skip_whitespace();
                continue;
            } else if (current_char == '"' && peek() == '"' && peek(2) == '"') {
                skip_multiline_comment();
                continue;
            } else if (current_char == '#') {
                skip_comment();
                continue;
            } else if (isdigit(current_char)) {
                return integer();
            } else if (isalpha(current_char)) {
                return id();
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
            } else if (current_char == ':') {
                advance();
                return Token(Token::COLON, ":");
            }
            error();
        }
        return Token();
    }
};

#endif