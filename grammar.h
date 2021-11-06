//
// Created by Strawberry on 2021/10/24.
//

#ifndef GRAMMARANALYSIS_GRAMMAR_H
#define GRAMMARANALYSIS_GRAMMAR_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <set>
#include <iomanip>
using namespace std;

struct Production{
    vector<string> key;
    vector<string> value;
}typedef Pro;

class grammar {
public:
    vector<string> N; //非终结符集合
    vector<string> T; //终结符集合
    string S; //起点式
    Pro P; //文法式 <非终结符,产生式>
    map<string,set<string>> FIRST; //FIRST集
    map<string,set<string>> FOLLOW; //FOLLOW集
    map<string, map<string,string>> TABLE; //预测分析表<非终结符,<终结符,产生式>>
};


#endif //GRAMMARANALYSIS_GRAMMAR_H
