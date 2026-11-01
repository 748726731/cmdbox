#pragma once
#include "list.h"
#include "storage存放.h"

struct TrieNode
{
    map<char,TrieNode*>Map;
    vector<Node*>items;//item,*prev,*next
};
struct App
{
    string path;
    CommandList *L;
    TrieNode *T;
};
void commit(App *app);
//增删改了链表后，重建树，新链表放回data里
void trieInsert(TrieNode *root,string cmd,Node *p);
//root是一颗空树。cmd是命令字符串，是p->item.cmd
void collect(TrieNode *cur,vector<Node*>&res);
vector<Node*>trieComplete(TrieNode *root,string prefix);
//树补全
void trieFree(TrieNode *&root);