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

