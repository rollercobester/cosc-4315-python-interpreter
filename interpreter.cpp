

#include <iostream>
#include <string>
#include <cctype>

using namespace std;

// Token structure
struct Token {
    enum TokenType { INT, PLUS, MINUS, MULT, DIV, EOF_TOKEN };
    TokenType type;
    string value;

    Token(TokenType t, string v) :  type(t), value(v) {}
    Token(const Token& token) : type(token.type), value(token.value) {}
};

class Interpreter {
private:
    string text;
    size_t pos;
    Token current_token;
    char current_char;
public:
    Interpreter(string input) : current_token(Token(Token::EOF_TOKEN, "")) {
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
                return Token(Token::INT, integer());
            }

            if (current_char == '+') {
                advance();
                return Token(Token::PLUS, "+");
            }

            if (current_char == '-') {
                advance();
                return Token(Token::MINUS, "-");
            }

            if (current_char == '*') {
                advance();
                return Token(Token::MULT, "*");
            }

            if (current_char == '/') {
                advance();
                return Token(Token::DIV, "/");
            }

            error("Error: Invalid character encountered");
        }

        return Token(Token::EOF_TOKEN, "");
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

        int int1 = stoi(left.value);
        int int2 = stoi(right.value);

        if (op.type == Token::PLUS)
            return int1 + int2;
        else if (op.type == Token::MINUS)
            return int1 - int2;
        else if (op.type == Token::MULT)
            return int1 * int2;
        else
            return int1 / int2;
    }
};

int main() {
    Interpreter i("1-2");
    int result = i.expr();
    cout << "Result: " << result << endl;
}