//
// Created by Strawberry on 2021/10/24.
//

#include "header.h"

//完成文法的输入
void input() {
    //首先输入文法起始符
    freopen("input.txt", "r", stdin);
    cout << "Input S" << endl;
    string s;
    cin >> s;
    G.S = s;
    //输入非终结符
    cout << "Input N" << endl;
    string input_s;
    cin >> input_s;
    while (input_s != "#") {
        G.N.push_back(input_s);
        cin >> input_s;
    }
//    for (const auto &item: G.N) {
//        cout << item << endl;
//    }

    //然后输入终结符
    cout << "Input T" << endl;
    cin >> input_s;
    while (input_s != "#") {
        G.T.push_back(input_s);
        cin >> input_s;
    }
//    for (const auto &item: G.T) {
//        cout << item << endl;
//    }

    //然后输入产生式
    // ~ 代表空
    cout << "Input P" << endl;
    string key;
    string value;
    cin >> key;
    while (key != "#") {
        cin >> value;
        G.P.key.push_back(key);
        G.P.value.push_back(value);
        cin >> key;
    }
    cin.clear();
//    for (int i = 0; i < G.P.key.size(); i++) {
//        cout << G.P.key[i] << "->" << G.P.value[i] << endl;
//    }
}


//FIRST
void construct_first() {
    bool changed = true; //用来指示本次循环中是否改变了某个非终结符的FIRST
    while (changed) {
        changed = false;
        //遍历每个产生式 找到其key的FIRST
        for (int i = 0; i < G.P.key.size(); i++) {
            //对于该非终结符
            if (std::count(G.T.begin(), G.T.end(), string(1, G.P.value[i][0])) != 0) {
                //如果产生式的第一个是终结符 直接加入
                //首先记录加入之前 FIRST 的大小 便于与插入之后作比较 如果不同则说明新加入了 还得再进行一轮
                int before = G.FIRST[G.P.key[i]].size();
                G.FIRST[G.P.key[i]].insert(string(1, G.P.value[i][0]));
                if (before != G.FIRST[G.P.key[i]].size()) {
                    changed = true;
                }

            } else if (G.P.value[i] == "~") {
                //如果产生式是空 则加入
                int before = G.FIRST[G.P.key[i]].size();
                G.FIRST[G.P.key[i]].insert("~");
                if (before != G.FIRST[G.P.key[i]].size()) {
                    changed = true;
                }
            } else {
                //如果产生式的第一个是非终结符 则将他的全部的FIRST加入即可
                int before = G.FIRST[G.P.key[i]].size();
                for (const auto &item: G.FIRST[string(1, G.P.value[i][0])]) {
                    if (item != "~") {
                        G.FIRST[G.P.key[i]].insert(item);
                    }
                }
                //再处理
                //依次判断当前的产生式的每一个非终结符是否能够推出空?如果可以则再往下判断 直到遇到终结符or走完为止 将此终结符or~加入FIRST
                bool breakFlag = false;

                for (int m = 0; m < G.P.value[i].length(); m++) {
                    char item = G.P.value[i][m];
                    if (std::count(G.N.begin(), G.N.end(), string(1, item)) != 0) {
                        //首先确定是非终结符 然后去看他的FIRST集是否有空 如果有说明他能推出空
                        if (G.FIRST[string(1, item)].count("~") != 1) {
                            //无空 直接跳出
                            breakFlag = true;
                            break;
                        } else {
                            //当前非终结符能推出空 则把他下一个字符的FIRST集加入
                            if (m + 1 < G.P.value[i].length() &&
                                std::count(G.T.begin(), G.T.end(), string(1, G.P.value[i][m + 1])) == 0) {
                                for (const auto &item2: G.FIRST[string(1, G.P.value[i][m + 1])]) {
                                    if (item2 != "~") {
                                        G.FIRST[G.P.key[i]].insert(item2);
                                    }
                                }
                            }
                        }
                    } else {
                        //走到了终结符 直接加入
                        G.FIRST[G.P.key[i]].insert(string(1, item));
                        breakFlag = true;
                        break;
                    }
                }
                if (!breakFlag) {
                    //不是break出来的 则说明是空 把~加入到FIRST
                    G.FIRST[G.P.key[i]].insert("~");
                }
                if (before != G.FIRST[G.P.key[i]].size()) {
                    changed = true;
                }
            }
        }
    }
}


