#include "main.h"
neww M[MAX];//全局变量
vector <quaternion> Q[MAX];
int Q_count = 0;
int ccscouont = 0;//文件行数，即：产生式条数
int I_count = 0;//状态个数
CSS css[MAX_Count];//产生式
VN_Set VN_First[MAX_Count];//非终结符集的First集合
string VT;//终结符集合
string VN;//非终结符集合
map<int, string> GOTO[MAX_Count];
map<int, string> ACTION[MAX_Count];
vector<CSS_LR1> I[MAX_Count]; //项目集

extern vector<int> G[1000];        //用邻接表存Tree
int cnt = 0;                //Tree的节点数
extern vector <quaternion> attr;            //每个节点对用的属性，类型是E
extern vector<string> symbol;    //每个节点对应的符号名字

bool cmp_vector(vector<CSS_LR1>& v1, vector<CSS_LR1>& v2)//v1、v2容器 动态空间储存 判断项目及是否存在
 {
	 if (v1.size() != v2.size()) return false;
	 int i;
	 for (i = 0; i < v2.size(); i++)
	 {
		 CSS_LR1 t;
		 t = v2[i];
		 vector<CSS_LR1>::iterator result = find(v1.begin(), v1.end(), t); //查找3
		 if (result == v1.end()) //没找到
			 return false;
	 }
	 return true;
 }
void read_the_text3()//读出文件 按行读 第一行：非终结符 第二行之后：产生式

{
    int mm = 0;
    int i = 0;
    int j = 0;
    int k = 0;
    ifstream istr;
    string temp;
    string a;
    vector<string> veci;
    istr.open("semantic.txt");
    string strline;
    //读取文件第一行的内容
    getline(istr,strline);
    mm=strline.length();
    VT = strline;
    //cout<<"终结符"<<':'<<VT<<endl;
    //读取表达式内容
    while(getline(istr,strline))
    {
        mm = strline.size();
        css[i].start = strline[0];
        if(VN.find(css[i].start) == -1)//还没存入
        {
            int z = VN.size();
            VN.insert(z,css[i].start);//在VN末尾插入一个字符
            VN_First[z].VN_name = css[i].start;
        }
        css[i].next = strline.substr(3);//字符串的截取
        cout<<"\n\n___产生式"<<i<<':'<<css[i].start<<"->"<<css[i].next<<endl;
        i++;
    }
    ccscouont = i;
//    cout<<"产生式个数："<<ccscouont<<endl;
//    cout<<"非终结符："<<VN<<endl;
    istr.close();
}
string first(string a)//a为非终结符求first集合
{
    int i = 0;
    int j = 0;
    int k = 0;//第几个非终结符
    k = VN.find(a);
    for(i = 0;i < ccscouont;i++)//每一条每一条地找
    {
        if(css[i].start == a)//a是文法左部
        {
            for(j = 0;j < css[i].next.size();j++)//文法右部 一个字符一个字符地找
            {
                if(VT.find(css[i].next[j]) <100 )//遇到非终结符 && VN.find(css[i].next[j]) == -1
                {
                    if(VN_First[k].FIRST.find(css[i].next[j]) >100)//该终结符在该非终结符的first集合中没有出现过
                    {
                        int z = VN_First[k].FIRST.size();//在末尾插入终结符
                        char b = css[i].next[j];
                        VN_First[k].FIRST.insert(z,1,b);//在first末尾加一个b字符
                    }
                    break;
                }
                else if(VN.find(css[i].next[j]) <100 && VT.find(css[i].next[j]) >100)//非终结符开头
                {
                    if(css[i].next[j] == css[i].start[0])//是非终结符本身，不需要新的元素进入first集
                    {
                        break;//找下一条规则
                    }

                    //先求出该终结符的first集
                    string D;
                    string c = "a";
                    char d[2] = {css[i].next[j],0};
                    c = d;
                    D = first(c);//求出该非终结符的first集合
                    int e = 0;
                    int m = 0;
                    for(e = 0; e < D.size(); e++)
                    {
                        int z = 0;
                        z = VN_First[k].FIRST.size();

                        if(VN_First[k].FIRST.find(D[e])>100)//没有重复
                        {
                            char b = D[e];
                            VN_First[k].FIRST.insert(z,1,b);//把该非终结符的first存入
                            m = 1;
                        }
                    }
                    //判断该非终结符能否推空
                    if(D.find('^')!=-1)//能推空 继续向下一个非终结符找
                    {
                        if(j != css[i].next.size() - 1)//没有走到文法右部最后一个字符
                        {
                            //删除^
                            int q = 0;
                            q = VN_First[k].FIRST.find('^');
                            VN_First[k].FIRST.erase(q,1);//字符的删除 q:位置 1：长度
                        }
                    }
                    else //不能推空
                    {
                       break;
                    }
                }
            }
        }
    }
    return VN_First[k].FIRST;
}
void get_first()
{
    int a;
    a = VN.size();
    int i = 0;
    string b = "a";
    for(i = 0;i < a;i++)
    {
        char c[2] = {VN[i],0};
        b = c;
        first(b);
    }
//    for(i = 0;i < a;i++)
//    {
//        cout<<VN[i]<<"的first集："<<VN_First[i].FIRST<<endl;
//    }
}


