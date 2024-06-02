#include "main.h"
vector<int> parted;//入口
int parted_count = 0;//块数
four FF[MAX];//储存被优化后的四元式
int FF_count = 100;four F[MAX];//每条四元式信息
int F_count = 100;//四元式个数



vector <optimize> DAG(int a,int b,vector <optimize> opp,int opp_count);
void simple(vector <optimize> opp);

void emit2()//输出优化后的四元式
{
    int i;
    cout<<"优化后的四元式:"<<endl;
    for(i = 100;i < FF_count;i++)
        cout<<i<<' '<<'('<<FF[i].a<<','<<FF[i].b<<','<<FF[i].c<<','<<FF[i].d<<')'<<endl;
}
int find_parted(int a)//查找a在parted容器中是否存在
{
    vector <int> :: iterator it;
    for(it = parted.begin();it!=parted.end();it++)
    {
        if(*it == a) return 1;//存在
    }
    return 0;//不存在
}
void apart()
{
    parted.insert(parted.end(),100);
    parted_count++;
    int i = 0;
    for(i = 0;i<F_count;i++)
    {
        if(F[i].a[1] == 'j' || F[i].a[2] == 'j')
        {
            if(find_parted(i) == 0)
            {
                parted.insert(parted.end(),i);
                parted_count++;
            }

            //把string转化为数字
            int a = 0;
            for(int y = 0;y < F[i].d.length();y++)
            {
                a*=10;
                a+=(F[i].d[y]-'0');
            }//end for
            if(find_parted(a) == 0)
            {
                parted.insert(parted.end(),a);
                parted_count++;
            }
            if(find_parted(i+1) == 0)
            {
                parted.insert(parted.end(),i+1);
                parted_count++;
            }


        }
        else if(F[i].a == "jump" )
        {
            if(find_parted(i) == 0)
            {
                parted.insert(parted.end(),i);
                parted_count++;
            }
            int a = 0;
            for(int y = 0;y < F[i].d.length();y++)
            {
                a*=10;
                a+=(F[i].d[y]-'0');
            }
            if(find_parted(a) == 0)
            {
                parted.insert(parted.end(),a);
                parted_count++;
            }
        }
    }
    parted_count = parted_count-1;
    sort(parted.begin(),parted.end());
    parted.insert(parted.end(),parted[parted_count]+1);
    work_optimize();
}

void work_optimize()
{
    int i = 0;
    int j = 0;
    for(i = 1;i<=parted_count;i ++)
    {
        vector <optimize> opp;//优化结点信息
        int opp_count = 0;//生成的DAG的节点数
      opp = DAG(parted[i],parted[i+1]-1, opp, opp_count);
     // opp = DAG(parted[i], parted[i+1]-1, 0, opp, opp_count, nullptr);
       simple(opp);
     //simple(opp_count, 0, opp);
        //print_point(i,parted[i],parted[i+1]-1,opp);//输出生成的结点信息
    }

    //跳转语句第四区段的改变
    int a = F_count - FF_count;
    for(i = 0;i<FF_count;i++)
    {
        if(FF[i].a[1] == 'j' || FF[i].a[2] == 'j' ||(FF[i].a=="jump"&&FF[i].d!="0"))
        {
            int m = 0;
            for(int y = 0;y < FF[i].d.length();y++)
            {
                m*=10;
                m+=(FF[i].d[y]-'0');
            }//end for
            m = m - a;
            std::stringstream ss;//自动分配储存空间 自动选择所需转换 进行输入输出操作
            ss<<m;//把num值传递入ss中 把int类型转换为string类型
            string s;
            s = ss.str();
            FF[i].d = s;
        }

    }
    emit2();
}
 void print_point(int a,int b,int c,vector <optimize> opp)
 {
      cout<<"第"<<a<<"块"<<':'<<endl;
      cout<<'('<<b<<'-'<<c<<')'<<endl;
      vector <optimize> :: iterator it;
      int j = 1;
      for(it = opp.begin(); it!=opp.end(); it++)
        {
            optimize p;
            p = *it;
            cout<<"第"<<j<<"个结点信息"<<':'<<endl;
            cout<<"down:"<<p.down<<endl;
            cout<<"left:";
            vector <string> :: iterator itt;
            for(itt = p.left.begin(); itt!=p.left.end(); itt++)
                cout<<*itt<<' ';
            cout<<endl;
            cout<<"son:";
            vector <int> :: iterator ittt;
            for(ittt = p.son.begin(); ittt!=p.son.end(); ittt++)
                cout<<*ittt<<' ';
            cout<<endl;
            cout<<"parent:";
            vector <int> :: iterator itttt;
            for(itttt = p.parent.begin(); itttt!=p.parent.end(); itttt++)
                cout<<*itttt<<' ';
            cout<<endl<<endl;
            j++;
        }
 }
int find_opp(string a,int opp_count,vector <optimize> opp)

