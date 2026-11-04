#pragma once  

bool hasBar(string s);
bool askLine(string prompt,string &out);

void addFlow(App *app,string &msg);
void delFlow(App *app,int id,string &msg);
void editFlow(App *app,int id,string &msg);
void runCommand(App *app,string line,string &msg);