int find_in_vector(vector<CSS_LR1> T, CSS_LR1 p)//确保没有重复出现
{
	 vector<CSS_LR1>::iterator it;
	 for (it = T.begin(); it != T.end(); it++) {
		 if (*it == p) {
			 return 1;//容器中已经存在了
		 }
	 }
	 return 0;//容器中没有存在
 }

void showI(vector<CSS_LR1> I) {//展示项目集
	 vector<CSS_LR1>::iterator it;//实现容器I中的元素遍历
	 for (it = I.begin(); it != I.end(); it++) {
		 CSS_LR1 p = *it;
		 cout << p.start << "->";
		 vector<string>::iterator s;
		 for (int j = 0; j < p.next.size(); j++) {
			 if (j == p.num) cout << ".";
			 cout << p.next[j];

		 }if (p.num == p.next.size())cout << ".";
		 cout << ",";
		 for (int k = 0; k < p.tail.size(); k++) {
			 cout << p.tail[k]<<' ';
		 }
	 }

 }
vector<CSS_LR1> CLOSURE(CSS_LR1 I,vector<CSS_LR1> t5) //求闭包 某状态中一条文法表达式的闭包
{
   //cout<<2<<endl;
   vector<CSS_LR1> T;
    if(I.num >= I.next.size())//规约项目或接受项目
    {
       // cout<<4<<endl;
        return T;//出口
    }
    else//待约项目或移进项目
    {
        string a;
        char b[2] = {I.next[I.num],0};
        a = b;//待移进的字符
        if(VT.find(a) < 100)//移进项目 不会有新的符号需要被分析 终结符
        {
                return T;//出口
        }

        else//待约项目 有新的项目需要被分析
        {
            int i = 0;
            for(i = 0;i<ccscouont;i++)//一条文法一条文法的找文法右部等待约项目的非终结符的产生式
            {
                if(css[i].start == a)//找到
                {
                    CSS_LR1 p;//新建一个文法表达式的闭包
                    p.start = css[i].start;
                    p.num = 0;
                    p.next = css[i].next;
                    string f1;
                    int j = 0;
                    for(j = I.num+1;j<I.next.size();j++)
                    {
                        string f2;//暂存first集合
                        string c;
                        char d[2] = {I.next[j],0};
                        c = d;
                        if(VT.find(d)<100)//点后边为终结符
                        {
                            int z = 0;
                            z = f1.size();
                            f1.insert(z,1,I.next[j]);//把非终结符存入

                            break;
                        }
                        else//点后边为非终结符
                        {
                            f2 = first(d);//求出该非终结符的first集合
                        }
                        int y = 0;
                        y = f1.size();
                        f1.insert(y,f2);
                        if(f2.find('^') > 100)//不能推空 停止寻找继续向前搜索符
                        {
                            break;
                        }
                    }

                    if(f1.size() == 0)//没有新的first集合
                    {
                        p.tail = I.tail;//把原来的向前搜索符复制下来
                    }
                    else
                    {
                        if(f1.find('^') < 100)//能推空 向前搜索符不能为空
                        {
                            int q = 0;
                            q = f1.find('^');//找到^的位置
                            f1.erase(q,1);//删除位置：q 删除个数：1
                            //在p.tail末尾插入原本的向前搜索符
                            int x = 0;
                            x = f1.size();
                            f1.insert(x,I.tail);
                        }
                        p.tail = f1;
                    }
                    if(find_in_vector(t5,p) == 0)//容器中不存在对该条表达式的分析
                    {
                        T.push_back(p);//在容器T末尾处添加元素p
                        vector<CSS_LR1> o = CLOSURE(p,t5);
                        vector<CSS_LR1>::iterator z;//迭代器，z的大小可以根据需要改变 z类型为CSS_LR1 实现元素遍历
                        for(z = o.begin();z != o.end();z++)
                            {
                                if(find_in_vector(t5,*z) == 0)
                                {
                                   T.push_back(*z);
                                }
                                else
                                    goto p1;
                            }
                    }
                    else if(find_in_vector(t5,p) == 1)
                        goto p1;
                }
            }
        }
    }
    p1:
    return T;
}