//FOLLOW
void construct_follow() {
    bool changed = true;
    while (changed) {
        //遍历全部的非终结符
        changed = false;
        for (const auto &n_word: G.N) {
            //对每个非终结符来说 遍历一遍全部的产生式 在value中找到包含他的产生式
            //如果是起始符 则加入 $
            if (n_word == G.S) {
                G.FOLLOW[n_word].insert("$");
            }
            for (int i = 0; i < G.P.key.size(); i++) {
                auto index = G.P.value[i].find(n_word);
                if (index != string::npos) {
                    //在这个value中包含本次的非终结符
                    int before = G.FOLLOW[n_word].size();
                    //情况如下
                    //1、包含他 且他的下一位有符号 例如 S->Aa or S->ABC 则把FIRST(a) or FIRST(BC)加入到FOLLOW(A)
                    if (index < G.P.value[i].length() - 1) {
                        //把后续的字符截出来 依次把他们FIRST中的非空元素加入
                        string right_str = G.P.value[i].substr(index + 1);
                        bool all_empty = true;
                        if(std::count(G.T.begin(), G.T.end(), string(1,right_str[0])) != 0){
                            //如果右边的字符 第一个就是终结符 则就加他自己就行
                            G.FOLLOW[n_word].insert(string(1,right_str[0]));
                            all_empty = false;
                        }else{
                            for (char m: right_str) {
                                string c = string(1, m);
                                //如果当前字符是终结符 则直接加入 因为终结符的FIRST就是他本身
                                if (std::count(G.T.begin(), G.T.end(), c) != 0) {
                                    G.FOLLOW[n_word].insert(c);
                                    all_empty = false;
                                } else {
                                    //当前字符不是终结符 则加入这一串的FIRST集的非空元素
                                    //先看当前这个非终结符的FIRST是否存在空
                                    if (G.FIRST[c].count("~") == 0) {
                                        //有一个不存在空 则说明后续不可能是全空
                                        all_empty = false;
                                        //当前非终结符不能推出空 则后续的FIRST其实就是他的FIRST 只加入他的FIRST即可 然后跳出循环
                                        for (const auto &to_be_add: G.FIRST[c]) {
                                            if (to_be_add != "~") {
                                                G.FOLLOW[n_word].insert(to_be_add);
                                            }
                                        }
                                        break;
                                    }else{
                                        //当前非终结符能推出空 则后续串的FIRST除了他自己的以外还有他下一个的FIRST
                                        for (const auto &to_be_add: G.FIRST[c]) {
                                            if (to_be_add != "~") {
                                                G.FOLLOW[n_word].insert(to_be_add);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        //1.1、在下一位有符号且是非终结符的基础上 判断后续的一直到结束能否推到到空(也就是判断他的FIRST里面有没有空) 如果能的话就把FOLLOW(key)加入到要求的FOLLOW
                        if (all_empty) {
                            //后续可以为全空 则加入key的FOLLOW集
                            for (const auto &item: G.FOLLOW[G.P.key[i]]) {
                                G.FOLLOW[n_word].insert(item);
                            }
                        }
                    }

                    //2、包含他 且他的下一位没有符号 则把FOLLOW(key)加入到要求的FOLLOW
                    else if (index == G.P.value[i].length() - 1) {
                        for (const auto &item: G.FOLLOW[G.P.key[i]]) {
                            G.FOLLOW[n_word].insert(item);
                        }
                    }
                    //看是否变化
                    if (before != G.FOLLOW[n_word].size()) {
                        changed = true;
                    }
                }
            }
        }
    }
}