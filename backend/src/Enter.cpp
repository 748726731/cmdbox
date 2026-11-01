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
string askLine(string input)
{
    rawOff();//恢复平常状态
    cout<<"\n"<<input<<flush;//endl=\0+flush，flush=把东西推到屏幕上
    string s;
    getline(cin,s);
    rawOn();
    return s;
}
//从用户那里拿子串。

void addFlow(App *app,string &msg)
{
    string cmd=askLine("命令: ");
    if(cmd==""){msg="已取消";return;}
    string note=askLine("注释（可留空）: ");
    string tag=askLine("标签（默认 misc）: ");
    if(tag=="")tag="misc";
    if(hasBar(cmd)||hasBar(note)||hasBar(tag))
    {
        msg="字段不能有|";
        return;
    }
    Node *p=addItem(app->L,cmd,note,tag);
    commit(app);
    msg="已添加 #"+to_string(p->item.id);
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
    commit(app);
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

    CommandItem old=p->item;
    CommandItem nw=old;
    string s=askLine("新命令（回车不改）: ");
    if(s!="") nw.cmd=s;
    s=askLine("新注释（回车不改）: ");
    if(s!="") nw.note=s;
    s=askLine("新标签（回车不改）: ");
    if(s!="") nw.tag=s;

    if(hasBar(nw.cmd)||hasBar(nw.note)||hasBar(nw.tag))
    {
        msg="字段里不能有 |";
        return;
    }

    p->item=nw;
    commit(app);
    msg="已修改 #"+to_string(id);
}

bool runCommand(App *app,string line,string &msg)
{
    int pos=line.find(' ');//find返回无符号下标
    string name,rest="";
    if(pos<0)//下标不可能小于0，小于0说明没找到
    {
        name=line;
    }
    else// 空格前是命令名，空格后是剩下的
    {
        name=line.substr(0,pos);
        rest=line.substr(pos+1);
    }
    if(name=="quit")return false;
    else if(name=="add")addFlow(app,msg);
    else if(name=="list")showItems(app,"");
    else if(name=="del"||name=="edit")
    {
        int id=0;
        for(int i=0;i<(int)rest.size();i++)
        {
            if(rest[i]>='0'&&rest[i]<='9')id=id*10+(rest[i]-'0');
            
        }//字符转整形
        if(name=="del")delFlow(app,id,msg);
        else editFlow(app,id,msg);
    }
    else msg="未知命令: "+name;
    return true;
} 
//跑冒号指令.