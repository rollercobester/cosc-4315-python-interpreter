#ifndef SCOPE_CPP
#define SCOPE_CPP

using namespace std;

#include <iostream>
#include <unordered_map>
#include "ast.cpp"

class Scope {
  private:
    unordered_map<string, AST*> scope;
    Scope* parent;
  public:
    Scope(Scope* node = nullptr) : parent(node) {}
    void set(string id, AST* value) {
        if (scope.find(id) == scope.end())
            scope.insert({id, value});
        else scope[id] = value;
    }
    AST* get(string id) {
        if (scope.find(id) != scope.end()) 
            return scope[id];
        return parent != nullptr ? parent->get(id) : nullptr;
    }
    AST* get_local(string id) {
        return scope.find(id) != scope.end() ? scope[id] : nullptr;
    }
    Scope* get_parent() {
        return parent;
    }
};

#endif