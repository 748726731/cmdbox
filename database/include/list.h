#pragma once
#include<bits/stdc++.h>
using namespace std;

struct CommandItem//命令条款
{
    int id;
    string cmd;
    string note;//中文注释
    string tag;//分类标签,例如git,ls
};
struct Node{
    CommandItem item;
    Node *prev,*next;
};
struct CommandList{
    Node *dummyH,*dummyT;
    int size,nextId;//size是有几行命令
    //nextid=下一个编号，例如要插进来一个node
    //node就会拿走这个nextid，然后nextid++;
};

void initList(CommandList *&L);//给表和节点分配内存以至于存活。
void destroyList(CommandList *&L);//释放内存
Node *insertBefore(CommandList *L,Node *pos,Node *p);//节点插入
//把x装入一个新节点，插入pos前后;L只是为了L->size++.
//不直接丢一个Node进去的原原因是？
Node *addItem(CommandList *L,string cmd,string note,string tag);//节点插入
//新增一条命令到链表末尾。
void removeNode(CommandList *L,Node *p);//节点删除
Node *findById(CommandList *L,int id);