void printclosure()//求所有LR文法分析表
{
  //cout<<1<<endl;
    CSS_LR1 p;
    int i = 0;
    vector<CSS_LR1> T;
    char a[3] = {'Z','`',0};
    string b;
    b = a;
    p.start = a;
    p.next = css[0].start;
    p.num = 0;
    int z = 0;
    z = p.tail.size();
    p.tail.insert(z,1,'#');
    //cout <<"p.start:"<<p.start<< "   p.tail:" << p.tail << endl;//z^,#
    I[0] = CLOSURE(p,T);
    I[0].insert(I[0].begin(), p);//在I[0]开头插入第一个新加的状态

//  cout << p.start << "  " << p.next << "  " << p.num << "  " << p.tail << endl;

    I_count  = 1;
    for(i = 0; i < I_count; i++)
    {
        //对新状态中的每一个文法表达式都要求closure
        vector<CSS_LR1>::iterator t;//定义一个过渡的状态 实现I[i]元素遍历

        //求状态i中的 规约项目 构建action表
        for(t = I[i].begin(); t!=I[i].end(); t++) //在状态I[i]中一条文法表达式一条文法表达式地找
        {
            int num = 0;
            CSS_LR1 t2;//用t2指针调用结构体
            t2 = *t;//调用一次就是调用一次文法表达式
            if(t2.num == t2.next.size() || t2.next[t2.num] == '^')//规约项目
            {
                int bb = 0;
                for(bb = 0; bb < ccscouont; bb ++)
                {
                    if(css[bb].start == t2.start && css[bb].next == t2.next)
                    {
                        num = bb+1;//找到是第几条文法表达式，即：规约到第一条文法表达式
                        //+1：新增了一条文法表达式，最开始的一条文法表达式
                        break;
                    }//end if
                }//end for
                std::stringstream ss;//自动分配储存空间 自动选择所需转换 进行输入输出操作
                ss<<num;//把num值传递入ss中 把int类型转换为string类型
                string s;
                s = ss.str();//把ss中的内容变成string类型赋值给s
                int k = 0;
                for(k = 0; k < t2.tail.size(); k++)//向前搜索符可能不止一个
                {
                   ACTION[i][t2.tail[k]] = "r"+s;

                 //  cout<<"ACTION[i][t2.tail[k]]:"<<ACTION[i][t2.tail[k]]<<endl;  表达式编号对应的为r几
               //  cout<<"t2.tail[k]:"<<t2.tail[k]<<endl;

                }//end for
                if(t2.num == 1 && t2.next[0] == css[0].start[0])//接受项目
                {
                    ACTION[i]['#'] = "acc";
                   // cout<<"ACTION[i]['#']:"<<ACTION[i]['#']<<endl;

                }//end if
            }//end if
        }//end for
        int x = 0;
        x = VN.size();
        int j = 0;

        for(j = 0; j < x; j++) //找非终结符
        {
            vector<CSS_LR1> temp;//过渡

            int k = 0;

            for(k = 0; k<I[i].size(); k++)//I[i].size()指该状态下有几条文法表达式
            {
                CSS_LR1 lr = I[i][k];//找状态i中的第k个文法表达式
                //cout<<'&'<<i<<k<<'&'<<endl;
                if(lr.num < lr.next.size() && lr.next[lr.num] == VN[j])//找到了待约项目 只针对一个带规约符号
                {
                   // cout<<1<<endl;
                    vector<CSS_LR1> t2;
                    vector<CSS_LR1> t7;
                    vector<CSS_LR1>::iterator zz;
                    lr.num = lr.num+1;//移进下一个符号
                    vector<CSS_LR1> T1;
                    t2 = CLOSURE(lr,temp);
                    T1 = t2;
                    t2.push_back(lr);//在t2末尾加入lr
                    //判断t2中的元素在temp中是否出现过
                    for(zz = t2.begin();zz!=t2.end();zz++)
                    {
                        if(find_in_vector(temp,*zz) == 0)
                            t7.push_back(*zz);//end if
                    }//end for
                    temp = temp + t7;//新的状态  vector<CSS_LR1>::iterator sss;

                }//end if
            }//end for

            //遇到非终结符构建GOTO表
            if(temp.size() > 0)
            {
                int w = 0;
                for(w = 0; w < I_count ; w++)
                {
                    if(cmp_vector(I[w],temp))//项目集存在
                    {
                        break;//不增添新状态
                    }//end if
                }//end for
                if(w == I_count)//状态没有出现过
                {
                    I[I_count] = temp;//添加新状态
                    //cout<<'I'<<i<<"--"<<VN[j]<<"->"<<'I'<<I_count<<endl;
                    std::stringstream ss;
                    ss<<I_count;
                    string s = ss.str();
                    GOTO[i][VN[j]] = s;
                    I_count ++;
                }
                else//状态已经出现过
                {
                    //cout<<'I'<<i<<"--"<<VN[j]<<"->"<<'I'<<w<<endl;
                    std::stringstream ss;
                    ss<<w;
                    string s = ss.str();
                    GOTO[i][VN[j]] = s;//
                }//end if
            }//end if
        }//end for

        int r = 0;
        r = VT.size();
        int h = 0;
        for(h = 0; h < r; h++) //在 终结符集合 中依次寻找
        {
            vector<CSS_LR1> t4;
            int v = 0;
            int vv = I[i].size();
            for(v = 0; v<vv; v++)//一条文法一条文法地找
            {
                CSS_LR1 zr = I[i][v];
                if(zr.num < zr.next.size() && zr.next[zr.num] == VT[h] && zr.next[zr.num] != '^')//找到了移进项目 且推出的不为空
                {
                    vector<CSS_LR1> t5;
                    vector<CSS_LR1> t6;
                    vector<CSS_LR1>::iterator z;
                    //cout<<zr.start<<"->"<<zr.next<<','<<zr.tail<<'%'<<endl;
                    zr.num++;
                    //vector<CSS_LR1>::iterator ssss;
                    vector<CSS_LR1> T2;
                    t5 = CLOSURE(zr,t4);//确保该文法产生式中没有重复的//要解决的问题：不同文法表达式的产生式不能有重复的
                    T2 = t5;
                    t5.insert(t5.begin(),zr);//对当前状态的运算
                    for(z = t5.begin();z!=t5.end();z++)
                    {
                        if(find_in_vector(t4,*z) == 0)
                            t6.push_back(*z);//end if
                    }//end for
                    t4 = t4+t6;//t4是一个新的项目集
                }//end if
            }//end for

            //遇到终结符 构建action表
            if(t4.size()>0)
            {
                int aa = 0;
                for(aa = 0; aa<I_count; aa++) //找新的项目集是否已存在
                {
                    if(cmp_vector(I[aa],t4))
                    {
                        break;
                    }//end if
                }//end for
                if(aa == I_count)//不存在更新分析表
                {
                    I[I_count] = t4;
                    //cout<<'I'<<i<<"--"<<VT[h]<<"->"<<'I'<<I_count<<endl;
                    std::stringstream ss;
                    ss<<I_count;
                    string s = ss.str();
                    ACTION[i][VT[h]] = "S"+s;
                    I_count ++;//更新
                }
                else
                {
                    //cout<<'I'<<i<<"--"<<VT[h]<<"->"<<'I'<<aa<<endl;
                    std::stringstream ss;
                    ss<<aa;
                    string s = ss.str();
                    ACTION[i][VT[h]] = "S"+s;
                }//end if
            }//end if
        }//end for
    }//end for
}

