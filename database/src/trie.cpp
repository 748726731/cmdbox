#include "trie.h"

void collect(TrieNode *cur,vector<Node*>&res)
{
    for(Node *p:cur->items) res.push_back(p);
    //例如子串git,txt里有git add -A，git commit -m "fix",cur指向t
    //items里没有东西可以遍历的话，range-for不会执行
    for(auto &e:cur->Map) collect(e.second,res);
    //res是个袋子
}//在子串末尾树节点处，往下探索继续找链表节点，找到了就丢到p里。

void commit(App *app)
{
    trieFree(app->T);
    app->T=new TrieNode();
    for(Node *p=app->L->dummyH->next;p!=app->L->dummyT;p=p->next)
    {
        trieInsert(app->T,p->item.cmd,p);
    }
    saveItems(app->path,app->L);
}
//增删改了链表后，重建树，新链表放回data里
void trieInsert(TrieNode *root,string cmd,Node *p)
{
    TrieNode *cur=root;
    for(char c:cmd)
    {
            auto it=cur->Map.find(c);
            //从根字典找书页
            if(it==cur->Map.end())
            {
                TrieNode *child=new TrieNode();
                cur->Map.insert(pair<char,TrieNode*>(c,child));
                cur=child;
            }//如果没有就新建书页
            else{cur=it->second;}
            //有的话就走到书页那里，找书页的书页
    }
    cur->items.push_back(p);
    //把p搞到路径末尾，从而拿到更多信息
}
//字符串+链表信息节点插字典树上
vector<Node*>trieComplete(TrieNode *root,string prefix)
{
    vector<Node*>res;//袋子
    TrieNode *cur=root;
    for(char c:prefix)
    {
        auto it=cur->Map.find(c);
        if(it==cur->Map.end())
        //在根节点的map上找书页c
        {return {};}
        else{cur=it->second;}
    }//循环使得cur从第一个子传字符开始走，往后一直走到最后一个字符
    collect(cur,res);//收集从最后一个字符走到的所有节点
    return res;
}
//树上找子串，返回子串匹配到的所有命令的所有信息.
void trieFree(TrieNode *&root)
{
    if(root==nullptr) return;
    //root是在main调用的，不会消失
    //只给child分配了内存。
    for(auto &e:root->Map)trieFree(e.second);
    delete root;
    root=nullptr;
}
