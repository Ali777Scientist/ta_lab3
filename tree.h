//
// Created by alibi on 8/18/24.
//

#ifndef LAB3_TREE_H
#define LAB3_TREE_H

#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <utility>
#include <vector>
#include <any>
#include <utility>

class A {//template class
public:
    A() = default;
    virtual void exe() = 0;
    virtual std::any getvalue() = 0;
};

extern int global_m;//rows
extern int global_n;//columns
extern int global_m_i;//iter rows
extern int global_n_i;//iter columns

extern std::map<std::string, std::shared_ptr<A>> global_func;
extern std::map<std::string, std::vector<std::vector<std::shared_ptr<A>>>> global_vars;
extern std::map<std::string, std::vector<std::vector<std::shared_ptr<A>>>> &global_vars_pointer;
extern std::vector<std::vector<std::shared_ptr<A>>> global_var;

extern bool isfunc;
extern std::any global_param;
extern std::any global_ret;

typedef struct Node{
    std::shared_ptr<A> val;
} Node;
Node *makeNode(std::shared_ptr<A> v);

//connectors
class Stm: public A {
    std::string value;
    std::shared_ptr<A> left;
    std::shared_ptr<A> right;
public:
    explicit Stm(std::shared_ptr<A> l, std::shared_ptr<A> r) : left(std::move(l)), right(std::move(r)){}
    void exe() override {left->exe(); right->exe();}
    std::any getvalue() override {return "Stm";}
};
class Vars: public A {
    std::string value;
    std::shared_ptr<A> left;
    std::shared_ptr<A> right;
public:
    explicit Vars(std::shared_ptr<A> l, std::shared_ptr<A> r) : left(std::move(l)), right(std::move(r)){}
    void exe() override;
    std::any getvalue() override {return std::string("Vars");}
};
class List: public A {
    std::string value;
    std::shared_ptr<A> left;
    std::shared_ptr<A> right;
public:
    explicit List(std::shared_ptr<A> l, std::shared_ptr<A> r) : left(std::move(l)), right(std::move(r)){}
    void exe() override;
    std::any getvalue() override {return std::string("List");}
};


//Terminals, ID, INT, BOOL, STRING
class [[maybe_unused]] Id : public A {
    std::string value;
public:
    explicit Id(std::string str) : value(std::move(str)){}
    void exe() override {}
    std::any getvalue() override {return value;}
};
class INTv : public A {
    int value;
public:
    explicit INTv(int v) : value(v){}
    void exe() override {}
    std::any getvalue() override {return value;}
};
class BOOLv : public A {
    bool value;
public:
    explicit BOOLv(bool v) : value(v){}
    void exe() override {}
    std::any getvalue() override {return value;}
};
class STRINGv : public A {
    std::string value;
public:
    explicit STRINGv(std::string str) : value(std::move(str)){}
    void exe() override {}
    std::any getvalue() override {return value;}
};

//functions
class NewFunc : public A {
    std::string name;
    std::shared_ptr<A> tree;
public:
    NewFunc(std::string n, std::shared_ptr<A> t) : name(std::move(n)), tree(std::move(t)) {}
    void exe() override;
    std::any getvalue() override {return std::string("newFunc");}
};
class CFunc : public A {
    std::any value;
    std::string name;
    std::shared_ptr<A> param;
public:
    CFunc(std::string n, std::shared_ptr<A> t) : name(std::move(n)), param(std::move(t)) {}
    void exe() override;
    std::any getvalue() override {return value;}
};
class Func : public A {
    std::any value;
    std::map<std::string, std::vector<std::vector<std::shared_ptr<A>>>> vars;
    std::shared_ptr<A> tree;
public:
    explicit Func(std::shared_ptr<A> t) : tree(std::move(t)) {}
    void exe() override;
    std::any getvalue() override {return value;}
};
class Return : public A{
    std::any value;
    std::shared_ptr<A> oper;
public:
    explicit Return(std::shared_ptr<A> o) : oper(std::move(o)) {}
    void exe() override;
    std::any getvalue() override {return value;}
};

