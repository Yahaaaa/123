#include "main.h"

int iskey(char c[]);

char key[11][20] = {"for","char","if","break","while","do","end","void","float","int"};//关键字

int llen[MAX];//每个字符串所在的源代码的行

extern neww M[MAX];
int M_count = 0;//单词个数

void read_the_text(line_e &ch) // 读出文件按行读按格式输出源代码
{
    int mm;
    ifstream istr;
    string strline;
    istr.open("program.txt");
    if (!istr.is_open())
    {
        cout << "Error opening file";
        return;
    }

    while (getline(istr, strline))
    {
        mm = strline.length();
        ch.linelen[ch.len] = mm; // 每一行的长度 (连上空格)
        cout << strline << endl;
        ch.line[ch.len].sit = ch.len;
        ch.len++; // 多少行
    }

    istr.close();
}

//按空格读出字符串
void read_the_text2(line_e &ch)//读入文件，按空格读
{
    int i,mm,s;
    i = 0;
    int n;//记录已经复制了几个字符
    n = 0;
    s = 0;
    ifstream istr;
    string temp;
    vector <string> veci;
    istr.open("program.txt");
    while(istr >> temp)
    {
        veci.push_back(temp);
        //cout<<i<<temp<<'*';
        mm=temp.length();
        ch.line[i].ret_len = mm;
        temp.copy(ch.line[i].ret_sig,mm,0);
        n = n+mm;
        //cout<<n<<'?'<<ch.linelen[s]<<endl;
        if (n > ch.linelen[s])
        {
            //cout<<llen[i]<<' '<<"**";
            s++;
            n=n-ch.linelen[s-1]+1;
        }
        llen[i] = s;
        //cout<<llen[i]<<"&&"<<endl;
        i++;
    }
    ch.num = i-1;
    istr.close();
}

char *str(char numstr[],int numstrsize)//把字符串数组转为字符串
{
    char *s=(char *)malloc((numstrsize)*sizeof(char));
    int i;
    for(i = 0;i<numstrsize;i++)
    {
        s[i] = numstr[i];
        //cout<<s[i];
    }
    return s;
}

//每一行的单词数

void exchang(line_e &ch,int &p)//把字符串数组转换成字符串
{
    char *c;
    char exchang[MAX];
    int len_num;//len_num为单词长度
    int j;//第j行
    j = 0;
    int returnkey;
    FILE *fp;
    fp = fopen("process.txt","a");
    for(j = 0; j <=ch.num; j++)
    {
        strcpy(exchang,ch.line[j].ret_sig);
        c = str(exchang,ch.line[j].ret_len);
        c[ch.line[j].ret_len] = '\0';
        len_num = ch.line[j].ret_len;
        returnkey = iskey(c,fp);
        if(returnkey != -1)
        cout<<'<'<<c<<','<<returnkey<<'>'<<endl;
        if(returnkey == -1)
            scanner(returnkey,p,c,len_num,j,fp);
        }
    fclose(fp);
//    for(int i = 0;i<M_count;i++)
//        cout<<M[i].language<<endl;
}
int iskey(char c[],FILE *fp)//判断是否为关键字
{
    int i;
    neww v;
    for(i = 0;i < 11;i++)
    {
        if(strcmp(c,key[i]) == 0)
        {
            if(strcmp("int",key[i]) == 0)
                {char a = 'n';
                 M[M_count].language = c;
                 M[M_count].pred = 1;
                 M[M_count].predd = c;
                 M_count++;
                 fputc(a,fp);
                }
            else if(strcmp("float",key[i]) == 0)
                {char a = 'l';
                 M[M_count].language = c;
                 M[M_count].pred = 1;
                 M[M_count].predd = c;
                 M_count++;
                fputc(a,fp);
                }
            else{
                char a = c[0];
                 M[M_count].language = c;
                 M[M_count].pred = 1;
                 M[M_count].predd = c;
                 M_count++;
                fputc(a,fp);
                }

            return i;
        }


    }
    return -1;
}
int ischar(char ch)//判断符字是不是字母
{
    if((ch>='a' && ch<='z') || (ch>='A' && ch<='Z'))
        return 1;
    else return 0;
}
int isnum(char ch)//判断首符是不是数字
{
    if(ch >= '0' && ch<='9')
        return 1;
    return 0;
}
int issymbol(char ch,int sym)//判断字符是不是符号
{

    if(ch == '+')
        sym = 20;
    else if(ch == '-')
        sym = 21;
    else if(ch == ':')
        sym = 22;
    else if(ch == ';')
        sym = 23;
    else if(ch == '<')
        sym = 24;//<=:31
    else if(ch == '>')
        sym = 25;
    else if(ch == '=')
        sym = 26;
    else if(ch == '*')
        sym = 27;
    else if(ch == '/')
        sym = 28;
    else if(ch == '(')
        sym = 29;
    else if(ch == ')')
        sym = 30;
    else if(ch == '!')
        sym = 19;
    else if(ch == '.')
        sym = 18;
    else if(ch == '&')
        sym = 17;
    else if(ch == '[')
        sym = 16;
    else if(ch == ']')
        sym = 15;
    else if(ch == '{')
        sym = 14;
    else if(ch == '}')
        sym = 13;
    else if(ch == ',')
        sym = 12;
    else if(ch == '"')
        sym = 37;
    else
        sym = 0;
    return sym;
}

