#include "kmp_search.h"

vector<int>getnext(string p)
{
    vector<int>nxt(p.size(),0);
    int index=0;
    for(int i=1;i<(int)p.size();i++)
    {
        while(index>0&&p[i]!=p[index])
        {
            index=nxt[index-1];
        }
        if(p[i]==p[index])
        {
            index++;                                                                       
        }
        nxt[i]=index;
    }
    return nxt;
}
//工具函数，获得next表kmpAll使用
//a b a  c a b a,aba<->aba,
//0 0 1  0 1 2 3
//tail+'b',c!=b
//aba(c)!=aba(b).3+1失效
//看x+1的x是生效的，用nxt表
//由于012=456,不可用，则查01是否=56
//nxt[2]表是aba比对01,12,也就是01是否等于56
//发现01!=12,也就是01!=56,
//再缩水查0=2,也就是0=6.
//a(b)==a(b)前缀1+1生效
//再判断最后那个1是否生效
//若生效,k++
//b=b(if(p[i]==p[k])k++)
//k=2
//若失效k-- k=0;

vector<int>kmpAll(string s,string p)
{
    vector<int>firstchar;
    if(p.empty()==1) return firstchar;
    vector<int>nxt=getnext(p);
    int j=0;
    for(int i=0;i<(int)s.size();i++){
        while(j>0&&s[i]!=p[j])j=nxt[j-1];
        if(s[i]==p[j])j++;
        if(j==(int)p.size())
        {
            firstchar.push_back(i-j+1);
            j=nxt[j-1];
        }
    }
    return firstchar;
}
string highlight(string s,string p,string base)
{
    vector<bool>hit(s.size(),false);//hit是vector名,开文本串长度，全给false
    for(int st:kmpAll(s,p))//st是首字符下标
    {
        for(int k=0;k<(int)p.size();k++)
        {hit[st+k]=true;}//一号位是1,7,12
        //p子串长度是2,则12 78 1213 false->true
    }
  string out;int last=-5;//一个永远不会遇见的数
    for(int i=0;i<(int)s.size();i++)
    {
        unsigned char c=s[i];
        if((c<0x80||c>0xBF)&&hit[i]!=last)
        {
            if(hit[i]==true) out+=RED;//底色+red=red
            else{out+=RESET;//变成白颜色。
            out+=base;}//如果是""就是不变
            //如果base=GRAY,就再染个灰色
            last=hit[i];
        }
        //倘若遇到中文字符，有头字节尾巴字节
        //此if判定是依旧循环走字节，如果走到尾字节就不加颜色码
        out+=s[i];//染完色后把字符搞进去
    }
out+=RESET;//彻底搞完了变成空颜色
return out;//返回染色串
}
//返回带颜色的串.|