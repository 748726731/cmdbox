#pragma once
#include<bits/stdc++.h>
using namespace std;

#define RED "\033[31m"
#define GRAY "\033[90m"//颜色，Linux终端的变色
#define RESET "\033[0m"

//s是文本串,p是子串
vector<int> getnext(string p);//next表
vector<int> kmpAll(string s,string p);//声明函数
string highlight(string s,string p,string base);
//使高亮的函数
