#ifndef VAR_CPP
#define VAR_CPP

#include <string>

using namespace std;

class Var {
  public:
    virtual ~Var() {}
};

class VarBool : public Var {
  public:
    bool value;
    VarBool(bool b) : value(b) {}
};

class VarInt : public Var {
  public:
    int value;
    VarInt(int v) : value(v) {}
};

class VarString : public Var {
  public:
    string text;
    VarString(string s) : text(s) {}
};

#endif