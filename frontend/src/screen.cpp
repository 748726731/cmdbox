#include "screen.h"
#include<bits/stdc++.h>
using namespace std;

void draw(App* app,string input,vector<Node*>&cand,int sel,string msg)
{
    cout<<"\033[2J\033[H";//清屏;
    cout<<"CmdBox ↑↓ 选择  Tab补全命令  Enter搜索  Ctrl+C退出\n";
    cout<<"数据文件:"<<app->path<<"\n\n";
    cout<<"> "<<input<<"\n";
    int start;
    start=(sel>=6)?sel-5:0;
    //如果下标在1-6共6条命令就不滚动。
    //如果在7条及以上就开始滚动
    for(int i=start;i<(int)cand.size()&&i<start+6;i++)
    //从i开始往下走，只画6行。
    //例如i=3,画123456,i=7，画234567
    {
        cout<<(i==sel?"> ":"  ")<<highlight(cand[i]->item.cmd,input,"")<<"  "<<"# "<<highlight(cand[i]->item.note,input,GRAY)<<"  ["<<highlight(cand[i]->item.tag,input,GRAY)<<"]"<<" id:"<<cand[i]->item.id<<RESET<<"\n"; 
    }
    if(msg!="")cout<<"\n"<<msg<<"\n";
    cout<<flush;
}
//实时界面
void showItems(App *app,string input)
{
    cout<<"\033[2J\033[H";
    cout<<"搜索: "<<(input==""?string("全部"):input)<<"\n\n";
    int found=0;
    for(Node *p=app->L->dummyH->next;p!=app->L->dummyT;p=p->next)
    {
        CommandItem &x=p->item;
        if(input!="") 
        {
            string s=x.cmd+"\n"+x.note+"\n"+x.tag;
            vector<int>firstchar=kmpAll(s,input);
            if(firstchar.empty())continue;
        }
        found++;
        cout<<highlight(x.cmd,input,"")<<"\n";
        //输出红白串
        cout<<"  "<<highlight("# "+x.note+"  ["+x.tag+"]",input,GRAY)<<"\n";
        //输出红灰串,灰色是note,红色是tag
    }//按下回车以后
    if(found==0) cout<<"没有找到\n";
    cout<<"\n按任意键返回..."<<flush;
    string tmp;
    readKey(tmp);//等待按按键
}
//Enter界面