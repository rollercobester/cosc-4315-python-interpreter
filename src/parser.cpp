#ifndef PARSER_CPP
#define PARSER_CPP

#include <iostream>
#include <stack>
#include <stdexcept>
#include "ast.cpp"
#include "scanner.cpp"
#include "token.cpp"

class Parser {
    
  private:
    Scanner scanner;
    Token current_token;
    stack<int> indent_level;
    int debug_depth = 0;


  public:
    Parser(Scanner &_) : scanner(_), current_token(scanner.get_next_token()) {
        indent_level.push(0);
    }

    void error() {
        throw runtime_error("Invalid syntax");
    }

    void eat(Token::TokenType type) {
        if (current_token.type == type) {
            current_token = scanner.get_next_token();
        } else {
            error();
        }
    }

    void parse_indent() {
        if (current_token.type == Token::INDENT) {
            int n = stoi(current_token.value);
            if (n >  indent_level.top()) indent(n);
            if (n <  indent_level.top()) unindent(n);
            if (n == indent_level.top()) return;
        }
        error();
    }

    void unindent(int n) {
        while (n < indent_level.top()) indent_level.pop();
    }

    void indent(int n) {
        indent_level.push(n);
    }

    AST* factor() {
        debugPrint("<factor>");
        Token token = current_token;
        AST* node;
        if (token.type == Token::EXCLAMATION) {
            eat(token.type);
            node = new UnaryOpNode(token, math_expr());
        } else if (token.type == Token::PLUS) {
            eat(Token::PLUS);
            node = new UnaryOpNode(token, factor());
        } else if (token.type == Token::MINUS) {
            eat(Token::MINUS);
            node = new UnaryOpNode(token, factor());
        } else if (token.type == Token::BOOL) {
            eat(Token::BOOL);
            node = new BoolNode(token);
        } else if (token.type == Token::INT) {
            eat(Token::INT);
            node = new IntNode(token);
        } else if (token.type == Token::L_PAREN) {
            eat(Token::L_PAREN);
            node = math_expr();
            eat(Token::R_PAREN);
        } else {
            eat(Token::ID);
            node = new VariableNode(token);
        }
        debugPrint("</factor>");
        return node;
    }

    AST* term() {
        debugPrint("<term>");
        AST* node = factor();
        while (current_token.type == Token::TIMES || current_token.type == Token::DIVIDE) {
            Token operator_token = current_token;
            eat(operator_token.type);
            node = new BinOpNode(node, operator_token, factor());
        }
        debugPrint("</term>");
        return node;
    }

    AST* math_expr() {
        debugPrint("<math_expr>");
        AST* node = term();
        while (current_token.type == Token::PLUS || current_token.type == Token::MINUS) {
            Token operator_token = current_token;
            eat(operator_token.type);
            node = new BinOpNode(node, operator_token, term());
        }
        debugPrint("</math_expr>");
        return node;
    }

    AST* expr() {
        debugPrint("<expr>");
        AST* node = math_expr();
        //AST* node = expr();?
        while (current_token.type == Token::EQUALS
            || current_token.type == Token::NOT_EQUALS
            || current_token.type == Token::LESS_THAN
            || current_token.type == Token::GREATER_THAN 
            || current_token.type == Token::LESS_THAN_EQUALS
            || current_token.type == Token::GREATER_THAN_EQUALS) {
            
            Token operator_token = current_token;
            eat(operator_token.type);
            //node = new BinOpNode(node, operator_token, expr());?
            node = new BinOpNode(node, operator_token, math_expr());
        }
        debugPrint("</expr>");
        return node;
    }

    AST* logic_expr() {
        debugPrint("<logic_expr>");
        AST* node = expr();
        //AST* node = expr();?
        while (current_token.type == Token::NOT
            || current_token.type == Token::AND 
            || current_token.type == Token::OR) {
            
            Token operator_token = current_token;
            eat(operator_token.type);
            if (current_token.type == Token::NOT)
                node = new UnaryOpNode(operator_token, node);
            else
                node = new BinOpNode(node, operator_token, expr());
        }
        debugPrint("</logic_expr>");
        return node;
    }

    AST* block() {
        debugPrint("<block>");
        
        BlockNode* node = new BlockNode();
        parse_indent();
        int block_indent = indent_level.top();
        eat(Token::INDENT);
        node->children.push_back(statement());
        eat(Token::END_LINE);
        while (current_token.type != Token::END_LINE && current_token.type != Token::EOF_TOKEN) {
            parse_indent();
            if (indent_level.top() == block_indent) {
                eat(Token::INDENT);
                node->children.push_back(statement());
                if (current_token.type == Token::END_LINE)
                    eat(Token::END_LINE);
            } else break;
        }
        debugPrint("</block>");
        return node;
    }

    AST* else_statement() {
        debugPrint("<else>");
        AST* node;
        eat(Token::ELSE);
        eat(Token::COLON);
        eat(Token::END_LINE);
        node = block();
        debugPrint("</else>");
        return node;
    }

    AST* if_statement() {
        debugPrint("<if>");
        eat(Token::IF);
        AST* condition = logic_expr();
        eat(Token::COLON);
        eat(Token::END_LINE);
        AST* if_body = block();
        parse_indent();
        eat(Token::INDENT);
        AST* else_body;
        if (current_token.type == Token::ELSE)
            else_body = else_statement();
        else
            else_body = new NoOp();
        //NoOp* else_body = new NoOp();
        debugPrint("</if>");
        return new ConditionalNode(condition, if_body, else_body);
    }

    AST* statement() {
        debugPrint("<statement>");
        AST* node;
        if (current_token.type == Token::IF)
            node = if_statement();
        else if (current_token.type == Token::ID)
            node = assignment_statement();
        else if (current_token.type == Token::END_LINE)
            node = empty();
        debugPrint("</statement>");
        return node;
    }

    AST* assignment_statement() {
        debugPrint("<assign>");
        VariableNode* left = variable();
        Token token = current_token;
        eat(Token::ASSIGN);
        AST* right = logic_expr();
        debugPrint("</assign>");
        return new AssignNode(left, token, right);
    }

    VariableNode* variable() {
        debugPrint("<var>");
        VariableNode* node = new VariableNode(current_token);
        eat(Token::ID);
        debugPrint("</var>");
        return node;
    }

    AST* empty() {
        debugPrint("<empty>");
        AST* node = new NoOp();
        debugPrint("</empty>");
        return node;
    }

    AST* program() {
        debugPrint("<program>");
        AST* node = block();
        if (current_token.type != Token::EOF_TOKEN) {
            error();
        }
        debugPrint("</program>");
        return node;
    }

    void debugPrint(string text) {
        return;
        if (text[1] == '/') debug_depth--;
        for (int i = 0; i < debug_depth; i++) {
            cout << " ";
        }
        if (text[1] != '/') debug_depth++;
        cout << text << endl;
    }
};

#endif