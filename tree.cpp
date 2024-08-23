//
// Created by alibi on 8/18/24.
//
#include "tree.h"

#include <utility>

Node *makeNode(std::shared_ptr<A> v){
    Node *temp;
    temp = new Node;
    temp->val = std::move(v);
    return temp;
}
//create variables
void Vars::exe() {
    if(left->getvalue().type() == typeid(std::string)){
        if(std::any_cast<std::string>(left->getvalue()) == "Vars"){
            left->exe();
        }else{
            if(global_n_i != global_n){
                global_var.at(global_m_i).at(global_n_i) = left;
                global_n_i++;
            }else{
                throw std::runtime_error("Escaping of variable!");
            }
        }
    }else{
        if(global_n_i != global_n){
            global_var.at(global_m_i).at(global_n_i) = left;
            global_n_i++;
        }else{
            throw std::runtime_error("Escaping of variable!");
        }
    }
    if(right->getvalue().type() == typeid(std::string)){
        if(std::any_cast<std::string>(right->getvalue()) == "Vars"){
            right->exe();
        }else{

        }
    }else{
        if(global_n_i != global_n){
            global_var.at(global_m_i).at(global_n_i) = right;
            global_n_i++;
        }else{
            throw std::runtime_error("Escaping of variable!");
        }
    }
}
void List::exe() {
    if(std::any_cast<std::string>(left->getvalue()) == "List"){
        left->exe();
    }else {
        if (global_m_i != global_m) {
            global_n_i = 0;
            left->exe();
            while(global_n_i < global_n){
                INTv temp(false);
                global_var.at(global_m_i).at(global_n_i) = std::make_shared<INTv>(temp);
                global_n_i++;
            }
            global_m_i++;
        } else {
            throw std::runtime_error("Escaping of row of var!");
        }
    }
    if(std::any_cast<std::string>(right->getvalue()) == "List"){
        right->exe();
    }else {
        if (global_m_i != global_m) {
            global_n_i = 0;
            right->exe();
            while(global_n_i < global_n){
                INTv temp(0);
                global_var.at(global_m_i).at(global_n_i) = std::make_shared<INTv>(temp);
                global_n_i++;
            }
            global_m_i++;
        } else {
            throw std::runtime_error("Escaping of row of var!");
        }
    }
}
void NewVar::exe() {
    global_m = first;
    global_n = second;
    std::vector<std::vector<std::shared_ptr<A>>> vec(global_m, std::vector<std::shared_ptr<A>>(global_n));
    global_var = vec;
    global_m_i = 0;
    global_n_i = 0;

    bool onlyVars = false;//without list
    bool onlyElem = false;//without vars and list

    //filling global var
    if(list->getvalue().type() == typeid(bool) || list->getvalue().type() == typeid(int)){
        onlyElem = true;
    }

    if(list->getvalue().type() == typeid(std::string)){
        if(std::any_cast<std::string>(list->getvalue()) == "Vars"){
            onlyVars = true;
        }
        if(std::any_cast<std::string>(list->getvalue()) != "Vars" && std::any_cast<std::string>(list->getvalue()) != "List"){
            onlyElem = true;
        }
    }
    if(onlyElem){
        if(global_n_i != global_n){
            global_var.at(global_m_i).at(global_n_i) = list;
            global_n_i++;
        }else{
            throw std::runtime_error("Escaping of variable!");
        }
        while (global_n_i < global_n) {
            INTv temp(0);
            global_var.at(global_m_i).at(global_n_i) = std::make_shared<INTv>(temp);
            global_n_i++;
        }
        global_m_i++;
    }else {
        list->exe();
        if (onlyVars) {
            while (global_n_i < global_n) {
                INTv temp(0);
                global_var.at(global_m_i).at(global_n_i) = std::make_shared<INTv>(temp);
                global_n_i++;
            }
            global_m_i++;
        }
    }
    while (global_m_i < global_m) {
        for (global_n_i = 0; global_n_i < global_n; global_n_i++) {
            INTv temp(false);
            global_var.at(global_m_i).at(global_n_i) = std::make_shared<INTv>(temp);
        }
        global_m_i++;
    }
    global_m_i = 0;
    global_n_i = 0;


    //adding new var
    vec = global_var;
    if (global_vars_pointer.find(name) == global_vars_pointer.end()) {
        global_vars_pointer[name] = vec;
    } else {
        std::string s = "There is ";
        s += name;
        s += " already!";
        throw std::runtime_error(s);
    }
}

//loops
void While::exe() {
    if(expr->getvalue().type() == typeid(bool)){
        expr->exe();
        bool temp = std::any_cast<bool>(expr->getvalue());
        if(!wORu){
            temp = !temp;
        }
        while(temp){
            stmt->exe();
            expr->exe();
            temp = std::any_cast<bool>(expr->getvalue());
            if(!wORu){
                temp = !temp;
            }
        }
    }else{
        throw std::runtime_error("error with expr of If!");
    }
}
void If::exe() {
    expr->exe();
    if(expr->getvalue().type() == typeid(bool)){
        if(std::any_cast<bool>(expr->getvalue())){
            stmt->exe();
        }
    }else{
        throw std::runtime_error("error with expr of If!");
    }
}

