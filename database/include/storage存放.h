#pragma once
#include "list.h"
bool parseInt(const string &s,int &out);
string dataPath();
bool loadItems(string path,CommandList *L);
bool saveItems(string path,CommandList *L);
void loadDefaults(CommandList *L);