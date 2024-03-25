

#include <iostream>
#include <string>
#include <cctype>

using namespace std;

// Token structure
struct Token {
    enum TokenType { INT, PLUS, MINUS, EOF_TOKEN };
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
public:
    Interpreter(string input) : text(input), pos(0), current_token(get_next_token()) { 
        cout << text << endl;
    }

    void error(string error_message) {
        cout << error_message << endl;
        exit(1);
    }

    Token get_next_token() {

        if (pos >= text.length()) {
            return Token(Token::EOF_TOKEN, '0');
        }

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
        } else {
            cout << "Error: Invalid character encountered: " << current_char << endl;
            exit(1);
        }
    }

    void eat(Token::TokenType type) {
        if (static_cast<int>(current_token.type) == static_cast<int>(type)) {
            current_token = get_next_token();
        } else {
            cout << "Error: Unexpected token type encountered: " << current_token.type << endl;
            exit(1);
        }
    }

    int expr() {

        Token left = current_token;
        eat(Token::INT);

        Token op = current_token;
        if (op.type == Token::PLUS)
            eat(Token::PLUS);
        else
            eat(Token::MINUS);
        
        Token right = current_token;
        eat(Token::INT);

        int int1 = left.value - '0';
        int int2 = right.value - '0';


        if (op.type == Token::PLUS)
            return int1 + int2;
        else
            return int1 - int2;
    }
};

int main() {
    Interpreter i("9-4");
    int result = i.expr();
    cout << "Result: " << result << endl;
}