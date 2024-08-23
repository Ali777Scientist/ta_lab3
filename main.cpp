#include <iostream>
#include "sources/build/parser.tab.hpp"

extern FILE* yyin;
extern Node *global_root;//root of tree of syntax

//variable
std::vector<std::vector<std::shared_ptr<A>>> global_var;
std::map<std::string, std::vector<std::vector<std::shared_ptr<A>>>> global_vars;
std::map<std::string, std::vector<std::vector<std::shared_ptr<A>>>> &global_vars_pointer = global_vars;
int global_m;//rows
int global_n;//columns
int global_m_i;//iter rows
int global_n_i;//iter columns

std::map<std::string, std::shared_ptr<A>> global_func;
std::any global_param;
std::any global_ret;

bool isfunc = false;


int main(int argc, char* argv[]) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            std::cerr << "Error in opening the file: " << argv[1] << std::endl;
            return 1;
        }
    }
    yyparse();
    auto tree = global_root->val;//tree
    //working with Tree
    try{
        tree->exe();
        std::cout << std::any_cast<std::string>(global_vars_pointer["LEXUS"].at(0).at(1)->getvalue());

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    std::cout << (global_func.find("myfunc") == global_func.end());
    //std::cout << std::any_cast<std::shared_ptr<A>>(global_func["myfunc"]);
    if (yyin) fclose(yyin);
    return 0;
}