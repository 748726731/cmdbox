#include "screen.h"
#include<bits/stdc++.h>
using namespace std;

void draw(App* app,string input,vector<Node*>&cand,int sel,string msg,int &top)
{
    cout<<"\033[H";//清屏;
    cout<<"CmdBox ↑↓ 选择  Tab补全命令  Ctrl+C退出\033[K\n";
    cout<<"数据文件:"<<app->path<<"\033[K\n\033[K\n";
    cout<<"> "<<input<<"\033[K\n";
   
    if(sel<top)top=sel;
    if(sel>=top+6)top=sel-5;

    for(int i=top;i<(int)cand.size()&&i<top+6;i++)
    //从top开始往下走，只画6行以内。
    {
        cout<<(i==sel?"> ":"  ")<<highlight(cand[i]->item.cmd,input,"")<<"\033[K\n"<<"# "<<highlight(cand[i]->item.note,input,GRAY)<<"  ["<<highlight(cand[i]->item.tag,input,"")<<"]"<<" id:"<<highlight(to_string(cand[i]->item.id),input,"")<<"\033[K\n"; 
    }
    if(msg!="")cout<<"\033[K\n"<<msg<<"\033[K\n";
    cout<<"\033[J"<<flush;
}
//实时界面
