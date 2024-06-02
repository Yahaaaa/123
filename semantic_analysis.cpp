#include "main.h"

extern four F[MAX];//每条四元式信息
extern int F_count;//四元式个数
vector<int> G[1000];//
vector<string> symbol;
vector <quaternion> attr;
extern vector <quaternion> Q[MAX];//每个节点信息

string table_z[MAX];//符号表 变量名
int table_z_count = 0;
string table_h[MAX];//符号表 函数名
int table_h_count = 0;
int new_w = 0;//新的临时变量的个数


int lookup(string a)//在符号表中查找
{
    int i = 0;
    for(i = 0;i<table_z_count;i++)
    {
        if(table_z[i] == a) return 1;//已存在
    }
    return 0;//不存在
}

void backpatch(int p,int t)//回填函数 把链首P所连接的每一个四元式的第四区段都填为转移目标t 回填：还未生成，却需要
{
    std::stringstream z;
    z<<t;
    string m = z.str();
    if((F[p].d[0] >= '9' && F[p].d[0] <= '0') || m == F[p].d) //第四区段不是数字，与其他不成链
    {
        return;
    }
    else if(F[p].d[0] <= '9' && F[p].d[0] >= '0')//第四区段是数字。与其他相连
    {
        std::stringstream ss;
        ss<<t;
        string s = ss.str();
        F[p].d = s;
        int o = 0;
        for(int y = 1; y < F[p].d.length(); y++)
        {
            o*=10;
            o+=(F[p].d[y]-'0');
        }
        backpatch(o,t);
    }
}
int merger(int p1,int p2)//拉链函数 当P2为空，返回P1；P2不为空，返回P2，将P2的链尾第四区改为P1
{

    if(p2 == 0)
        return p1;
    else if(p2 != 0 )
    {
        //把p1转成string
        std::stringstream ss;
        ss<<p1;
        string s = ss.str();
        F[p2].d = s;
       //找p2链，把链尾改为p1
        return p2;
    }
}
void emit()//输出四元式
{
    int i;
    cout<<"四元式:"<<endl;
    for(i = 100;i < F_count;i++)
        cout<<i<<' '<<'('<<F[i].a<<','<<F[i].b<<','<<F[i].c<<','<<F[i].d<<')'<<endl;
    cout<<endl;
}
string newtemp()//生成新的临时变量
{
    string temp = "t";
    std::stringstream ss;
    ss<<new_w;
    string s = ss.str();
    temp.insert(temp.size(),s);
    new_w++;
    return temp;
}
//语义分析主要函数
void quater(int u)//从第u个结点开始向下遍历语法树，分析每个结点，执行相应语义动作
{
    if (G[u].empty())
    {
        return;//该结点为空直接返回 出口
    }//end if
    for(int i = 0; i<G[u].size(); i++)
    {
            quater(G[u][i]);//遍历所有的儿子节点
    }//end for

    //根据不同的产生式分析 移进、规约
    //cout<<4<<' '<<symbol[u]<<endl;
    if( symbol[u] ==  "0" )//定义变量 函数名；结点信息为0表示用第0条产生式进行规约
    {//执行第零条产生式的语义动作
        int s = attr[G[u][1]].chain;
        attr[u].chain = s;
        string name = attr[G[u][1]].name;//把已经定义过的函数名存入符号表
        table_h[table_h_count] = name;
        table_h_count++;
    }
    else if(symbol[u]=="1")//结点信息为1表示用第1条产生式进行规约
    {
        attr[u].chain = merger(attr[G[u][0]].chain,attr[G[u][2]].chain);
    }
    else if(symbol[u]=="2")
    {

       F[F_count].a = "jump";
       F[F_count].b = "-";
       F[F_count].c = "-";
       std::stringstream ss;
       ss<<attr[G[u][0]].codebegin;
       string s = ss.str();
       F[F_count].d = s;
       //cout<<'#'<<F_count<<endl;
       F_count++;
       backpatch(attr[G[u][2]].chain,attr[G[u][0]].codebegin);//
    }
    else if(symbol[u] == "3")
    {
        attr[u].chain = attr[G[u][2]].chain;
        backpatch(attr[G[u][0]].chain,F_count);
    }
    else if(symbol[u] == "4")
    {
        attr[u].chain = attr[G[u][1]].chain;
    }
    else if(symbol[u]=="5")
    {
        if(lookup(attr[G[u][1]].name) == 1)//已存在
        {
            cout<<"重复定义"<<endl;
            return;
        }
        else
        {
            attr[u].chain = 0;
            table_z[table_z_count] = attr[G[u][1]].name;
            table_z_count++;
        }

    }
    else if(symbol[u]=="6")
    {
        attr[u].chain = 0;
    }
    else if(symbol[u]=="7")
    {
        backpatch(attr[G[u][2]].tru_e,F_count);
        attr[u].chain = attr[G[u][2]].fals_e;
    }
    else if(symbol[u]=="8")
    {
        attr[u].codebegin = attr[G[u][0]].codebegin;
        backpatch(attr[G[u][2]].tru_e,F_count);
        attr[u].chain = attr[G[u][2]].fals_e;
    }
    else if(symbol[u]=="9")
    {
        attr[u].codebegin = attr[G[u][0]].codebegin;
        backpatch(attr[G[u][4]].tru_e,F_count-1);
        attr[u].chain = attr[G[u][4]].fals_e;
    }
    else if(symbol[u]=="10")
    {
        if(lookup(attr[G[u][1]].name)==1)
        {

            cout<<"重复定义"<<endl;
            return;
        }

        else
        {
            attr[u].chain = 0;
            F[F_count].a = ":=";
             F[F_count].b = attr[G[u][3]].val;
            F[F_count].c = "-";
            F[F_count].d = attr[G[u][1]].name;
            //cout<<'#'<<F_count<<endl;
            F_count++;
            table_z[table_z_count] = attr[G[u][1]].name;
            table_z_count++;
        }

    }
    else if(symbol[u]=="11")
    {
        if(lookup(attr[G[u][1]].name)==1)
        {
            cout<<"重复定义"<<endl;
            return;
        }

        else
        {
            attr[u].chain = 0;
            F[F_count].a = ":=";
                F[F_count].b = attr[G[u][3]].val;
            F[F_count].c = "-";
            F[F_count].d = attr[G[u][1]].name;
            //cout<<'#'<<F_count<<endl;
            F_count++;
            table_z[table_z_count] = attr[G[u][1]].name;
            table_z_count++;
        }
    }
    else if(symbol[u] == "12")
    {
        if(lookup(attr[G[u][0]].name)==0)
        {
            cout<<"定义为空"<<endl;
            return;
        }

        else
        {
            attr[u].chain = 0;
            F[F_count].a = ":=";
            F[F_count].b = attr[G[u][2]].place;
            F[F_count].c = "-";
            F[F_count].d = attr[G[u][0]].name;
            //cout<<'#'<<F_count<<endl;
            F_count++;
            attr[u].chain = 0;
            table_z[table_z_count] = attr[G[u][1]].name;
            table_z_count++;
        }
    }
    else if(symbol[u]=="13")
    {

        attr[u].place = newtemp();
        F[F_count].a = "+";
        F[F_count].b = attr[G[u][0]].name;
        F[F_count].c = attr[G[u][2]].place;
        F[F_count].d = attr[u].place;
        //cout<<'#'<<F_count<<endl;
        F_count++;
    }
    else if(symbol[u]=="14")
    {
        attr[u].place = newtemp();
        F[F_count].a = "*";
        F[F_count].b = attr[G[u][0]].name;
        F[F_count].c = attr[G[u][2]].place;
        F[F_count].d = attr[u].place;
        //cout<<'#'<<F_count<<endl;
        F_count++;
    }
    else if(symbol[u]=="15")
    {
        attr[u].place = newtemp();
        F[F_count].a = "-";
        F[F_count].b = attr[G[u][0]].name;
        F[F_count].c = attr[G[u][2]].place;
        F[F_count].d = attr[u].place;
        //cout<<'#'<<F_count<<endl;
        F_count++;
    }
    else if(symbol[u] == "16")
    {
        attr[u].codebegin = F_count;
        attr[u].tru_e = F_count;
        attr[u].fals_e = F_count+1;
        F[F_count].a = ">j";
        F[F_count].b = attr[G[u][0]].name;
        F[F_count].c = attr[G[u][2]].val;
        F[F_count].d = "0";
        //cout<<'#'<<F_count<<endl;
        F_count++;
        F[F_count].a = "jump";
        F[F_count].b = "-";
        F[F_count].c = "-";
        F[F_count].d = "0";
        //cout<<'#'<<F_count<<endl;
        F_count++;
    }
    else if(symbol[u] == "17")
    {
        attr[u].codebegin = F_count;
        attr[u].tru_e = F_count;
        attr[u].fals_e = F_count+1;
        F[F_count].a = "<j";
        F[F_count].b = attr[G[u][0]].name;
        F[F_count].c = attr[G[u][2]].val;
        F[F_count].d = "0";
        //cout<<'#'<<F_count<<endl;
        F_count++;
        F[F_count].a = "jump";
        F[F_count].b = "-";
        F[F_count].c = "-";
        F[F_count].d = "0";
        //cout<<'#'<<F_count<<endl;
        F_count++;
    }
    else if(symbol[u] == "18")
    {
        attr[u].codebegin = attr[G[u][1]].codebegin;
        backpatch(attr[G[u][1]].fals_e,attr[G[u][6]].tru_e);
        attr[u].tru_e = merger(attr[G[u][1]].tru_e,attr[G[u][6]].tru_e);
        attr[u].fals_e = attr[G[u][6]].fals_e;
    }
    else if(symbol[u] == "19")
    {
        attr[u].codebegin = attr[G[u][1]].codebegin;
        backpatch(attr[G[u][1]].tru_e,attr[G[u][6]].codebegin);
        attr[u].tru_e = attr[G[u][6]].tru_e;
        attr[u].fals_e = merger(attr[G[u][1]].fals_e,attr[G[u][6]].fals_e);
    }
    else if(symbol[u] == "20")
    {
        attr[u].codebegin = F_count;
        attr[u].tru_e = F_count;
        attr[u].fals_e = F_count+1;
        F[F_count].a = "!=j";
        F[F_count].b = attr[G[u][0]].place;
        F[F_count].c = attr[G[u][2]].place;
        F[F_count].d = "0";
        //cout<<'#'<<F_count<<endl;
        F_count++;
        F[F_count].a = "jump";
        F[F_count].b = "-";
        F[F_count].c = "-";
        F[F_count].d = "0";
        //cout<<'#'<<F_count<<endl;
        F_count++;
    }
    else if(symbol[u] == "21")
    {
        attr[u].codebegin = F_count;
        attr[u].tru_e = F_count;
        attr[u].fals_e = F_count+1;
        F[F_count].a = "==j";
        F[F_count].b = attr[G[u][0]].place;
        F[F_count].c = attr[G[u][2]].place;
        F[F_count].d = "0";
        //cout<<'#'<<F_count<<endl;
        F_count++;
        F[F_count].a = "jump";
        F[F_count].b = "-";
        F[F_count].c = "-";
        F[F_count].d = "0";
        //cout<<'#'<<F_count<<endl;
        F_count++;
    }
    else if(symbol[u]=="22")
    {
        attr[u].place = newtemp();
        attr[u].chain = 0;
        F[F_count].a = "+";
        F[F_count].b = attr[G[u][0]].name;
        F[F_count].c = "1";
        F[F_count].d = attr[u].place;
        //cout<<'#'<<F_count<<endl;
        F_count++;
    }
    else if(symbol[u]=="23")
    {
        attr[u].place = attr[G[u][0]].name;
        attr[u].chain = 0;
        F[F_count].a = ":=";
        F[F_count].b = attr[G[u][0]].name;
        F[F_count].c = "-";
        F[F_count].d = attr[u].place;
        //cout<<'#'<<F_count<<endl;
        F_count++;
    }
    else if(symbol[u]=="24")
    {
       attr[u].place = newtemp();
       attr[u].chain = 0;
        F[F_count].a = ":=";
        F[F_count].b = attr[G[u][0]].val;
        F[F_count].c = "-";
        F[F_count].d = attr[u].place;
        //cout<<'#'<<F_count<<endl;
        F_count++;
    }
    else if(symbol[u]=="25")
    {
        attr[u].chain = F_count;
    }
    else if(symbol[u] == "26")
    {
        attr[u].codebegin = F_count;
    }
    else if(symbol[u] == "27")
    {
        attr[u].codebegin = F_count;
    }
    else if(symbol[u] == "28")
    {
        attr[u].codebegin = F_count;
        attr[u].tru_e = F_count;
        attr[u].fals_e = F_count+1;
        F[F_count].a = "==j";
        F[F_count].b = attr[G[u][0]].name;
        F[F_count].c = attr[G[u][3]].val;
        F[F_count].d = "0";
        //cout<<'#'<<F_count<<endl;
        F_count++;
        F[F_count].a = "jump";
        F[F_count].b = "-";
        F[F_count].c = "-";
        F[F_count].d = "0";
        //cout<<'#'<<F_count<<endl;
        F_count++;
    }
    else if(symbol[u] == "29")
    {
       if(lookup(attr[G[u][1]].name)==1)
        {

            cout<<"重复定义"<<endl;
            return;
        }

        else
        {
            attr[u].chain = 0;
            F[F_count].a = ":=";
            F[F_count].b = attr[G[u][3]].name;
            F[F_count].c = "-";
            F[F_count].d = attr[G[u][1]].name;
            //cout<<'#'<<F_count<<endl;
            F_count++;
            table_z[table_z_count] = attr[G[u][1]].name;
            table_z_count++;
        }
    }
    else if(symbol[u] == "30")
    {
        attr[u].chain = 0;
        F[F_count].a = "-";
        F[F_count].b = attr[G[u][4]].val;
        F[F_count].c = "-";
        F[F_count].d = attr[G[u][1]].name;
        F_count++;
        table_z[table_z_count] = attr[G[u][1]].name;
        table_z_count++;
    }//end if
}







