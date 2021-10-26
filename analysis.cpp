//
// Created by Strawberry on 2021/10/25.
//

#include "header.h"

void analysis(){
    string X = stack.back();
    string ip = string (1,buffer.front());
    int cnt = 1;

    while (X != "$"){
        cout << "\nCNT--->" << cnt << endl;
        //如果X是终结符号
        if(std::count(G.T.begin(), G.T.end(), X) != 0){
            if(ip == X){
                //X=a!=$，从栈顶弹出X，输入指针前移一个位置(就是去掉buffer的头字符)
                cout << "pop:" << X << endl;
                stack.pop_back();
                cout << "buffer_remove:" << ip << endl;
                buffer.erase(buffer.begin());
            }else{
                cout << "ERROR!!!" << endl;
                break;
            }
        }
        //如果X是非终结符
        else{
            //找到表项M[X][ip]
            if(G.TABLE.find(X) == G.TABLE.end()){
                cout << "ERROR!!!" << endl;
                break;
            }
            if(G.TABLE[X].find(ip) == G.TABLE[X].end()){
                cout << "ERROR!!!" << endl;
                break;
            }
            string production = G.TABLE[X].at(ip).substr(3);//因为前三个是 X-> 故从第三个开始往后才是产生式
            cout << "OUT:" << G.TABLE[X].at(ip) << endl;
            if(production == "~"){
                //若M[X,a]是产生式X->空
                //从栈顶弹出X
                cout << "pop:" << X << endl;
                stack.pop_back();
            }else{
                //若M[X,a]是产生式X->Y1Y2…Yn
                //先将X从栈顶弹出
                cout << "pop:" << X << endl;
                stack.pop_back();
                //然后把产生式的右部符号串按反序
                //（即按Yn、…、Yn-1、Y2、Y1的顺序）一一推入栈中；
                cout << "push:";
                for(auto it2=production.rbegin();it2 != production.rend();++it2)
                {
                    cout<<*it2;
                    stack.emplace_back(1,*it2);
                }
                cout << "\n";
            }
        }
        X = stack.back();
        ip = string (1,buffer.front());
        cout << "STACK--->";
        for (const auto &item : stack){
            cout << item << " ";
        }
        cout << "\n";
        cout << "BUFFER--->";
        for (const auto &item : buffer){
            cout << item << " ";
        }
        cout << "\n";
        cnt++;
        //如果读到了$
        if(X == "$" && ip == X){
            //X=a=$，宣告分析成功，停止分析
            cout << "Analysis Success!" << endl;
            break;
        }
    }
    //end while
}