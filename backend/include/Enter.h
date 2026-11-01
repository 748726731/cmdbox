#pragma once  

bool hasBar(string s);
string askLine(string prompt);

void addFlow(App *app,string &msg);
void delFlow(App *app,int id,string &msg);
void editFlow(App *app,int id,string &msg);
bool runCommand(App *app,string line,string &msg);
