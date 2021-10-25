cmake_minimum_required(VERSION 3.20)
project(GrammarAnalysis)

set(CMAKE_CXX_STANDARD 14)

add_executable(GrammarAnalysis main.cpp grammar.cpp grammar.h header.h init.cpp constructTable.cpp analysis.cpp)