void analysis()//对字符串进行分析
{
    //vector <string> print;//输入串
    vector <string> actionn;//action栈
    vector <string> gotoo;//goto栈
    vector <int> state;//状态栈
    vector <string> sign;//符号栈
    vector <int> num;//记录状态数的位数
    string input;
    int step = 1;//步骤
    ifstream infile;
    infile.open("process.txt");
    getline(infile, input);

    //0号状态入状态栈
    //#入符号栈
    state.push_back(0);
    sign.push_back("#");
    //开始分析字符串
    int a = input.size();//a为字符串长度
    input = input + "#";
    string inputt;
    inputt = input;
    int j = 0;
    int i = 0;
    quaternion g;//结点
    stack <int> S;//生成树
   // E g;
    cout<<"步骤"<<'\t'<<"状态栈"<<'\t'<<"符号栈"<<'\t'<<"输入串"<<setw(27)<<"ACTION"<<setw(27)<<"GOTO"<<endl;
    while(i <= a)
    {
        string s;//要进入的已经简化的字符
        char b[2] = {input[i],0};
        s = b;
        int statee = state.back();//状态栈传回最后一个数据
        //状态栈出栈、串输入栈出栈，在分析表寻找

        if(ACTION[statee][s[0]] == "acc")//分析成功
        {
             cout<<step<<'\t';
            vector<string> :: iterator it;
            vector<int> :: iterator itt;
            for(itt = state.begin(); itt!=state.end(); itt++)
                cout<<*itt;
            cout<<'\t';
            for(it = sign.begin(); it!=sign.end(); it++)
                cout<<*it;
            cout<<'\t';
                cout<<inputt;
            cout<<'\t'<<"分析成功！"<<endl;
             break;//分析结束
        }//end if
        string c;
        c = ACTION[statee][s[0]];
        int d = c.size();


        if(d == 0)//分析表为空，分析失败
        {
            cout<<step<<'\t';
            vector<string> :: iterator it;
            vector<int> :: iterator itt;
            for(itt = state.begin(); itt!=state.end(); itt++)
                cout<<*itt;
            cout<<'\t';
            for(it = sign.begin(); it!=sign.end(); it++)
                cout<<*it;
            cout<<'\t'<<'\t';
                cout<<inputt;
            cout<<'\t'<<"分析失败！"<<endl;
            cout<<"正确应为："<<endl;
            int c = state.back();
            for(int v = 0;v<VT.size();v++)
                {string m;
                 m = ACTION[statee][VT[v]];
                int dd = m.size();
                if(dd != 0)
                    cout<<VT[v]<<' ';}//end for
            cout<<endl;
            break;
        }
        else if(d != 0)//分析表不为空
        {
            //要分析的状态栈的内容和要分析的输入串栈的内容
            string now;
            now = s;
            //要进入符号栈的是终结符
                string e = ACTION[statee][now[0]];

                if(e[0] == 'S')//是移进项目，要进入符号栈的是终结符
                {
                    //把第i个结点的属性存入结点
                    quaternion g;//结点
                    if(M[i].pred == 2)//是字符，存入字符的名称
                    {
                        g.name = M[i].language;
                    }

                    else if(M[i].pred == 3)//是变量，存入变量数值
                    {
                        g.val = M[i].language;
                    }

                    else if(M[i].pred == 4 || M[i].pred == 1)//是标识符或符号，存入标识符或符号本身
                    {
                        g.type = M[i].language;
                    }

                    symbol.push_back(M[i].predd);//结点的符号名字
                    S.push(Q_count++);
                    attr.push_back(g);

                    cout<<step<<'\t';
                    vector<string> :: iterator it;
                    vector<int> :: iterator itt;
                    for(itt = state.begin();itt!=state.end();itt++)//输出状态
                        cout<<*itt;
                    cout<<'\t';
                    for(it = sign.begin();it!=sign.end();it++)//输出符号
                        cout<<*it;
                    cout<<'\t'<<'\t';
                    actionn.push_back(e);//进入action栈
                    sign.push_back(now);
                    i++;
                    int o = 0;
                    for(int y = 1;y < e.length();y++)
                    {
                        o*=10;
                        o+=(e[y]-'0');
                    }//endfor
                    int z = e.length()-1;
                    num.insert(num.end(),1,z);
                    state.push_back(o);
                        cout<<inputt;//输出 输入串
                    cout<<'\t'<<'\t';
                    cout<<e;//输出  动作
                    cout<<'\t';
                    cout<<endl;
                    step++;
                    inputt.erase(0, 1);
                }
                else if(e[0] == 'r')//规约
                {
                   cout<<step<<'\t';
                    vector <string> :: iterator it;
                    vector <int> :: iterator itt;
                    for(itt = state.begin();itt!=state.end();itt++)
                        cout<<*itt;
                    cout<<'\t';
                    for(it = sign.begin();it!=sign.end();it++)
                        cout<<*it;
                    cout<<'\t'<<'\t';
                    string f;
                    actionn.push_back(e);
                    int pp = 0;
                    for(int y = 1;y < e.length();y++)
                    {
                        pp*=10;
                        pp+=(e[y]-'0');
                    }//end for
                    f = css[pp-1].start;//pp-1为用第几条产生式进行规约

                    int gg = css[pp-1].next.size();
                    int h = 0;
                    queue <int> son;//使用队列 front()访问队首元素 back()访问队尾元素
                    //son记录出站的结点
                    for(h = 0;h<gg;h++)
                    {
                        int l = num.back();
                        state.pop_back();//由于规约状态出栈
                        sign.pop_back();
                        num.pop_back();
                        son.push(S.top());
                        S.pop();//结点出栈
                    }//end for
                    int j = state.back();//状态栈栈顶元素
                    sign.push_back(f);// 规约后的状态 入栈

                    std::stringstream ss;
                    ss<<pp-1;
                    string s = ss.str();

                    symbol.push_back(s);// 规约的符号 入栈
                    attr.push_back(g);
                    S.push(Q_count++);

                    while(!son.empty())//构建语法树
                    {
                        G[Q_count-1].push_back(son.front());
                        son.pop();
                    }

                    string k = GOTO[j][f[0]];
                    int o = 0;
                    for(int y = 0;y < k.length();y++)
                    {
                        o*=10;
                        o+=(k[y]-'0');
                    }//end for
                    gotoo.push_back(k);
                    state.push_back(o);
                    cout<<inputt;//输出 输入串
                    cout<<'\t'<<'\t';
                    cout<<e;//输出 ACTION
                    cout<<'\t';
                    cout<<k;//输出 GOTO
                    cout<<endl;
                    step++;
                }//endif
            }//endif
            //要入栈的是非终结符

    }//endwhile
    cout<<endl;
   // cout<<"语法树："<<endl;
    for(int i = 0;i<Q_count;i++)
    {
        if(!G[i].empty())
        {
           // cout<<'('<<i<<','<<symbol[i]<<')'<<"---->";
            sort(all(G[i]));
         //   for(int j = 0;j<G[i].size();j++)
          //  cout<<'('<<G[i][j]<<','<<symbol[G[i][j]]<<')'<<' ';
          //  cout<<endl;
        }
    }
    cout<<endl<<endl;
    quater(Q_count-1);

   // cout<<"\n\nQ_count:"<<Q_count<<endl;

    emit();
}
void print_line() {
	 cout << "--------------" << endl;
 }
