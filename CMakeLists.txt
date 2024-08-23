cmake_minimum_required(VERSION 3.28)
project(lab3)

set(CMAKE_CXX_STANDARD 23)

add_executable(lab3
        main.cpp
        sources/build/lex.yy.cpp
        sources/build/parser.tab.cpp
        sources/tree.cpp
        sources/tree.h)
