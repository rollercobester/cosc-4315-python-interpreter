

#include <iostream>
#include <string>
#include <cctype>

using namespace std;

// Token structure
struct Token {
    enum TokenType { INT, PLUS, MINUS, MULT, DIV, EOF_TOKEN };
    TokenType type;
    char value;

    Token(TokenType t, char v) :  type(t), value(v) {}
    Token(const Token& token) : type(token.type), value(token.value) {}
};

class Interpreter {
private:
    string text;
    size_t pos;
    Token current_token;
    char current_char;
public:
    Interpreter(string input) : current_token(get_next_token()) {
        text = input;
        pos = 0;
        current_char = text[pos];
        cout << text << endl;
    }

    void error(string error_message) {
        cout << error_message << endl;
        exit(1);
    }

    void advance() {
        pos++;
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

    int integer() {
        int result = 0;
        while (current_char != '\0' && isdigit(current_char)) {
            result = result * 10 + (current_char - '0');
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
                return Token(Token::INT, integer());
            }

            if (current_char == '+') {
                advance();
                return Token(Token::PLUS, '+');
            }

            if (current_char == '-') {
                advance();
                return Token(Token::MINUS, '-');
            }

            error("Error: Invalid character encountered");
        }

<<<<<<< HEAD
        return Token(Token::EOF_TOKEN, '\0');
=======
        char current_char = text[pos];

        if (isdigit(current_char)) {
            Token token(Token::INT, current_char);
            ++pos;
            return token;
        } else if (current_char == '+') {
            Token token(Token::PLUS, current_char);
            ++pos;
            return token;
        } else if (current_char == '-') {
            Token token(Token::MINUS, current_char);
            ++pos;
            return token;
        } else if (current_char == '*') {
            Token token(Token::MULT, current_char);
            ++pos;
            return token;
        } else if (current_char == '/') {
            Token token(Token::DIV, current_char);
            ++pos;
            return token;
        }
        else {
            cout << "Error: Invalid character encountered: " << current_char << endl;
            exit(1);
        }
>>>>>>> 14d47021b2c4f9869063e284462358634752b741
    }

    void eat(Token::TokenType type) {
        if (static_cast<int>(current_token.type) == static_cast<int>(type)) {
            current_token = get_next_token();
        } else {
            error("Error: Unexpected token type encountered");
        }
    }

    int expr() {

        current_token = get_next_token();
        
        Token left = current_token;
        eat(Token::INT);

        Token op = current_token;
        if (op.type == Token::PLUS)
            eat(Token::PLUS);
        else if (op.type == Token::MINUS)
            eat(Token::MINUS);
        else if (op.type == Token::MULT)
            eat(Token::MULT);
        else
            eat(Token::DIV);
        
        Token right = current_token;
        eat(Token::INT);

        int int1 = left.value - '0';
        int int2 = right.value - '0';


        if (op.type == Token::PLUS)
            return int1 + int2;
        else if (op.type == Token::MINUS)
            return int1 - int2;
        else if (op.type == Token::MULT)
            return int1 * int2;
        else
            return int1 / int2;
    }

    void ignoreWhiteSpace() {
        
    }
};

int main() {
<<<<<<< HEAD
    Interpreter i("92-44");
=======
    Interpreter i("9/4");
>>>>>>> 14d47021b2c4f9869063e284462358634752b741
    int result = i.expr();
    cout << "Result: " << result << endl;
}