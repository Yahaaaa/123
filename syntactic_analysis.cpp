#include "main.h"
neww M[MAX];//ȫ�ֱ���
vector <quaternion> Q[MAX];
int Q_count = 0;
int ccscouont = 0;//�ļ���������������ʽ����
int I_count = 0;//״̬����
CSS css[MAX_Count];//����ʽ
VN_Set VN_First[MAX_Count];//���ս������First����
string VT;//�ս������
string VN;//���ս������
map<int, string> GOTO[MAX_Count];
map<int, string> ACTION[MAX_Count];
vector<CSS_LR1> I[MAX_Count]; //��Ŀ��

extern vector<int> G[1000];        //���ڽӱ��Tree
int cnt = 0;                //Tree�Ľڵ���
extern vector <quaternion> attr;            //ÿ���ڵ���õ����ԣ�������E
extern vector<string> symbol;    //ÿ���ڵ��Ӧ�ķ�������

bool cmp_vector(vector<CSS_LR1>& v1, vector<CSS_LR1>& v2)//v1��v2���� ��̬�ռ䴢�� �ж���Ŀ���Ƿ����
 {
	 if (v1.size() != v2.size()) return false;
	 int i;
	 for (i = 0; i < v2.size(); i++)
	 {
		 CSS_LR1 t;
		 t = v2[i];
		 vector<CSS_LR1>::iterator result = find(v1.begin(), v1.end(), t); //����3
		 if (result == v1.end()) //û�ҵ�
			 return false;
	 }
	 return true;
 }
void read_the_text3()//�����ļ� ���ж� ��һ�У����ս�� �ڶ���֮�󣺲���ʽ

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
    //��ȡ�ļ���һ�е�����
    getline(istr,strline);
    mm=strline.length();
    VT = strline;
    //cout<<"�ս��"<<':'<<VT<<endl;
    //��ȡ���ʽ����
    while(getline(istr,strline))
    {
        mm = strline.size();
        css[i].start = strline[0];
        if(VN.find(css[i].start) == -1)//��û����
        {
            int z = VN.size();
            VN.insert(z,css[i].start);//��VNĩβ����һ���ַ�
            VN_First[z].VN_name = css[i].start;
        }
        css[i].next = strline.substr(3);//�ַ����Ľ�ȡ
        cout<<"\n\n___����ʽ"<<i<<':'<<css[i].start<<"->"<<css[i].next<<endl;
        i++;
    }
    ccscouont = i;
