//
// Created by Strawberry on 2021/10/25.
//

#include "header.h"


//构造预测分析表
void constructTable() {
    for (int i = 0; i < G.P.key.size(); i++) {
        string n_word = G.P.key[i];
        string production = G.P.value[i];

        //先求出production的FIRST集
        set<string> production_FIRST;
        if (std::count(G.T.begin(), G.T.end(), string(1, production[0])) != 0) {
            //如果第一个字符是终结符 则他的FIRST就是它本身
            production_FIRST.insert(string(1, production[0]));
        }
        else {
            //如果第一个字符不是终结符 则求FIRST如下
            for (char m: production) {
                string c = string(1, m);
                //cout << "c=" << c << endl;
                //如果当前字符是终结符 则直接加入 因为终结符的FIRST就是他本身
                if (std::count(G.T.begin(), G.T.end(), c) != 0) {
                    production_FIRST.insert(c);
                } else {
                    //当前不是终结符 则加入这一串的FIRST集的非空元素
                    //先看当前这个非终结符的FIRST是否存在空
                    if (G.FIRST[c].count("~") == 0 && c != "~") {
                        //当前非终结符不能推出空 则后续的FIRST其实就是他的FIRST 只加入他的FIRST即可 然后跳出循环
                        for (const auto &to_be_add: G.FIRST[c]) {
                            production_FIRST.insert(to_be_add);
                        }
                        break;
                    } else if(G.FIRST[c].count("~") != 0){
                        //当前非终结符能推出空 则后续串的FIRST除了他自己的以外还有他下一个的FIRST
                        for (const auto &to_be_add: G.FIRST[c]) {
                            production_FIRST.insert(to_be_add);
                        }
                    }else if(c == "~"){
                        production_FIRST.insert("~");
                    }
                }
            }
        }
        //production_FIRST end

//        for (const auto &item : production_FIRST){
//            cout << "item:" << item << endl;
//        }

        //对于production_FIRST中的每个终结符 把产生式放入指定位置
        for (const auto &item : production_FIRST){
            if(std::count(G.T.begin(), G.T.end(), item) != 0){
                //是终结符
                G.TABLE[n_word].insert(pair<string,string>(item,n_word + "->" + production));
            }
        }

        //如果production_FIRST包含空 则对于FOLLOW(key)中的每个b 放入
        if(production_FIRST.count("~") != 0){
            for (const auto &item : G.FOLLOW[n_word]){
                G.TABLE[n_word].insert(pair<string,string>(item,n_word + "->" + production));
            }
        }


    }
    for (const auto &n_word : G.N){
        //2021-11-6更改
        //增加同步信息synch
        //对于每个n_word 如果他的FOLLOW集中的元素对应在TABLE中为空 给他加上synch
        for (const auto &item : G.FOLLOW[n_word]){
            map<string,string> temp = G.TABLE[n_word];
//            cout << n_word << endl;
//            for (const auto &item2 : temp){
//                cout << item2.first << endl;
//            }
            if(temp.find(item) == temp.end()){
                //如果该FOLLOW集中的元素不存在于其table中 则加入synch
                G.TABLE[n_word].insert(pair<string,string>(item,n_word + "->" + "synch"));
                //cout << "synch:" << item << endl;
            }
        }

        //2021-11-6更改
        //增加错误信息error
        //全部构造完之后 对于现在全部的还未加入的非终结符 给他加上error
        for (const auto &item : G.T){
            map<string,string> temp = G.TABLE[n_word];
            if(temp.find(item) == temp.end()){
                G.TABLE[n_word].insert(pair<string,string>(item,n_word + "->" + "error"));
                //cout << "error:" << item << endl;
            }
        }
    }
}