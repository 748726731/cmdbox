#pragma once
#include<termios.h>
#include<unistd.h>
#include<bits/stdc++.h>
enum KeyType{Char,Enter,Tab,Back,Up,Down,Quit,Other};
termios oldT;
//termios装的是linux的设置参数
using namespace std;

void rawOn();
void rawOff();
KeyType readKey(string &s);
