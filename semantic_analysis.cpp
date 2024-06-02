#include "main.h"

extern four F[MAX];//ÿ����Ԫʽ��Ϣ
extern int F_count;//��Ԫʽ����
vector<int> G[1000];//
vector<string> symbol;
vector <quaternion> attr;
extern vector <quaternion> Q[MAX];//ÿ���ڵ���Ϣ

string table_z[MAX];//���ű� ������
int table_z_count = 0;
string table_h[MAX];//���ű� ������
int table_h_count = 0;
int new_w = 0;//�µ���ʱ�����ĸ���


int lookup(string a)//�ڷ��ű��в���
{
    int i = 0;
    for(i = 0;i<table_z_count;i++)
    {
        if(table_z[i] == a) return 1;//�Ѵ���
    }
    return 0;//������
}

void backpatch(int p,int t)//����� ������P�����ӵ�ÿһ����Ԫʽ�ĵ������ζ���Ϊת��Ŀ��t �����δ���ɣ�ȴ��Ҫ
{
    std::stringstream z;
    z<<t;
    string m = z.str();
    if((F[p].d[0] >= '9' && F[p].d[0] <= '0') || m == F[p].d) //�������β������֣�������������
    {
        return;
    }
    else if(F[p].d[0] <= '9' && F[p].d[0] >= '0')//�������������֡�����������
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
int merger(int p1,int p2)//�������� ��P2Ϊ�գ�����P1��P2��Ϊ�գ�����P2����P2����β��������ΪP1
{

    if(p2 == 0)
        return p1;
    else if(p2 != 0 )
    {
        //��p1ת��string
        std::stringstream ss;
        ss<<p1;
        string s = ss.str();
        F[p2].d = s;
       //��p2��������β��Ϊp1
        return p2;
    }
}
void emit()//�����Ԫʽ
{
    int i;
    cout<<"��Ԫʽ:"<<endl;
    for(i = 100;i < F_count;i++)
        cout<<i<<' '<<'('<<F[i].a<<','<<F[i].b<<','<<F[i].c<<','<<F[i].d<<')'<<endl;
    cout<<endl;
}
string newtemp()//�����µ���ʱ����
{
    string temp = "t";
    std::stringstream ss;
    ss<<new_w;
    string s = ss.str();
    temp.insert(temp.size(),s);
    new_w++;
    return temp;
}
//���������Ҫ����
void quater(int u)//�ӵ�u����㿪ʼ���±����﷨��������ÿ����㣬ִ����Ӧ���嶯��
{
    if (G[u].empty())
    {
        return;//�ý��Ϊ��ֱ�ӷ��� ����
    }//end if
    for(int i = 0; i<G[u].size(); i++)
    {
            quater(G[u][i]);//�������еĶ��ӽڵ�
    }//end for

    //���ݲ�ͬ�Ĳ���ʽ���� �ƽ�����Լ
    //cout<<4<<' '<<symbol[u]<<endl;
    if( symbol[u] ==  "0" )//������� �������������ϢΪ0��ʾ�õ�0������ʽ���й�Լ
    {//ִ�е���������ʽ�����嶯��
        int s = attr[G[u][1]].chain;
        attr[u].chain = s;
        string name = attr[G[u][1]].name;//���Ѿ�������ĺ�����������ű�
        table_h[table_h_count] = name;
        table_h_count++;
    }
    else if(symbol[u]=="1")//�����ϢΪ1��ʾ�õ�1������ʽ���й�Լ
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
        if(lookup(attr[G[u][1]].name) == 1)//�Ѵ���
        {
            cout<<"�ظ�����"<<endl;
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

            cout<<"�ظ�����"<<endl;
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
            cout<<"�ظ�����"<<endl;
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
            cout<<"����Ϊ��"<<endl;
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

            cout<<"�ظ�����"<<endl;
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







