//
// Created by Strawberry on 2021/10/24.
//

#ifndef GRAMMARANALYSIS_HEADER_H
#define GRAMMARANALYSIS_HEADER_H

#endif //GRAMMARANALYSIS_HEADER_H
#include "grammar.h"

extern void input();
extern void construct_first();
extern void construct_follow();
extern void constructTable();
extern void analysis();

extern grammar G;
extern vector<string> stack;
extern vector<char> buffer;