void scanner(int &syn,int &p,char c[],int word_num,int len,FILE *fp)
{
    p = 0;
    syn = -1;
    int sym;
    sym = 0;
    if(ischar(c[0])== 1)//首字母是字符
    {
        syn = 10;
        char strr[MAX];
        while(p<word_num)
        {
            if(ischar(c[p]) ==1 || isnum(c[p])==1)
            {
                strr[p] = c[p];
                p++;
            }
            else if(ischar(c[p]) ==0 && isnum(c[p])==0)
            {
                break;
            }
        }
        strr[p] = '\0';
        if(syn != -1)
        {
            char a = 'z';
            fputc(a,fp);
            M[M_count].language = strr;
            M[M_count].pred = 2;
            M[M_count].predd = "ID";
            M_count++;
            cout<<'<'<<strr<<','<<syn<<'>'<<endl;
        }

        else if(syn == -1)
        {
            cout<<'<'<<strr<<','<<syn<<'>'<<endl;
            cout<<"error's len is:"<<llen[len]+1<<endl;
        }
        if(issymbol(c[p],sym)!=0)
        {
                goto p1;
        }
        else if(p<word_num && issymbol(c[p],sym) == 0)
            {

                goto p2;
            }
    }
    else if(isnum(c[p]) == 1)
    {
        char num[MAX];
        int k;
        k = 0;
        num[k] = c[p];
        p++;
        k++;
        int b,d;//记录数字个数
        b = 1;
        d = 0;
        if(ischar(c[p+1])==1)
        {
            syn = -1;
            cout<<'<'<<c[p]<<','<<syn<<'>'<<"word benind number"<<endl;
            cout<<"error's len is:"<<llen[len]+1<<endl;
        }
        syn = 11;
        while(p<word_num)//数字打头
        {
            if(isnum(c[p]) == 0 && (b<8 && d<9) && issymbol(c[p],sym) != 18)//
            {
                    break;
            }
            else if(isnum(c[p]) == 0 && (b >= 8 ||d>=9) && issymbol(c[p],sym) != 18)
            {
                syn = -1;
                break;
            }
            else if(isnum(c[p])==1 && (b >= 8 ||d>=9))
            {
                syn = -1;
                num[k] = c[p];
                p++;
                k++;
                if(d==0) b++;
                if(d!=0) d++;
            }
            else if(isnum(c[p])==1 && (b<8 && d<9))
            {
                num[k] = c[p];
                p++;
                k++;
                if(d==0) b++;
                if(d!=0) d++;
            }
            else if( issymbol(c[p],sym) == 18)//浮点数
            {
                num[k] = c[p];
                k++;
                p++;
                d = 1;

            }
        }//break
        d = d-1;
        if(b<8 && d<9 && syn == 11 )
        {
            num[k] = '\0';
            char a = 'k';
            fputc(a,fp);
            M[M_count].language = num;
            M[M_count].pred = 3;
            M[M_count].predd = "NUM";
            M_count++;
            cout<<'<'<<num<<','<<syn<<'>'<<endl;
        }
        else if(syn == -1)
        {
            num[p] = '\0';
            cout<<'<'<<num<<','<<syn<<'>'<<endl;
            cout<<"error's len is:"<<llen[len]+1<<endl;
        }
        if(issymbol(c[p],sym) != 0)
        {
                goto p1;
        }
        else if(p<word_num && issymbol(c[p],sym) == 0)
            goto p2;
    }//isnum(c[p]) == 1
    else if(issymbol(c[p],sym) != 0)
    {
p1:
        while(p<word_num)
        {
            if(issymbol(c[p],sym) != 0)
            {
                if(issymbol(c[p+1],sym)==26 || issymbol(c[p+1],sym)==17 || issymbol(c[p+1],sym)==20)//可能是双目运算符
                {
                    if(issymbol(c[p],sym) == 24)
                    {
                        p++;
                        if(issymbol(c[p],sym) ==26)
                        {
                            syn = 31;
                            char a = c[p-1];
                            fputc(a,fp);
                            a = '=';
                            fputc(a,fp);
                            char b[2] = {c[p-1],0};
                            string t = b;
                            M[M_count].language = t;
                            M[M_count].pred = 4;
                            M[M_count].predd = t;
                            M_count++;
                            char bb[2] = {c[p],0};
                            string tt = bb;
                            M[M_count].language = tt;
                            M[M_count].pred = 4;
                            M[M_count].predd = tt;
                            M_count++;
                            cout<<'<'<<c[p-1]<<'='<<','<<syn<<'>'<<endl;
                        }
                    }
                    else if(issymbol(c[p],sym) == 17)
                    {
                        p++;
                        if(issymbol(c[p],sym) ==17)
                        {
                            syn = 35;
                            char a = c[p-1];
                            fputc(a,fp);
                            a = '&';
                            fputc(a,fp);
                            char b[2] = {c[p-1],0};
                            string t = b;
                            M[M_count].language = t;
                            M[M_count].pred = 4;
                            M[M_count].predd = t;
                            M_count++;
                            char bb[2] = {c[p],0};
                            string tt = bb;
                            M[M_count].language = tt;
                            M[M_count].pred = 4;
                            M[M_count].predd = tt;
                            M_count++;
                            cout<<'<'<<c[p-1]<<'&'<<','<<syn<<'>'<<endl;
                        }
                    }
                    else if (issymbol(c[p],sym) == 25)
                    {
                        p++;
                        if(issymbol(c[p],sym) ==26)
                        {
                            syn = 32;
                            char a = c[p-1];
                            fputc(a,fp);
                            a = '=';
                            fputc(a,fp);
                            char b[2] = {c[p-1],0};
                            string t = b;
                            M[M_count].language = t;
                            M[M_count].pred = 4;
                            M[M_count].predd = t;
                            M_count++;
                            char bb[2] = {c[p],0};
                            string tt = bb;
                            M[M_count].language = tt;
                            M[M_count].pred = 4;
                            M[M_count].predd = tt;
                            M_count++;
                            cout<<'<'<<c[p-1]<<'='<<','<<syn<<'>'<<endl;
                        }
                    }
                    else if (issymbol(c[p],sym) == 26)
                    {
                        p++;
                        if(issymbol(c[p],sym) ==26)
                        {
                            syn = 33;
                            char a = c[p-1];
                            fputc(a,fp);
                            a = '=';
                            fputc(a,fp);
                            char b[2] = {c[p-1],0};
                            string t = b;
                            M[M_count].language = t;
                            M[M_count].pred = 4;
                            M[M_count].predd = t;
                            M_count++;
                            char bb[2] = {c[p],0};
                            string tt = bb;
                            M[M_count].language = tt;
                            M[M_count].pred = 4;
                            M[M_count].predd = tt;
                            M_count++;
                            cout<<'<'<<c[p-1]<<'='<<','<<syn<<'>'<<endl;
                        }
                    }
                    else if (issymbol(c[p],sym) == 19)
                    {
                        p++;
                        if(issymbol(c[p],sym) ==26)
                        {
                            syn = 34;
                            char a = c[p-1];
                            fputc(a,fp);
                            a = '=';
                            fputc(a,fp);
                            char b[2] = {c[p-1],0};
                            string t = b;
                            M[M_count].language = t;
                            M[M_count].pred = 4;
                            M[M_count].predd = t;
                            M_count++;
                            char bb[2] = {c[p],0};
                            string tt = bb;
                            M[M_count].language = tt;
                            M[M_count].pred = 4;
                            M[M_count].predd = tt;
                            M_count++;
                            cout<<'<'<<c[p-1]<<'='<<','<<syn<<'>'<<endl;
                        }
                    }
                    else if (issymbol(c[p],sym) == 20)
                    {
                        p++;
                        if(issymbol(c[p],sym) == 20)//++
                        {
                            syn = 36;
                            char a = c[p-1];
                            fputc(a,fp);
                            a = '+';
                            fputc(a,fp);
                            char b[2] = {c[p-1],0};
                            string t = b;
                            M[M_count].language = t;
                            M[M_count].pred = 4;
                            M[M_count].predd = t;
                            M_count++;
                            char bb[2] = {c[p],0};
                            string tt = bb;
                            M[M_count].language = tt;
                            M[M_count].pred = 4;
                            M[M_count].predd = tt;
                            M_count++;
                            cout<<'<'<<c[p-1]<<'+'<<','<<syn<<'>'<<endl;
                        }
                    }
                }//双目运算符
                else if(issymbol(c[p],sym) != 0)
                {
                    syn = issymbol(c[p],sym);
                    char a = c[p];
                    fputc(a,fp);
                    char b[2] = {c[p],0};
                    string t = b;
                    M[M_count].language = t;
                    M[M_count].pred = 4;
                    M[M_count].predd = t;
                    M_count++;
                    cout<<'<'<<c[p]<<','<<syn<<'>'<<endl;
                }
            }//if(issymbol(c[p],sym) != 0)
            else if(p<word_num && issymbol(c[p],sym) == 0 && ischar(c[p])==0 && isnum(c[p]) == 0)
                    {
                    goto p2;}
            else if(ischar(c[p]) == 1)
            {
                char strrr[MAX];
                int z;
                z = 0;
                while(p<word_num){
                    if(ischar(c[p]) ==1 || isnum(c[p])==1)
                    {
                        syn = 10;
                        strrr[z] = c[p];
                        p++;
                        z++;
                    }
                    else if(ischar(c[p]) ==0 && isnum(c[p])==0)
                    {
                        break;
                    }
                }
                strrr[z] = '\0';
                char a = 'z';
                fputc(a,fp);
//                char b[2] = {a,0};
//                string t = b;
                M[M_count].language = strrr;
                M[M_count].pred = 2;
                M[M_count].predd = "ID";
                M_count++;
                cout<<'<'<<strrr<<','<<syn<<'>'<<endl;
                if(issymbol(c[p],sym) != 0)//是符号
                {
                   goto p1;
                }
                else if(p<word_num && issymbol(c[p],sym) == 0)//是非法字符
                    goto p2;
            }//else if(ischar(c[p]) == 1)
            else if(isnum(c[p]) != 0)
            {
                char nummm[MAX];
                int k;
                k = 0;
                nummm[k] = c[p];
                p++;
                k++;
                int b,d;//记录数字个数
                b = 1;
                d = 0;
                syn = 11;
                while(p<word_num)//数字打头
                {
                    if(isnum(c[p]) == 0 && (b<8 && d<9) && issymbol(c[p],sym) != 18)
                    {
                            break;
                    }
                    else if(isnum(c[p]) == 0 && (b >= 8 ||d>=9) && issymbol(c[p],sym) != 18)
                    {
                        syn = -1;
                        break;
                    }
                    else if(isnum(c[p])==1 && (b >= 8 ||d>=9))
                    {
                        syn = -1;
                        nummm[k] = c[p];
                        p++;
                        k++;
                        if(d==0) b++;
                        if(d!=0) d++;
                    }
                    else if(isnum(c[p])==1 && (b<8 && d<9))
                    {
                        nummm[k] = c[p];
                        p++;
                        k++;
                        if(d==0) b++;
                        if(d!=0) d++;
                    }
                    else if( issymbol(c[p],sym) == 18)
                    {
                        nummm[k] = c[p];
                        k++;
                        p++;
                        d = 1;
                    }
                }
                d = d-1;
                if(b<8 && d<9 && syn == 11)
                {
                    nummm[k] = '\0';
                    char a = 'k';
                    fputc(a,fp);
//                    char b[2] = {a,0};
//                    string t = b;
                    M[M_count].language = nummm;
                    M[M_count].pred = 3;
                    M[M_count].predd = "NUM";
                    M_count++;
                    cout<<'<'<<nummm<<','<<syn<<'>'<<endl;
                }

                else if(syn == -1)
                {
                    nummm[k] = '\0';
                    cout<<'<'<<nummm<<','<<syn<<'>'<<"too more number"<<endl;
                    cout<<"error's len is:"<<llen[len]+1<<endl;
                }
            if(issymbol(c[p],sym) != 0)
            {
                goto p1;
            }
            else if(p<word_num && issymbol(c[p],sym) == 0)
            goto p2;
            }//else if(isnum(c[p]) != 0)
            p++;
        }
    }
    else
    {
        p2:
            syn = -1;
            char err[MAX];
            int t;
            t = 0;
            while(p<word_num)
            {
                err[t] = c[p];
                t++;
                p++;
            }
            err[t] = '\0';
            cout<<'<'<<err<<','<<syn<<'>'<<"illegal"<<endl;
          //  cout<<"error's len is:"<<llen[len]+1<<endl;
    }
}