//    cout<<"����ʽ������"<<ccscouont<<endl;
//    cout<<"���ս����"<<VN<<endl;
    istr.close();
}
string first(string a)//aΪ���ս����first����
{
    int i = 0;
    int j = 0;
    int k = 0;//�ڼ������ս��
    k = VN.find(a);
    for(i = 0;i < ccscouont;i++)//ÿһ��ÿһ������
    {
        if(css[i].start == a)//a���ķ���
        {
            for(j = 0;j < css[i].next.size();j++)//�ķ��Ҳ� һ���ַ�һ���ַ�����
            {
                if(VT.find(css[i].next[j]) <100 )//�������ս�� && VN.find(css[i].next[j]) == -1
                {
                    if(VN_First[k].FIRST.find(css[i].next[j]) >100)//���ս���ڸ÷��ս����first������û�г��ֹ�
                    {
                        int z = VN_First[k].FIRST.size();//��ĩβ�����ս��
                        char b = css[i].next[j];
                        VN_First[k].FIRST.insert(z,1,b);//��firstĩβ��һ��b�ַ�
                    }
                    break;
                }
                else if(VN.find(css[i].next[j]) <100 && VT.find(css[i].next[j]) >100)//���ս����ͷ
                {
                    if(css[i].next[j] == css[i].start[0])//�Ƿ��ս����������Ҫ�µ�Ԫ�ؽ���first��
                    {
                        break;//����һ������
                    }

                    //��������ս����first��
                    string D;
                    string c = "a";
                    char d[2] = {css[i].next[j],0};
                    c = d;
                    D = first(c);//����÷��ս����first����
                    int e = 0;
                    int m = 0;
                    for(e = 0; e < D.size(); e++)
                    {
                        int z = 0;
                        z = VN_First[k].FIRST.size();

                        if(VN_First[k].FIRST.find(D[e])>100)//û���ظ�
                        {
                            char b = D[e];
                            VN_First[k].FIRST.insert(z,1,b);//�Ѹ÷��ս����first����
                            m = 1;
                        }
                    }
                    //�жϸ÷��ս���ܷ��ƿ�
                    if(D.find('^')!=-1)//���ƿ� ��������һ�����ս����
                    {
                        if(j != css[i].next.size() - 1)//û���ߵ��ķ��Ҳ����һ���ַ�
                        {
                            //ɾ��^
                            int q = 0;
                            q = VN_First[k].FIRST.find('^');
                            VN_First[k].FIRST.erase(q,1);//�ַ���ɾ�� q:λ�� 1������
                        }
                    }
                    else //�����ƿ�
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
//        cout<<VN[i]<<"��first����"<<VN_First[i].FIRST<<endl;
//    }
}


int find_in_vector(vector<CSS_LR1> T, CSS_LR1 p)//ȷ��û���ظ�����
{
	 vector<CSS_LR1>::iterator it;
	 for (it = T.begin(); it != T.end(); it++) {
		 if (*it == p) {
			 return 1;//�������Ѿ�������
		 }
	 }
	 return 0;//������û�д���
 }

void showI(vector<CSS_LR1> I) {//չʾ��Ŀ��
	 vector<CSS_LR1>::iterator it;//ʵ������I�е�Ԫ�ر���
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
vector<CSS_LR1> CLOSURE(CSS_LR1 I,vector<CSS_LR1> t5) //��հ� ĳ״̬��һ���ķ����ʽ�ıհ�
{
   //cout<<2<<endl;
   vector<CSS_LR1> T;
    if(I.num >= I.next.size())//��Լ��Ŀ�������Ŀ
    {
       // cout<<4<<endl;
        return T;//����
    }
    else//��Լ��Ŀ���ƽ���Ŀ
    {
        string a;
        char b[2] = {I.next[I.num],0};
        a = b;//���ƽ����ַ�
        if(VT.find(a) < 100)//�ƽ���Ŀ �������µķ�����Ҫ������ �ս��
        {
                return T;//����
        }

        else//��Լ��Ŀ ���µ���Ŀ��Ҫ������
        {
            int i = 0;
            for(i = 0;i<ccscouont;i++)//һ���ķ�һ���ķ������ķ��Ҳ��ȴ�Լ��Ŀ�ķ��ս���Ĳ���ʽ
            {
                if(css[i].start == a)//�ҵ�
                {
                    CSS_LR1 p;//�½�һ���ķ����ʽ�ıհ�
                    p.start = css[i].start;
                    p.num = 0;
                    p.next = css[i].next;
                    string f1;
                    int j = 0;
                    for(j = I.num+1;j<I.next.size();j++)
                    {
                        string f2;//�ݴ�first����
                        string c;
                        char d[2] = {I.next[j],0};
                        c = d;
                        if(VT.find(d)<100)//����Ϊ�ս��
                        {
                            int z = 0;
                            z = f1.size();
                            f1.insert(z,1,I.next[j]);//�ѷ��ս������

                            break;
                        }
                        else//����Ϊ���ս��
                        {
                            f2 = first(d);//����÷��ս����first����
                        }
                        int y = 0;
                        y = f1.size();
                        f1.insert(y,f2);
                        if(f2.find('^') > 100)//�����ƿ� ֹͣѰ�Ҽ�����ǰ������
                        {
                            break;
                        }
                    }

                    if(f1.size() == 0)//û���µ�first����
                    {
                        p.tail = I.tail;//��ԭ������ǰ��������������
                    }
                    else
                    {
                        if(f1.find('^') < 100)//���ƿ� ��ǰ����������Ϊ��
                        {
                            int q = 0;
                            q = f1.find('^');//�ҵ�^��λ��
                            f1.erase(q,1);//ɾ��λ�ã�q ɾ��������1
                            //��p.tailĩβ����ԭ������ǰ������
                            int x = 0;
                            x = f1.size();
                            f1.insert(x,I.tail);
                        }
                        p.tail = f1;
                    }
                    if(find_in_vector(t5,p) == 0)//�����в����ڶԸ������ʽ�ķ���
                    {
                        T.push_back(p);//������Tĩβ�����Ԫ��p
                        vector<CSS_LR1> o = CLOSURE(p,t5);
                        vector<CSS_LR1>::iterator z;//��������z�Ĵ�С���Ը�����Ҫ�ı� z����ΪCSS_LR1 ʵ��Ԫ�ر���
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

void printclosure()//������LR�ķ�������
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
    I[0].insert(I[0].begin(), p);//��I[0]��ͷ�����һ���¼ӵ�״̬

//  cout << p.start << "  " << p.next << "  " << p.num << "  " << p.tail << endl;

    I_count  = 1;
    for(i = 0; i < I_count; i++)
    {
        //����״̬�е�ÿһ���ķ����ʽ��Ҫ��closure
        vector<CSS_LR1>::iterator t;//����һ�����ɵ�״̬ ʵ��I[i]Ԫ�ر���

        //��״̬i�е� ��Լ��Ŀ ����action��
        for(t = I[i].begin(); t!=I[i].end(); t++) //��״̬I[i]��һ���ķ����ʽһ���ķ����ʽ����
        {
            int num = 0;
            CSS_LR1 t2;//��t2ָ����ýṹ��
            t2 = *t;//����һ�ξ��ǵ���һ���ķ����ʽ
            if(t2.num == t2.next.size() || t2.next[t2.num] == '^')//��Լ��Ŀ
            {
                int bb = 0;
                for(bb = 0; bb < ccscouont; bb ++)
                {
                    if(css[bb].start == t2.start && css[bb].next == t2.next)
                    {
                        num = bb+1;//�ҵ��ǵڼ����ķ����ʽ��������Լ����һ���ķ����ʽ
                        //+1��������һ���ķ����ʽ���ʼ��һ���ķ����ʽ
                        break;
                    }//end if
                }//end for
                std::stringstream ss;//�Զ����䴢��ռ� �Զ�ѡ������ת�� ���������������
                ss<<num;//��numֵ������ss�� ��int����ת��Ϊstring����
                string s;
                s = ss.str();//��ss�е����ݱ��string���͸�ֵ��s
                int k = 0;
                for(k = 0; k < t2.tail.size(); k++)//��ǰ���������ܲ�ֹһ��
                {
                   ACTION[i][t2.tail[k]] = "r"+s;

                 //  cout<<"ACTION[i][t2.tail[k]]:"<<ACTION[i][t2.tail[k]]<<endl;  ���ʽ��Ŷ�Ӧ��Ϊr��
               //  cout<<"t2.tail[k]:"<<t2.tail[k]<<endl;

                }//end for
                if(t2.num == 1 && t2.next[0] == css[0].start[0])//������Ŀ
                {
                    ACTION[i]['#'] = "acc";
                   // cout<<"ACTION[i]['#']:"<<ACTION[i]['#']<<endl;

                }//end if
            }//end if
        }//end for
        int x = 0;
        x = VN.size();
        int j = 0;

        for(j = 0; j < x; j++) //�ҷ��ս��
        {
            vector<CSS_LR1> temp;//����

            int k = 0;

            for(k = 0; k<I[i].size(); k++)//I[i].size()ָ��״̬���м����ķ����ʽ
            {
                CSS_LR1 lr = I[i][k];//��״̬i�еĵ�k���ķ����ʽ
                //cout<<'&'<<i<<k<<'&'<<endl;
                if(lr.num < lr.next.size() && lr.next[lr.num] == VN[j])//�ҵ��˴�Լ��Ŀ ֻ���һ������Լ����
                {
                   // cout<<1<<endl;
                    vector<CSS_LR1> t2;
                    vector<CSS_LR1> t7;
                    vector<CSS_LR1>::iterator zz;
                    lr.num = lr.num+1;//�ƽ���һ������
                    vector<CSS_LR1> T1;
                    t2 = CLOSURE(lr,temp);
                    T1 = t2;
                    t2.push_back(lr);//��t2ĩβ����lr
                    //�ж�t2�е�Ԫ����temp���Ƿ���ֹ�
                    for(zz = t2.begin();zz!=t2.end();zz++)
                    {
                        if(find_in_vector(temp,*zz) == 0)
                            t7.push_back(*zz);//end if
                    }//end for
                    temp = temp + t7;//�µ�״̬  vector<CSS_LR1>::iterator sss;

                }//end if
            }//end for

            //�������ս������GOTO��
            if(temp.size() > 0)
            {
                int w = 0;
                for(w = 0; w < I_count ; w++)
                {
                    if(cmp_vector(I[w],temp))//��Ŀ������
                    {
                        break;//��������״̬
                    }//end if
                }//end for
                if(w == I_count)//״̬û�г��ֹ�
                {
                    I[I_count] = temp;//�����״̬
                    //cout<<'I'<<i<<"--"<<VN[j]<<"->"<<'I'<<I_count<<endl;
                    std::stringstream ss;
                    ss<<I_count;
                    string s = ss.str();
                    GOTO[i][VN[j]] = s;
                    I_count ++;
                }
                else//״̬�Ѿ����ֹ�
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
        for(h = 0; h < r; h++) //�� �ս������ ������Ѱ��
        {
            vector<CSS_LR1> t4;
            int v = 0;
            int vv = I[i].size();
            for(v = 0; v<vv; v++)//һ���ķ�һ���ķ�����
            {
                CSS_LR1 zr = I[i][v];
                if(zr.num < zr.next.size() && zr.next[zr.num] == VT[h] && zr.next[zr.num] != '^')//�ҵ����ƽ���Ŀ ���Ƴ��Ĳ�Ϊ��
                {
                    vector<CSS_LR1> t5;
                    vector<CSS_LR1> t6;
                    vector<CSS_LR1>::iterator z;
                    //cout<<zr.start<<"->"<<zr.next<<','<<zr.tail<<'%'<<endl;
                    zr.num++;
                    //vector<CSS_LR1>::iterator ssss;
                    vector<CSS_LR1> T2;
                    t5 = CLOSURE(zr,t4);//ȷ�����ķ�����ʽ��û���ظ���//Ҫ��������⣺��ͬ�ķ����ʽ�Ĳ���ʽ�������ظ���
                    T2 = t5;
                    t5.insert(t5.begin(),zr);//�Ե�ǰ״̬������
                    for(z = t5.begin();z!=t5.end();z++)
                    {
                        if(find_in_vector(t4,*z) == 0)
                            t6.push_back(*z);//end if
                    }//end for
                    t4 = t4+t6;//t4��һ���µ���Ŀ��
                }//end if
            }//end for

            //�����ս�� ����action��
            if(t4.size()>0)
            {
                int aa = 0;
                for(aa = 0; aa<I_count; aa++) //���µ���Ŀ���Ƿ��Ѵ���
                {
                    if(cmp_vector(I[aa],t4))
                    {
                        break;
                    }//end if
                }//end for
                if(aa == I_count)//�����ڸ��·�����
                {
                    I[I_count] = t4;
                    //cout<<'I'<<i<<"--"<<VT[h]<<"->"<<'I'<<I_count<<endl;
                    std::stringstream ss;
                    ss<<I_count;
                    string s = ss.str();
                    ACTION[i][VT[h]] = "S"+s;
                    I_count ++;//����
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

void analysis()//���ַ������з���
{
    //vector <string> print;//���봮
    vector <string> actionn;//actionջ
    vector <string> gotoo;//gotoջ
    vector <int> state;//״̬ջ
    vector <string> sign;//����ջ
    vector <int> num;//��¼״̬����λ��
    string input;
    int step = 1;//����
    ifstream infile;
    infile.open("process.txt");
    getline(infile, input);

    //0��״̬��״̬ջ
    //#�����ջ
    state.push_back(0);
    sign.push_back("#");
    //��ʼ�����ַ���
    int a = input.size();//aΪ�ַ�������
    input = input + "#";
    string inputt;
    inputt = input;
    int j = 0;
    int i = 0;
    quaternion g;//���
    stack <int> S;//������
   // E g;
    cout<<"����"<<'\t'<<"״̬ջ"<<'\t'<<"����ջ"<<'\t'<<"���봮"<<setw(27)<<"ACTION"<<setw(27)<<"GOTO"<<endl;
    while(i <= a)
    {
        string s;//Ҫ������Ѿ��򻯵��ַ�
        char b[2] = {input[i],0};
        s = b;
        int statee = state.back();//״̬ջ�������һ������
        //״̬ջ��ջ��������ջ��ջ���ڷ�����Ѱ��

        if(ACTION[statee][s[0]] == "acc")//�����ɹ�
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
            cout<<'\t'<<"�����ɹ���"<<endl;
             break;//��������
        }//end if
        string c;
        c = ACTION[statee][s[0]];
        int d = c.size();


        if(d == 0)//������Ϊ�գ�����ʧ��
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
            cout<<'\t'<<"����ʧ�ܣ�"<<endl;
            cout<<"��ȷӦΪ��"<<endl;
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
        else if(d != 0)//������Ϊ��
        {
            //Ҫ������״̬ջ�����ݺ�Ҫ���������봮ջ������
            string now;
            now = s;
            //Ҫ�������ջ�����ս��
                string e = ACTION[statee][now[0]];

                if(e[0] == 'S')//���ƽ���Ŀ��Ҫ�������ջ�����ս��
                {
                    //�ѵ�i���������Դ�����
                    quaternion g;//���
                    if(M[i].pred == 2)//���ַ��������ַ�������
                    {
                        g.name = M[i].language;
                    }

                    else if(M[i].pred == 3)//�Ǳ��������������ֵ
                    {
                        g.val = M[i].language;
                    }

                    else if(M[i].pred == 4 || M[i].pred == 1)//�Ǳ�ʶ������ţ������ʶ������ű���
                    {
                        g.type = M[i].language;
                    }

                    symbol.push_back(M[i].predd);//���ķ�������
                    S.push(Q_count++);
                    attr.push_back(g);

                    cout<<step<<'\t';
                    vector<string> :: iterator it;
                    vector<int> :: iterator itt;
                    for(itt = state.begin();itt!=state.end();itt++)//���״̬
                        cout<<*itt;
                    cout<<'\t';
                    for(it = sign.begin();it!=sign.end();it++)//�������
                        cout<<*it;
                    cout<<'\t'<<'\t';
                    actionn.push_back(e);//����actionջ
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
                        cout<<inputt;//��� ���봮
                    cout<<'\t'<<'\t';
                    cout<<e;//���  ����
                    cout<<'\t';
                    cout<<endl;
                    step++;
                    inputt.erase(0, 1);
                }
                else if(e[0] == 'r')//��Լ
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
                    f = css[pp-1].start;//pp-1Ϊ�õڼ�������ʽ���й�Լ

                    int gg = css[pp-1].next.size();
                    int h = 0;
                    queue <int> son;//ʹ�ö��� front()���ʶ���Ԫ�� back()���ʶ�βԪ��
                    //son��¼��վ�Ľ��
                    for(h = 0;h<gg;h++)
                    {
                        int l = num.back();
                        state.pop_back();//���ڹ�Լ״̬��ջ
                        sign.pop_back();
                        num.pop_back();
                        son.push(S.top());
                        S.pop();//����ջ
                    }//end for
                    int j = state.back();//״̬ջջ��Ԫ��
                    sign.push_back(f);// ��Լ���״̬ ��ջ

                    std::stringstream ss;
                    ss<<pp-1;
                    string s = ss.str();

                    symbol.push_back(s);// ��Լ�ķ��� ��ջ
                    attr.push_back(g);
                    S.push(Q_count++);

                    while(!son.empty())//�����﷨��
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
                    cout<<inputt;//��� ���봮
                    cout<<'\t'<<'\t';
                    cout<<e;//��� ACTION
                    cout<<'\t';
                    cout<<k;//��� GOTO
                    cout<<endl;
                    step++;
                }//endif
            }//endif
            //Ҫ��ջ���Ƿ��ս��

    }//endwhile
    cout<<endl;
   // cout<<"�﷨����"<<endl;
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
void print_ACTION_GOTO() {//��excel����ϴ�ӡ�� ������
	 set<string>::iterator it;
	 print_line();
	 cout << setw(27) << setiosflags(ios::right) << "ACTION";
	 cout << setw(250) << setiosflags(ios::left) << "  GOTO" << endl;
	 print_line();
	 cout << setw(8) << "��Ŀ��" << "|";

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

			 if (GOTO[j][VN[i]].size() != 0)//GOTO��Ϊ0
				 cout << setw(8) << GOTO[j][VN[i]] << "|";
			 else {
				 cout << setw(8) << " " << "|";
			 }
		 }
		 cout << endl;
	 }
	 print_line();
 }
