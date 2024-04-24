#ifndef SCOPE_TABLE_CPP
#define SCOPE_TABLE_CPP

using namespace std;

#include <iostream>
#include <unordered_map>
#include "ast.cpp"

class ScopeTree {
  private:
    unordered_map<string, AST*> scope;
    ScopeTree* parent;
  public:
    ScopeTree(ScopeTree* node = nullptr) : parent(node) {}
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
    ScopeTree* increase_scope() {
        return new ScopeTree(this);
    }
    ScopeTree* decrease_scope() {
        return parent;
    }
};

#endif