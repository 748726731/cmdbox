#pragma once
#include "list.h"
void parseInt(const string &s,int &out);
string dataPath();
void loadItems(string path,CommandList *L);
void saveItems(string path,CommandList *L);
void loadDefaults(CommandList *L);