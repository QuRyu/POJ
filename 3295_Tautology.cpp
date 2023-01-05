#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

class Operator {
public:
  virtual bool evaluate(const unordered_map<string, bool> &env) const = 0;
  virtual ~Operator() {}
  virtual ostream &format(ostream &os, const Operator &op) const = 0;

  friend ostream &operator<<(ostream &os, const Operator &op) {
    return op.format(os, op);
  }
};

class And : public Operator {
  unique_ptr<Operator> x;
  unique_ptr<Operator> y;

public:
  And(unique_ptr<Operator> &&x, unique_ptr<Operator> &&y)
      : x(move(x)), y(move(y)) {}

  bool evaluate(const unordered_map<string, bool> &env) const {
    return x->evaluate(env) and y->evaluate(env);
  }

  ostream &format(ostream &os, const Operator &a) const {
    return os << "K" << *(this->x) << *(this->y);
  }
};

class Or : public Operator {
  unique_ptr<Operator> x;
  unique_ptr<Operator> y;

public:
  Or(unique_ptr<Operator> &&x, unique_ptr<Operator> &&y)
      : x(move(x)), y(move(y)) {}

  bool evaluate(const unordered_map<string, bool> &env) const {
    return x->evaluate(env) or y->evaluate(env);
  }

  ostream &format(ostream &os, const Operator &a) const {
    return os << "A" << *(this->x) << *(this->y);
  }
};

class Implies : public Operator {
  unique_ptr<Operator> x;
  unique_ptr<Operator> y;

public:
  Implies(unique_ptr<Operator> &&x, unique_ptr<Operator> &&y)
      : x(move(x)), y(move(y)) {}

  bool evaluate(const unordered_map<string, bool> &env) const {
    bool x_eval = x->evaluate(env);
    bool y_eval = y->evaluate(env);

    return (not x_eval or (x_eval and y_eval));
  }

  ostream &format(ostream &os, const Operator &a) const {
    return os << "C" << *(this->x) << *(this->y);
  }
};

class Not : public Operator {
  unique_ptr<Operator> x;

public:
  Not(unique_ptr<Operator> &&x) : x(move(x)) {}

  bool evaluate(const unordered_map<string, bool> &env) const {
    return not x->evaluate(env);
  }

  ostream &format(ostream &os, const Operator &a) const {
    return os << "N" << *(this->x);
  }
};

class Equals : public Operator {
  unique_ptr<Operator> x;
  unique_ptr<Operator> y;

public:
  Equals(unique_ptr<Operator> &&x, unique_ptr<Operator> &&y)
      : x(move(x)), y(move(y)) {}

  bool evaluate(const unordered_map<string, bool> &env) const {
    bool x_eval = x->evaluate(env);
    bool y_eval = y->evaluate(env);

    return not(x_eval xor y_eval);
  }

  ostream &format(ostream &os, const Operator &a) const {
    return os << "E" << *(this->x) << *(this->y);
  }
};

class Var : public Operator {
  string x;

public:
  Var(string x) : x(x) {}

  bool evaluate(const unordered_map<string, bool> &env) const {
    return env.find(x)->second;
  }

  ostream &format(ostream &os, const Operator &a) const {
    return os << this->x;
  }
};

struct Ret {
  unique_ptr<Operator> op;
  unordered_set<string> vars;
  int i;

  Ret(unique_ptr<Operator> op, unordered_set<string> vars)
      : op(move(op)), vars(vars), i(0) {}
  Ret(unique_ptr<Operator> op, unordered_set<string> vars, int i)
      : op(move(op)), vars(vars), i(i) {}
  Ret() {}
};

Ret parse(const string &input, int i) {
  assert(i < input.size());
  switch (input[i]) {
  case 'A': {
    Ret ret1 = parse(input, i + 1);
    Ret ret2 = parse(input, ret1.i);

    ret1.vars.insert(ret2.vars.begin(), ret2.vars.end());
    Ret ret(make_unique<Or>(move(ret1.op), move(ret2.op)), ret1.vars,
            ret2.i + 1);
    return ret;
  }

  case 'K': {

    Ret ret1 = parse(input, i + 1);
    Ret ret2 = parse(input, ret1.i);

    ret1.vars.insert(ret2.vars.begin(), ret2.vars.end());
    Ret ret(make_unique<And>(move(ret1.op), move(ret2.op)), ret1.vars,
            ret2.i + 1);
    return ret;
  }

  case 'N': {
    Ret ret1 = parse(input, i + 1);

    Ret ret(make_unique<Not>(move(ret1.op)), ret1.vars, ret1.i + 1);
    return ret;
  }

  case 'C': {
    Ret ret1 = parse(input, i + 1);
    Ret ret2 = parse(input, ret1.i);

    ret1.vars.insert(ret2.vars.begin(), ret2.vars.end());
    Ret ret(make_unique<Implies>(move(ret1.op), move(ret2.op)), ret1.vars,
            ret2.i + 1);
    return ret;
  }

  case 'E': {
    Ret ret1 = parse(input, i + 1);
    Ret ret2 = parse(input, ret1.i);

    ret1.vars.insert(ret2.vars.begin(), ret2.vars.end());
    Ret ret(make_unique<Equals>(move(ret1.op), move(ret2.op)), ret1.vars,
            ret2.i + 1);
    return ret;
  }
  default: {
    Ret ret(make_unique<Var>(string(1, input[i])), {string(1, input[i])},
            i + 1);
    return ret;
  }
  }
}

bool tautology = true;

void check_tautology(const Ret &ret) {
  unordered_map<int, string> var_idx;
  vector<bool> vars;
  unordered_map<string, bool> env;

  int i = 0;
  for (unordered_set<string>::iterator it = ret.vars.begin();
       it != ret.vars.end(); it++) {
    var_idx[i++] = *it;
  }

  for (i = 0; i < (1 << var_idx.size()); i++) {
    for (int j = 0; j < var_idx.size(); j++) {
      env[var_idx.find(j)->second] = i & (1 << j);
    }

    if (not ret.op->evaluate(env)) {
      tautology = false;
      return;
    }
  }
}

int main() {
  string input;

  while (true) {
    cin >> input;

    if (input == "0")
      break;

    Ret ret = parse(input, 0);
    tautology = true;

    check_tautology(ret);

    if (tautology)
      cout << "tautology" << endl;
    else
      cout << "not" << endl;
  }
}