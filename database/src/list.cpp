#include "list.h"
void initList(CommandList *&L)
{
    L=new CommandList();
    L->dummyH=new Node();
    L->dummyT=new Node();
    L->dummyH->next=L->dummyT;
    L->dummyT->prev=L->dummyH;
    L->size=0;L->nextId=1;
}
void destroyList(CommandList *&L)
{
    Node *p=L->dummyH,*q;
    while(p!=nullptr)
    {
        q=p->next;
        delete p;
        p=q;
    }
    delete L;L=nullptr;
    //首先，给dummyHT分配了内存，放HT的节点的，
    //然后再新建节点输入数据，插入进去，
    //于是接下来链表成了H->1->2->3->T,
    //五个节点都是有内存的，
    //此刻qp做遍历把五个内存全都释放。
}
Node *insertBefore(CommandList *L,Node *pos,Node *p)
{
    p->prev=pos->prev;
    p->next=pos;
    pos->prev->next=p;
    pos->prev=p;
    L->size++;
    return p;
}
Node *addItem(CommandList *L,string cmd,string note,string tag)
// :add git status,查看状态，git.没有提供id，因此自动丢给末尾。
{
    CommandItem x;
    Node *p=new Node();
    x.id=L->nextId;
    L->nextId++;
    x.cmd=cmd;
    x.tag=tag;
    x.note=note;
    p->item=x;
    return insertBefore(L,L->dummyT,p);
}
void removeNode(CommandList *L,Node *p)
{
    p->prev->next=p->next;
    p->next->prev=p->prev;
    L->size--;
    delete p;
}
Node *findById(CommandList *L,int id)
{
    Node *p=L->dummyH;
    for(int i=0;i<L->size;i++)
    {
        p=p->next;
        if(p->item.id==id)return p;
    }
    return nullptr;
}