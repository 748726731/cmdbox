#include "linux.h"
void rawOn()
{
    tcgetattr(STDIN_FILENO,&oldT);
    //get attr获得属性
    //STDIN_FILENO是硬盘,oldT是存放设置参数的地方,
    termios t=oldT;                       
    t.c_lflag&=~(ICANON|ECHO|ISIG);
    //给这三个属性全关了    
    //ICANON必须等回车再把字交给程序
    //ECHO敲什么键，就往屏幕上显示什么字(按上下键不会出现^[[A)
    //ISIG,按ctrl+C杀死程序

    t.c_cc[VMIN]=1;//至少要用户输入1个字节再开始读          

    t.c_cc[VTIME]=0;//计时器，拿到字节以后直接开找

    tcsetattr(STDIN_FILENO,TCSAFLUSH,&t);//交付给状态t
}//状态更新

void rawOff()
{
    tcsetattr(STDIN_FILENO,TCSAFLUSH,&oldT);
}//状态返回

KeyType readKey(string &s)
{
    unsigned char c=0;
    if(read(STDIN_FILENO,&c,1)!=1)return Quit;
    //read是linux的，(从哪读，存到哪，读几个)

    if(c==3)return Quit;//Ctrl+C=3

    if(c=='\n'||c=='\r')return Enter;//enter

    if(c=='\t')return Tab;//tab

    if(c==127||c==8)return Back;//Backspace删除

    if(c==27)//^[[A or ^[[B
    {
        unsigned char d=0;
        if(read(STDIN_FILENO,&d,1)==1)
        {
            while(read(STDIN_FILENO ,&d,1)==1)
            {
                if(d>=0x40&&d<=0x7E)break;
                //0x是16进制，64-126字母结尾
                //以ESC开头的功能键中间不允许出现字母
            }
            if(d=='A')return Up;
            if(d=='B')return Down;
        }
        return Other;//例如如果是D，就是左键，当成无用按键。
    }

    if(c<32)return Other;//32是空格，0-31是控制键例如ctrl+A

    //中文=UTF-8=2~4字节的字符
    int len=1;
    if(c>=0xF0)len=4;
    //看看一个字符有多少个字节
    //1字节 0xxx xxxx,2字节开头110x 3字节开头111x 4字节开头1111;
    //多字节的尾巴，例如3字节的另两个字节都是10xx xxxx,
    else if(c>=0xE0)len=3;
    else if(c>=0xC0)len=2;

    s="";
    s=(char)c;//把第一个字节搞进去
    for(int k=1;k<len;k++)//把剩下的字节搞进去,例如3字节的中文汉字
    //就拼成了一个完整的字.
    {
        unsigned char d=0;
        if(read(STDIN_FILENO,&d,1)!=1)break;
        s+=(char)d;
    }
    return Char;
}
//读按键