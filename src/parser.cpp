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
    bool DEBUG_MODE = false;
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
        if (token.type == Token::NOT) {
            eat(Token::NOT);
            node = new UnaryOpNode(token, factor());
        } else if (token.type == Token::PLUS) {
            eat(Token::PLUS);
            node = new UnaryOpNode(token, factor());
        } else if (token.type == Token::MINUS) {
            eat(Token::MINUS);
            node = new UnaryOpNode(token, factor());
        } else if (token.type == Token::STRING) {
            eat(Token::STRING);
            node = new StringNode(token.value);
        } else if (token.type == Token::BOOL) {
            eat(Token::BOOL);
            node = new BoolNode(token.value == "True");
        } else if (token.type == Token::INT) {
            eat(Token::INT);
            node = new IntNode(stoi(token.value));
        } else if (token.type == Token::L_PAREN) {
            eat(Token::L_PAREN);
            node = logic_expr();
            eat(Token::R_PAREN);
        } else if (token.type == Token::FUNCTION_ID) {
            node = function_call();
            //eat(Token::FUNCTION_ID);
            //node = new FunctionCallNode(token.value);
        } else {
            eat(Token::VARIABLE_ID);
            node = new VariableNode(token.value);
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
            node = new BinaryOpNode(node, operator_token, factor());
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
            node = new BinaryOpNode(node, operator_token, term());
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
            //node = new BinaryOpNode(node, operator_token, expr());?
            node = new BinaryOpNode(node, operator_token, math_expr());
        }
        debugPrint("</expr>");
        return node;
    }

    AST* logic_expr() {
        debugPrint("<logic_expr>");
        AST* node = expr();
        //AST* node = expr();?
        while (current_token.type == Token::AND || current_token.type == Token::OR) {
            Token operator_token = current_token;
            eat(operator_token.type);
            node = new BinaryOpNode(node, operator_token, expr());
        }
        debugPrint("</logic_expr>");
        return node;
    }

    BlockNode* block() {
        debugPrint("<block>");
        BlockNode* node = new BlockNode();
        if (current_token.type == Token::INDENT) {
            parse_indent();
            eat(Token::INDENT);
        }
        int block_indent = indent_level.top();
        node->children.push_back(statement());
        if (current_token.type == Token::END_LINE)
            eat(Token::END_LINE);
        while (current_token.type != Token::END_LINE && current_token.type != Token::EOF_TOKEN) {
            if (current_token.type == Token::INDENT) {
                parse_indent();
                if (indent_level.top() == block_indent)
                    eat(Token::INDENT);
            }
            if (indent_level.top() == block_indent) {
                //eat(Token::INDENT);
                node->children.push_back(statement());
                if (current_token.type == Token::END_LINE)
                    eat(Token::END_LINE);
            } else break;
        }
        debugPrint("</block>");
        return node;
    }

    AST* function_definition() {
        debugPrint("<def>");
        eat(Token::DEF);
        FunctionNode* function = new FunctionNode(current_token.value);
        eat(Token::FUNCTION_ID);
        eat(Token::L_PAREN);
        if (current_token.type != Token::R_PAREN) {
            function->parameters.push_back(current_token.value);
            eat(Token::VARIABLE_ID);
        }
        while(current_token.type == Token::COMMA) {
            eat(Token::COMMA);
            function->parameters.push_back(current_token.value);
            eat(Token::VARIABLE_ID);
        }
        eat(Token::R_PAREN);
        eat(Token::COLON);
        eat(Token::END_LINE);
        function->function_body = block();
        debugPrint("</def>");
        return function;
    }

    AST* function_call() {
        debugPrint("<function>");
        FunctionCallNode* node = new FunctionCallNode(current_token.value);
        eat(Token::FUNCTION_ID);
        eat(Token::L_PAREN);
        if (current_token.type != Token::R_PAREN)
            node->parameters.push_back(logic_expr());
        while (current_token.type == Token::COMMA) {
            eat(Token::COMMA);
            node->parameters.push_back(logic_expr());
        }
        eat(Token::R_PAREN);
        debugPrint("</function>");
        return node;
    }

    AST* else_statement(int if_indent) {
        debugPrint("<else>");
        int else_indent = indent_level.top();
        AST* else_body;
        if (if_indent == else_indent) {
            eat(Token::INDENT);
        }
        if (current_token.type == Token::ELSE) {
            eat(Token::ELSE);
            eat(Token::COLON);
            eat(Token::END_LINE);
            else_body = block();
        } else else_body = empty();
        debugPrint("</else>");
        return else_body;
    }

    AST* if_statement() {
        debugPrint("<if>");
        int if_indent = indent_level.top();
        eat(Token::IF);
        AST* condition = logic_expr();
        eat(Token::COLON);
        eat(Token::END_LINE);
        AST* if_body = block();
        debugPrint("</if>");
        AST* else_body = else_statement(if_indent);
        return new ConditionalNode(condition, if_body, else_body);
    }

    AST* return_statement() {
        debugPrint("<return>");
        eat(Token::RETURN);
        AST* value = (current_token.type == Token::END_LINE ? empty() : logic_expr());
        eat(Token::END_LINE);
        return new ReturnNode(value);
        debugPrint("</return>");
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
    
    AST* statement() {
        debugPrint("<statement>");
        AST* node;
        if (current_token.type == Token::IF)               node = if_statement();
        else if (current_token.type == Token::DEF)         node = function_definition();
        else if (current_token.type == Token::RETURN)      node = return_statement();
        else if (current_token.type == Token::VARIABLE_ID) node = assignment_statement();
        else if (current_token.type == Token::FUNCTION_ID) {
            node = function_call();
            eat(Token::END_LINE);
        } else node = empty();
        debugPrint("</statement>");
        return node;
    }

    VariableNode* variable() {
        debugPrint("<var>");
        VariableNode* node = new VariableNode(current_token.value);
        eat(Token::VARIABLE_ID);
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
        if (!DEBUG_MODE) return;
        if (text[1] == '/') debug_depth--;
        for (int i = 0; i < debug_depth; i++) {
            cout << " ";
        }
        if (text[1] != '/') debug_depth++;
        cout << text << endl;
    }
};

#endif