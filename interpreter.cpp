#ifndef INTERPRETER_CPP
#define INTERPRETER_CPP

#include <iostream>
#include "lexer.cpp"
#include "token.cpp"

using namespace std;

class Interpreter {
private:
    Lexer lexer;
    Token current_token;
public:
    Interpreter(Lexer &_) : lexer(_), current_token(lexer.get_next_token()) {}

    void eat(Token::TokenType type) {
        if (current_token.type == type) {
            current_token = lexer.get_next_token();
        } else {
            error();
        }
    }

    int factor() {
        Token token = current_token;
        eat(Token::INT);
        return stoi(token.value);
    }

    int term() {
        int result = factor();
        while (current_token.type == Token::MUL || current_token.type == Token::DIV) {
            Token operator_token = current_token;
            if (operator_token.type == Token::MUL) {
                eat(Token::MUL);
                result = result * factor();
            } else {
                eat(Token::DIV);
                result = result / factor();
            }
        }
        return result;
    }

    int expr() {
        int result = term();
        while (current_token.type == Token::ADD || current_token.type == Token::SUB) {
            Token operator_token = current_token;
            if (operator_token.type == Token::ADD) {
                eat(Token::ADD);
                result += term();
            } else {
                eat(Token::SUB);
                result -= term();
            }
        }
        return result;
    }

    void error() {
        cout << "Error: invalid syntax" << endl;
        exit(1);
    }
};

#endif

int main() {
    Lexer lexer("1*2+3*4");
    Interpreter interpreter(lexer);
    int result = interpreter.expr();
    cout << "Result: " << result << endl;
    return 0;
}