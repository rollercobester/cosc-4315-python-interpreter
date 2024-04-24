#ifndef SCOPE_TABLE_CPP
#define SCOPE_TABLE_CPP

using namespace std;

#include <iostream>
#include <unordered_map>
#include "ast.cpp"

class Scope {
  private:
    unordered_map<string, AST*> scope;
  public:
    Scope() {}
    void set(string id, AST* value) {
        if (scope.find(id) == scope.end())
            scope.insert({id, value});
        else scope[id] = value;
    }
    AST* get(string id) {
        if (scope.find(id) != scope.end())
            return scope[id];
        return nullptr;
    }
};

class ScopeTable {
  private:
    vector<Scope> scope_table;
  public:
    ScopeTable() { increase_scope(); }
    void increase_scope() {
        scope_table.push_back(Scope());
    }
    void decrease_scope() {
        scope_table.pop_back();
    }
    void set(string id, AST* value) {
        scope_table.back().set(id, value);
    }
    AST* get(string id) {
        for (auto it = scope_table.rbegin(); it != scope_table.rend(); ++it) {
            AST* result = it->get(id);
            if (result != nullptr) return result;
        }
        return nullptr;
    }
};
#endif