//IF
class If : public A {
    std::shared_ptr<A> expr;
    std::shared_ptr<A> stmt;
public:
    If(std::shared_ptr<A> e, std::shared_ptr<A> s) : expr(std::move(e)), stmt(std::move(s)) {}
    void exe() override;
    std::any getvalue() override {return "If";}
};
//WHILE
class While : public A {
    bool wORu;
    std::shared_ptr<A> expr;
    std::shared_ptr<A> stmt;
public:
    While(bool w, std::shared_ptr<A> e, std::shared_ptr<A> s) : wORu(w), expr(std::move(e)), stmt(std::move(s)) {}
    void exe() override;
    std::any getvalue() override {return "While";}
};

//variables
class NewVar : public A {
    std::string name;
    int first;
    int second;
    std::shared_ptr<A> list;
public:
    NewVar(std::string n, int a, int b, std::shared_ptr<A> l) : name(std::move(n)), first(a), second(b), list(std::move(l)) {}
    void exe() override;
    std::any getvalue() override {return std::string("nVar");}
};
class Var : public A {
    std::string name;
    int first;
    int second;
    std::shared_ptr<A> *value;
public:
    Var(std::string n, int a, int b) : name(std::move(n)), first(a), second(b), value(nullptr) {}
    void exe() override { value = &global_vars_pointer[name].at(first).at(second);}
    std::any getvalue() override {return value;}
};
class Param : public A {
    std::any value;
    int status;
public:
    explicit Param(int s) : status(s) {};
    void exe() override {value = global_param;}
    std::any getvalue() override {if(isfunc){return value;}else{throw std::runtime_error("No param out of the func!");}}
};

//operations
class Plus : public A {
    std::any value;
    std::shared_ptr<A> left;
    std::shared_ptr<A> right;
public:
    Plus(std::shared_ptr<A> l, std::shared_ptr<A> r) : left(std::move(l)), right(std::move(r)) {}
    void exe() override;
    std::any getvalue() override {return value;}
};
class Minus : public A {
    std::any value;
    std::shared_ptr<A> left;
    std::shared_ptr<A> right;
public:
    Minus(std::shared_ptr<A> l, std::shared_ptr<A> r) : left(std::move(l)), right(std::move(r)) {}
    void exe() override;
    std::any getvalue() override {return value;}
};
class Eq : public A {
    std::any value;
    std::shared_ptr<A> left;
    std::shared_ptr<A> right;
public:
    Eq(std::shared_ptr<A> l, std::shared_ptr<A> r) : left(std::move(l)), right(std::move(r)) {}
    void exe() override;
    std::any getvalue() override {return value;}
};
class Invert : public A {
    std::any value;
    std::shared_ptr<A> mem;
public:
    explicit Invert(std::shared_ptr<A> m) : mem(std::move(m)) {}
    void exe() override;
    std::any getvalue() override {return value;}
};
class Equal : public A {
    std::any value;
    std::shared_ptr<A> left;
    std::shared_ptr<A> right;
public:
    Equal(std::shared_ptr<A> l, std::shared_ptr<A> r) : left(std::move(l)), right(std::move(r)) {}
    void exe() override;
    std::any getvalue() override {return value;}
};
class Convert : public A {
    int first;
    int second;
    std::any value;
    std::shared_ptr<A> oper;
public:
    Convert(int a, int b, std::shared_ptr<A> o) : first(a), second(b), oper(std::move(o)) {}
    void exe() override {}
    std::any getvalue() override {return "convert";}
};
class Digitize : public A {
    std::shared_ptr<A> oper;
public:
    explicit Digitize(std::shared_ptr<A> o) : oper(std::move(o)) {}
    void exe() override {}
    std::any getvalue() override {return "convert";}
};

//commands
class Look : public A {
    int direction;
public:
    explicit Look(int d) : direction(d) {}
    void exe() override {}
    std::any getvalue() override {return "var";}
};

class Move : public A {
    int direction;
public:
    explicit Move(int d) : direction(d) {}
    void exe() override {}
    std::any getvalue() override {return "var";}
};

#endif //LAB3_TREE_H
