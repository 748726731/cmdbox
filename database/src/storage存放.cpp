#include "storage存放.h"
#include<unistd.h>
//--------------------------Tool-------------------------------
void parseInt(const string &s,int &id)
{
    if(s.empty()) return ;
    if(s.size()>9) return ;
    //太长，防止溢出
    int v=0;
    for(char c:s)
    {
        if(c<'0'||c>'9') return ;  
        //混进非数字
        v=v*10+(c-'0');
    }
    id=v;
    return ;
}
//工具函数,用于loadItems:把string id变为int
vector<string>splitLine(string line,char sep)
//sep=separate：分割
{
    vector<string>parts;
    string cur;
    for(char c:line)
    {
        if(c==sep)
        {parts.push_back(cur);cur.clear();}
        else cur+=c;
    }//在不遇到分隔符之前，一股脑丢到cur里
    //遇到分隔符后，把装有东西的cur丢到part里，然后clear
    parts.push_back(cur);
    return parts;
}
//工具函数, 用于loadItems:分割一行的命令。
void loadDefaults(CommandList *L)
{
    addItem(L,"git add -A","暂存所有改动","git",-1);
    addItem(L,"git commit -m \"fix\"","提交并写明本次修复内容","git",-1);
    addItem(L,"git push","把本地提交推送到远程仓库","git",-1);
    addItem(L,"g++ -o main main.cpp","最简编译：把 main.cpp 编译成 main","build",-1);
    addItem(L,"make -j4","并行编译整个项目","build",-1);
    addItem(L,"ls -la","列出所有文件（含隐藏文件）","linux",-1);
}
//工具函数,用于loadItems:如果下载失败，制作模板txt

//--------------------------Main----------------------------------
string dataPath()
{
    string p(4096,'\0');
    readlink("/proc/self/exe",&p[0],p.size()-1);
    //readlink返回写了多少字节
    //"/proc/self/exe"=文件g++后产生的exe位置(linux不产生exe，产生的文件没有后缀)
    //readlink，先把"/proc/self/exe"改成本地地址，然后最多允许抄sizeof(buf)-1个字符到buf上。
    //ssize_t是size_t的带符号版本,sizeof()很多返回的都是size_t
    int n=p.rfind('/');
    p=p.substr(0,n+1);//n是最后一个斜杠的下标，0-n有n+1个字符
    return p+"data.txt";
}
//找路径
void loadItems(string path,CommandList *L)
{
    ifstream in;
    //设一个装文件位置类型的变量in(读)
    in.open(path);//使得in对应着path这个文件
    if(!in.is_open()){loadDefaults(L);return ;}
    string line;
    while(getline(in,line))
    //从文件里一行一行getline，getline不要\n
    {
        if(line.empty()==0&&line.back()=='\r')
        {line.pop_back();}
        //调整windows格式

        vector<string>parts=splitLine(line,'|');
        //给getline的那一行分割了，拿id,cmd,note,tag四部分
        
        int fid=0;
        parseInt(parts[0],fid);
        //把字符数字改成整数数字，存id里

        addItem(L,parts[1],parts[2],parts[3],fid);
    }
    return ;
}
//把path路径里的东西搞到in里，然后一行一行看in的命令,把命令套链表上
void saveItems(string path,CommandList *L)
{
    ofstream out;//ifstream是读,ofstream是写
    out.open(path);//给变量整上具体地址,如果没有，就创建。
    for(Node *p=L->dummyH->next;p!=L->dummyT;p=p->next)
    {
        out<<p->item.id<<'|'<<p->item.cmd<<'|'<<p->item.note<<'|'<<p->item.tag<<'\n';
        // out<< 为往文件里写什么什么东西。
    }
    return ;
}