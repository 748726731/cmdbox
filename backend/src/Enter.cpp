#include "Enter.h"
#include "storage存放.h"     //saveItems
#include "linux.h"//rawOff,rawOn

bool hasBar(string s)
{
    for(int i=0;i<(int)s.size();i++)
    {
        if(s[i]=='|')return true;
    }
    return false;
}
//用于addFLow,检查字段里有无切割符
bool askLine(string input,string &out)
{
    out="";//用户子串
    string s;
    cout<<"\n\n";
    while(true)
    {
        cout<<"\r\033[K"<<input<<out<<flush;
        // \r=光标回到本行开头
        KeyType k=readKey(s);
        if(k==Quit)return false;
        if(k==Enter)return true;
        if(k==Char)out+=s;
        else if(k==Back&&out.empty()==0)
        {
            int i=out.size()-1;
            while(((unsigned char)out[i]&0xC0)==0x80)i--;
            //如果是尾字节就下标-1
            out.erase(i);
        }
    }
}
//从用户那里拿子串。

void addFlow(App *app,string &msg)
{
    string cmd,note,tag;
    if(askLine("命令: ",cmd)==0||cmd==""){msg="已取消";return;}
    if(askLine("注释(可留空): ",note)==0){msg="已取消";return;}
    if(askLine("标签(默认 misc): ",tag)==0){msg="已取消";return;}
    if(tag=="")tag="misc";
    if(hasBar(cmd)||hasBar(note)||hasBar(tag))
    {
        msg="字段不能有|";
        return;
    }
    addItem(app->L,cmd,note,tag,-1);
    saveItems(app->path,app->L);
    msg="已添加";
}
void delFlow(App *app,int id,string &msg)
{
    Node *p=findById(app->L,id);
    if(p==nullptr)
    {
        msg="无此编号 #"+to_string(id);
        return;
    }
    removeNode(app->L,p);
    saveItems(app->path,app->L);
    msg="已删除 #"+to_string(id);
}
void editFlow(App *app,int id,string &msg)
{
    Node *p=findById(app->L,id);
    if(p==nullptr)
    {
        msg="没有编号 #"+to_string(id);
        return;
    }

    CommandItem nw=p->item;
    string s;
    if(askLine("新命令（回车不改）: ",s)==0){msg="已取消";return;}
    if(s!="") nw.cmd=s;
    if(askLine("新注释（回车不改）: ",s)==0){msg="已取消";return;}
    if(s!="") nw.note=s;
    if(askLine("新标签（回车不改）: ",s)==0){msg="已取消";return;}
    if(s!="") nw.tag=s;

    if(hasBar(nw.cmd)||hasBar(nw.note)||hasBar(nw.tag))
    {
        msg="字段里不能有 |";
        return;
    }

    p->item=nw;
    saveItems(app->path,app->L);
    msg="已修改 #"+to_string(id);
}

void runCommand(App *app,string line,string &msg)
{
    int pos=line.find(' ');//find返回无符号下标
    string name,rest="";
    if(pos==-1)//:add,找不到' '返回值是-1
    {
        name=line;
    }
    else// 空格前是命令名，空格后是剩下的
    {
        name=line.substr(0,pos);
        rest=line.substr(pos+1);
    }

    if(name=="add")addFlow(app,msg);
    else if(name=="del"||name=="edit")
    {
        int id=0;
        parseInt(rest,id);
        
        if(name=="del")delFlow(app,id,msg);
        else editFlow(app,id,msg);
    }
    else msg="未知命令: "+name;
} 
//跑冒号指令.