void Equal::exe() {
    left->exe();
    right->exe();
    std::shared_ptr<A> *temp;
    std::shared_ptr<A> *r;
    std::any val;
    if(left->getvalue().type() == typeid(std::shared_ptr<A>*)){
        temp = std::any_cast<std::shared_ptr<A>*>(left->getvalue());
    }else{
        throw std::runtime_error("error with Equal, left is not variable!");
    }
    if(right->getvalue().type() == typeid(std::shared_ptr<A>*)){//is var
        r = std::any_cast<std::shared_ptr<A>*>(right->getvalue());
        if(r->get()->getvalue().type() == typeid(int)){
            val = std::any_cast<int> (r->get()->getvalue());
        }else if(r->get()->getvalue().type() == typeid(bool)){
            val = std::any_cast<bool> (r->get()->getvalue());
        }else if(r->get()->getvalue().type() == typeid(std::string)){
            val = std::any_cast<std::string> (r->get()->getvalue());
        }else{
            throw std::runtime_error("error with Equal, right is not identified!");
        }
    }else{
        val = right->getvalue();
    }

    if(val.type() == typeid(int)){
        INTv t(std::any_cast<int>(val));
        *temp = std::make_shared<INTv>(t);
    }else if(val.type() == typeid(bool)){
        BOOLv t(std::any_cast<bool>(val));
        *temp = std::make_shared<BOOLv>(t);
    }else if(val.type() == typeid(std::string)){
        STRINGv t(std::any_cast<std::string>(val));
        *temp = std::make_shared<STRINGv>(t);
    }else{
        throw std::runtime_error("error with Equal!");
    }
}
void Minus::exe() {
    left->exe();
    right->exe();
    std::any a;
    std::any b;
    std::shared_ptr<A> *l;
    std::shared_ptr<A> *r;
    if(left->getvalue().type() == typeid(std::shared_ptr<A>*)){//is var, left
        l = std::any_cast<std::shared_ptr<A>*>(left->getvalue());
        if(l->get()->getvalue().type() == typeid(int)){
            a = std::any_cast<int> (l->get()->getvalue());
        }else if(l->get()->getvalue().type() == typeid(bool)){
            a = std::any_cast<bool> (l->get()->getvalue());
        }else if(l->get()->getvalue().type() == typeid(std::string)){
            a = std::any_cast<std::string> (l->get()->getvalue());
        }else{
            throw std::runtime_error("error with Plus, left is not identified!");
        }
    }else{
        a = left->getvalue();
    }
    if(right->getvalue().type() == typeid(std::shared_ptr<A>*)){//is var, right
        r = std::any_cast<std::shared_ptr<A>*>(right->getvalue());
        if(r->get()->getvalue().type() == typeid(int)){
            b = std::any_cast<int> (r->get()->getvalue());
        }else if(r->get()->getvalue().type() == typeid(bool)){
            b = std::any_cast<bool> (r->get()->getvalue());
        }else if(r->get()->getvalue().type() == typeid(std::string)){
            b = std::any_cast<std::string> (r->get()->getvalue());
        }else{
            throw std::runtime_error("error with Equal, right is not identified!");
        }
    }else{
        b = right->getvalue();
    }
    if(a.type() == typeid(int) && b.type() == typeid(int)){
        value = std::any_cast<int>(a) - std::any_cast<int>(b);
    }else{
        throw std::runtime_error("error with Minus!");
    }
}
void Plus::exe() {
    left->exe();
    right->exe();
    std::any a;
    std::any b;
    std::shared_ptr<A> *l;
    std::shared_ptr<A> *r;
    if(left->getvalue().type() == typeid(std::shared_ptr<A>*)){//is var, left
        l = std::any_cast<std::shared_ptr<A>*>(left->getvalue());
        if(l->get()->getvalue().type() == typeid(int)){
            a = std::any_cast<int> (l->get()->getvalue());
        }else if(l->get()->getvalue().type() == typeid(bool)){
            a = std::any_cast<bool> (l->get()->getvalue());
        }else if(l->get()->getvalue().type() == typeid(std::string)){
            a = std::any_cast<std::string> (l->get()->getvalue());
        }else{
            throw std::runtime_error("error with Plus, left is not identified!");
        }
    }else{
        a = left->getvalue();
    }
    if(right->getvalue().type() == typeid(std::shared_ptr<A>*)){//is var, right
        r = std::any_cast<std::shared_ptr<A>*>(right->getvalue());
        if(r->get()->getvalue().type() == typeid(int)){
            b = std::any_cast<int> (r->get()->getvalue());
        }else if(r->get()->getvalue().type() == typeid(bool)){
            b = std::any_cast<bool> (r->get()->getvalue());
        }else if(r->get()->getvalue().type() == typeid(std::string)){
            b = std::any_cast<std::string> (r->get()->getvalue());
        }else{
            throw std::runtime_error("error with Equal, right is not identified!");
        }
    }else{
        b = right->getvalue();
    }
    if(a.type() == typeid(bool) && b.type() == typeid(bool)){
        value = std::any_cast<bool>(a) || std::any_cast<bool>(b);
    }else if(a.type() == typeid(int) && b.type() == typeid(int)){
        value = std::any_cast<int>(a) + std::any_cast<int>(b);
    }else if(a.type() == typeid(std::string) && b.type() == typeid(std::string)){
        value = std::any_cast<std::string>(a) + std::any_cast<std::string>(b);
    }else{
        throw std::runtime_error("error with Plus!");
    }
}

