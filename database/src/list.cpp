#include "list.h"
void initList(CommandList *&L)
{
    L=new CommandList();
    L->dummyH=new Node();
    L->dummyT=new Node();
    L->dummyH->next=L->dummyT;
    L->dummyT->prev=L->dummyH;
    L->size=0;
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
int nextFreeId(CommandList *L)//最小空号:1,2,3,...
{
    int id=1;
    for(Node *q=L->dummyH->next;q!=L->dummyT;q=q->next)
    {
        if(q->item.id>id)break;//中间有空号
        if(q->item.id==id)id++;//这个号被占了,看下一个
    }
    return id;
}
//发id的.
void insertBefore(CommandList *L,Node *tail,Node *p)
{
    p->prev=tail->prev;
    p->next=tail;
    tail->prev->next=p;
    tail->prev=p;
    L->size++;
    return ;
}
void addItem(CommandList *L,string cmd,string note,string tag,int fid)
// :add git status,查看状态，git.没有提供id，因此自动丢给末尾。
{
    CommandItem x;
    x.id=(fid==-1)?nextFreeId(L):fid;
    //id有两个来源，第一个是文档里的id，第二个是派发的id
    x.cmd=cmd;
    x.tag=tag;
    x.note=note;

    Node *p=new Node();
    p->item=x;

    Node *pos=L->dummyT;//按id升序插入,否则新号会跑到链表末尾
    for(Node *q=L->dummyH->next;q!=L->dummyT;q=q->next)
    {
        if(q->item.id>x.id){pos=q;break;}
        //找链表里 最开始是哪个节点的id比p的大的
    }
    insertBefore(L,pos,p);
    //找到了后就把id对应节点插入到 pos节点的前面。
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
//从L里找id，返回对应的节点，用于:del :edit