void print_ACTION_GOTO() {//在excel表格上打印出 分析表
	 set<string>::iterator it;
	 print_line();
	 cout << setw(27) << setiosflags(ios::right) << "ACTION";
	 cout << setw(250) << setiosflags(ios::left) << "  GOTO" << endl;
	 print_line();
	 cout << setw(8) << "项目集" << "|";

	 for (int i = 0; i < VT.size(); i++) {
		 cout << setw(8) << VT[i] << "|";

	 }
	 cout << setw(8) << "#" << "|";
	 for (int i = 0; i< VN.size(); i++) {
		 cout << setw(8) << VN[i] << "|";

	 }
	 cout << endl;
	 int j = 0;
	 int i = 0;
	 for ( j = 0; j < I_count; j++) {
		 cout << setw(6) << "I" << setw(2) << j << "|";
		 for (i = 0; i< VT.size(); i++) {
			 cout << setw(8) << ACTION[j][VT[i]] << "|";

		 }
		 cout << setw(8) << ACTION[j]['#'] << "|";
		 for (i = 0; i< VN.size(); i++) {

			 if (GOTO[j][VN[i]].size() != 0)//GOTO表为0
				 cout << setw(8) << GOTO[j][VN[i]] << "|";
			 else {
				 cout << setw(8) << " " << "|";
			 }
		 }
		 cout << endl;
	 }
	 print_line();
 }