{
    for(int i = 0; i<opp_count; i++)
    {
        vector <string> :: iterator it = find(opp[i].left.begin(),opp[i].left.end(),a);
        if(it != opp[i].left.end()) return i;//变量已存在返回节点值
    }
    return -1;//变量不存在
}
int find_parent_son(vector<int> a,vector <int> b)
{
    vector <int> :: iterator it1;
    vector <int> :: iterator it2;
    for(it1 = a.begin();it1!=a.end();it1++)
    {
        for(it2 = b.begin();it2!=b.end();it2++)
        {
            if(*it1 == *it2)//有相同元素返回该元素
                return *it1;
        }
    }
    return -1;//没有相同元素
}
vector <optimize> DAG(int a,int b,vector <optimize> opp,int opp_count)//a、b为两个入口语句
{
    int i = 0;
    for(i = a;i <= b; i++)//遍历一块
    {
        if(F[i].a == ":=")//零型
        {
            int a = find_opp(F[i].b,opp_count,opp);
            if(a == -1)
            {
                optimize g;
                g.down = F[i].b;
                g.left.insert(g.left.end(),F[i].d);
                g.future = 0;
                opp.push_back(g);
                opp_count++;
            }
            else if(a != -1)
            {
               opp[a].left.insert(opp[a].left.end(),F[i].d);
            }

        }
        else if(F[i].a == "+" || F[i].a == "-" || F[i].a == "*" ||F[i].a[1] == 'j'||F[i].a[2] == 'j')//二型
        {
            int b = find_opp(F[i].b,opp_count,opp);
            int c = find_opp(F[i].c,opp_count,opp);
            //在symboll里找有没有要进行运算的点
            if(b!=-1 && c!=-1)//节点都存在
            {
                if(find_parent_son(opp[b].parent,opp[c].parent) != -1)//有共同的父节点并且能合并
                {

                    int a = find_parent_son(opp[b].parent,opp[c].parent);
                    if(opp[a].down == F[i].a)
                        opp[a].left.insert(opp[a].left.end(),F[i].d);
                    else
                        goto p1;
                }

                else //需要建立新的结点
                {
                    p1:
                    optimize g;
                    g.down = F[i].a;
                    g.left.insert(g.left.end(),F[i].d);
                    g.son.insert(g.son.end(),b);
                    g.son.insert(g.son.end(),c);
                    g.future = 1;
                    opp[b].parent.insert(opp[b].parent.end(),opp_count);
                    opp[c].parent.insert(opp[c].parent.end(),opp_count);
                    opp.push_back(g);
                    opp_count++;
                }
            }
            else if(b!=-1 && c == -1)//只有一个结点存在
            {
                opp[b].parent.insert(opp[b].parent.end(),opp_count);//b结点的父节点增加
                optimize g;//新建一个节点c
                g.down = F[i].c;
                g.left.insert(g.left.end(),F[i].c);
                opp.push_back(g);
                opp_count++;
                optimize h;//新建一个节点存新的运算结果
                h.down = F[i].a;
                h.left.insert(h.left.end(),F[i].d);
                h.son.insert(h.son.end(),b);
                h.son.insert(h.son.end(),opp_count-1);
                h.future = 1;
                opp.push_back(h);
                opp[opp_count-1].parent.insert(opp[opp_count-1].parent.end(),opp_count);
                opp_count++;
            }
            else if(b==-1&&c!=-1)//只有一个节点存在
            {
                opp[c].parent.insert(opp[c].parent.end(),opp_count);//c结点的父节点增加
                optimize g;//新建一个节点b
                g.down = F[i].b;
                g.left.insert(g.left.end(),F[i].b);
                opp.push_back(g);
                opp_count++;

                optimize h;//新建一个节点存新的运算结果
                h.down = F[i].a;
                h.left.insert(h.left.end(),F[i].d);
                h.son.insert(h.son.end(),c);
                h.son.insert(h.son.end(),opp_count-1);
                h.future = 1;
                opp.push_back(h);
                opp[opp_count-1].parent.insert(opp[opp_count-1].parent.end(),opp_count);
                opp_count++;
            }
            else if(b==-1&&c==-1)//一个节点都不存在
            {
                optimize g;//新建一个节点b
                g.down = F[i].b;
                g.left.insert(g.left.end(),F[i].b);
                g.parent.insert(g.parent.end(),opp_count+2);
                opp.push_back(g);
                opp_count++;

                optimize h;//新建一个节点b
                h.down = F[i].c;
                h.left.insert(h.left.end(),F[i].c);
                h.parent.insert(h.parent.end(),opp_count+1);
                opp.push_back(h);
                opp_count++;

                optimize k;
                k.down = F[i].a;
                k.left.insert(k.left.end(),F[i].d);
                k.son.insert(k.son.end(),opp_count-1);
                k.son.insert(k.son.end(),opp_count-2);
                k.future = 1;
                opp.push_back(k);
                opp_count++;
            }


            }//二型或四型
            else if(F[i].a == "jump")//六型
            {
                optimize k;
                k.down = F[i].a;
                k.left.insert(k.left.end(),F[i].d);
                k.future = 2;
                opp.push_back(k);
                opp_count++;
            }
    }//end if

        return opp;
}
void simple(vector <optimize> opp)
{
    vector <optimize> :: iterator it;
    for(it = opp.begin();it!=opp.end();it++)
    {
        optimize g;
        g = *it;
        if(g.future == 1)//+-*条件跳转
        {
            FF[FF_count].a = g.down;
            FF[FF_count].b = opp[g.son[0]].down;//0个son节点的第一个string
            FF[FF_count].c = opp[g.son[1]].down;
            FF[FF_count].d = g.left[0];
            FF_count++;
        }
        else if(g.future == 2)//无条件跳转
        {
            FF[FF_count].a = g.down;
            FF[FF_count].b = '-';
            FF[FF_count].c = '-';
            FF[FF_count].d = g.left[0];
            FF_count++;
        }
    }
}