void Eq::exe() {
    left->exe();
    right->exe();
    std::any a;
    std::any b;
    std::shared_ptr<A> *l;
    std::shared_ptr<A> *r;
    if(left->getvalue().type() == typeid(std::shared_ptr<A>*)){//is var, left
        l = std::any_cast<std::shared_ptr<A>*>(left->getvalue());
        if(l->get()->getvalue().type() == typeid(int)){
            a = std::any_cast<int> (l->get()->getvalue());
        }else if(l->get()->getvalue().type() == typeid(bool)){
            a = std::any_cast<bool> (l->get()->getvalue());
        }else if(l->get()->getvalue().type() == typeid(std::string)){
            a = std::any_cast<std::string> (l->get()->getvalue());
        }else{
            throw std::runtime_error("error with Plus, left is not identified!");
        }
    }else{
        a = left->getvalue();
    }
    if(right->getvalue().type() == typeid(std::shared_ptr<A>*)){//is var, right
        r = std::any_cast<std::shared_ptr<A>*>(right->getvalue());
        if(r->get()->getvalue().type() == typeid(int)){
            b = std::any_cast<int> (r->get()->getvalue());
        }else if(r->get()->getvalue().type() == typeid(bool)){
            b = std::any_cast<bool> (r->get()->getvalue());
        }else if(r->get()->getvalue().type() == typeid(std::string)){
            b = std::any_cast<std::string> (r->get()->getvalue());
        }else{
            throw std::runtime_error("error with Equal, right is not identified!");
        }
    }else{
        b = right->getvalue();
    }
    if(a.type() == typeid(bool) && b.type() == typeid(bool)){
        value = std::any_cast<bool>(a) == std::any_cast<bool>(b);
    }else if(a.type() == typeid(int) && b.type() == typeid(int)){
        value = std::any_cast<int>(a) == std::any_cast<int>(b);
    }else if(a.type() == typeid(std::string) && b.type() == typeid(std::string)){
        value = std::any_cast<std::string>(a) == std::any_cast<std::string>(b);
    }else{
        throw std::runtime_error("error with Plus!");
    }
}

void Invert::exe() {
    mem->exe();
    std::any a;
    std::shared_ptr<A> *l;
    if(mem->getvalue().type() == typeid(std::shared_ptr<A>*)){//is var, left
        l = std::any_cast<std::shared_ptr<A>*>(mem->getvalue());
        if(l->get()->getvalue().type() == typeid(int)){
            a = std::any_cast<int> (l->get()->getvalue());
        }else if(l->get()->getvalue().type() == typeid(bool)){
            a = std::any_cast<bool> (l->get()->getvalue());
        }else if(l->get()->getvalue().type() == typeid(std::string)){
            a = std::any_cast<std::string> (l->get()->getvalue());
        }else{
            throw std::runtime_error("error with Plus, left is not identified!");
        }
    }else{
        a = mem->getvalue();
    }
    if(a.type() == typeid(bool)){
        value = !std::any_cast<bool>(a);
    }else if(a.type() == typeid(int)){
        value = -std::any_cast<int>(a);
    }else if(a.type() == typeid(std::string)){
        auto str = std::any_cast<std::string>(a);
        std::reverse(str.begin(), str.end());
        value = str;
    }else{
        throw std::runtime_error("error with Plus!");
    }
}


void NewFunc::exe() {
    if(!isfunc) {
        Func temp(tree);
        if (global_func.find(name) == global_func.end()) {
            global_func[name] = std::make_shared<Func>(temp);
        } else {
            std::string s = "There is ";
            s += name;
            s += " function already!";
            throw std::runtime_error(s);
        }
    }else{
        throw std::runtime_error("You can't create func in the func!!!");
    }
}
void Func::exe() {
    vars.clear();
    global_vars_pointer = vars;
    tree->exe();
    std::cout << std::any_cast<int>(vars["SUPER"].at(0).at(0));
    global_vars_pointer = global_vars;
    value = global_ret;
}
void CFunc::exe() {
    isfunc = true;
    if(global_func.find(name) != global_func.end()){
        auto func = global_func[name];
        global_param = param;
        func->exe();
        value = func->getvalue();
    }else{
        throw std::runtime_error("I couldn't find your function!");
    }
    isfunc = false;

}
void Return::exe() {
    if(isfunc) {
        oper->exe();
        global_ret = oper->getvalue();
    }else{
        throw std::runtime_error("You can't use RETURN out of the function!!!");
    }
}