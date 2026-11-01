#include "list.h"
#include "storage存放.h"
#include "kmp_search.h"
#include "trie.h"
#include "linux.h"
#include "Enter.h"
#include "screen.h"

#include "list.cpp"
#include "storage存放.cpp"
#include "kmp_search.cpp"
#include "trie.cpp"
#include "linux.cpp"
#include "Enter.cpp"
#include "screen.cpp"

int main(int argc,char*argv[])
{
bool pick=false;//pick是终端模式
if(argc>1&&string(argv[1])=="--pick")pick=true;
//ai生成-脚本所需内容

    App app;//path,L,T;
    app.path=dataPath();
    initList(app.L);
    app.T=new TrieNode();
    //建立一棵树

   loadItems(app.path,app.L);
    //从本地文件下载命令，读instream in+in.open()+getline in
    //下载=分割+id转类型+命令4部分合成为一命令+新建节点合并到链表里
    //如果没有data.txt就新建txt，用源代码里的预制命令
    commit(&app);
    //commit重建树,根据链表的数据

    rawOn();//改Linux原本设置


streambuf *oldBuf=cout.rdbuf();
ofstream tty;
string chosen="";
if(pick)
{     
    tty.open("/dev/tty");     
    cout.rdbuf(tty.rdbuf());
}//ai生成-脚本所需内容


    string input,msg,text;
    //input=用户子串,msg提示词,text=按下的键
    int sel=0;
    //sel=命令的下标

    while(true)
    {
        vector<Node*> cand=trieComplete(app.T,input);
        //拿用户输入的子串，找子串下的所有节点
        //例如输入git，能拿到git add -A,git commit -m ""节点

        draw(&app,input,cand,sel,msg);
        //显示函数
        msg="";

        KeyType k=readKey(text);
        //等待按按键.例如输入了git后等待按enter
        if(k==Quit)break;
        if(k==Char)
        {
            input+=text;
            sel=0;
        }
        else if(k==Back)
        {
        if(input.empty()==0)
        {   
            int i=input.size()-1;//
            while(i>0&&((unsigned char)input[i]&0xC0)==0x80)i--;
            //位与&检查,因为要检查他的前两位是不是10，是10说明是2-4字节。
            //s[i]与0xC0=1100 0000位于比较，由于末尾6位0xC0都是0，因此结果六位都是0
            //剩下前缀2位，如果是10和11比，就是10 000000 =0x80
            //就说明是尾字节，非头字节
            //中文字符是2-4字节，若是3字节的中文字符，则是1110 xxxx,4字节则1111 xxxx
            //2字节 1100 xxxx,如果是2-4字节的，他还会有除了头字节外的尾字节
            //尾字节是10xx xxxx,所以这么位于比较。
            //然后i--，直到找到头字节，
            input.erase(i);
            //erase的i指向头字节，就把头字节和尾字节删了。 
        }
        sel=0;
        }
        //由于按了BackSpace后，需要重新检测input
        //因此需要额外popChar，
        //而非只是用户看不到那个字符了而已。
        else if(k==Up)
        {
            if(sel>0)sel--;
        }
        else if(k==Down)
        {
            if(sel+1<(int)cand.size())sel++;
        }
        else if(k==Tab)
        {
            if(!cand.empty())input=cand[sel]->item.cmd;
            //此刻出来了命令，按了上下键盘，指向命令了
            //然后按tab补全你的命令;
            sel=0;//补全后input变了，然后候选命令only自己了
            if(pick)//脚本模式:补全完就把这一行交给bash
            {
                chosen=input;
                break;
            }
        }
        else if(k==Enter)
        {     
            if(input.size()>0&&input[0]==':')
            {
                if(runCommand(&app,input.substr(1),msg)==0)break;
                //substr(1)：从第一行开始取input,当string
                //然后runConmand看他:后，空格前是啥。空格后是啥。
                //这里干了两件事，一个是查看是否是quit(==0)
                //一个是运行除了quit外的指令。
            }
            //检查是否是命令
            else{showItems(&app,input);}//搜索命令
            input="";
            sel=0;
            //按回车后要初始化，不按回车不用。
        }
    }

    rawOff();
    cout.rdbuf(oldBuf);//换回真stdout
    if(pick){if(chosen!="")cout<<chosen<<"\n";}
    //终端模式,返回选中命令.
    else{cout<<"\n已退出,数据保存在 "<<app.path<<"\n";}
    trieFree(app.T);
    destroyList(app.L);
    return 0;
}
