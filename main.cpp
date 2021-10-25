#include "header.h"

grammar G;
vector<string> stack;
vector<char> buffer;


int main() {
    std::cout << "Hello, World!" << std::endl;
    input();
    construct_first();
    cout << "FIRST:" << endl;
    for (const auto &item : G.FIRST){
        cout << item.first << ":{ ";
        for (const auto &item2 : item.second){
            cout << item2 << " ";
        }
        cout << "}\n";
    }
    construct_follow();
    cout << "FOLLOW:" << endl;
    for (const auto &item : G.FOLLOW){
        cout << item.first << ":{ ";
        for (const auto &item2 : item.second){
            cout << item2 << " ";
        }
        cout << "}\n";
    }
    constructTable();
    cout << "TABLE:" << endl;
    for (const auto &item : G.TABLE){
        for (const auto &item2 : item.second){
            cout << "{ " << item.first << "-" <<item2.first << " : ( " << item2.second << " ) } ";
        }
        cout << "\n";
    }

    cout << "Input w:" << endl;
    string w;
    cin >> w;
    //先将 $和起始符压栈
    stack.emplace_back("$");
    stack.push_back(G.S);
    //将输入串放入buffer
    for (const auto &item : w){
        buffer.emplace_back(item);
    }
    buffer.emplace_back('$');
    analysis();

    return 0;
}
