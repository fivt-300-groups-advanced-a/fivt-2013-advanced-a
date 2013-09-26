#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
vector<string> s;
bool type(char a)
{
    return a>='0' && a<='9';
}
void get_lexem(string &a,string &s1)
{

    a+=" ";
    if (a==" ") s1="";
    else 
    {
        s1="";
        do { 
            s1+=a[0];
            a.erase(a.begin()); 
        } while (a!=" " && type(s1[s1.length()-1])==type(a[0]));
    }
    a.erase(a.end()-1);
}
int srav_lexem(string s1,string s2)
{
    if (s1==s2) return 0;
    if (s1=="") return -1;
    if (s2=="") return 1;
    if (type(s1[0]) && type(s2[0])){
        if (atoi(s1.c_str())>atoi(s2.c_str())) return 1;
        else if (atoi(s1.c_str())<atoi(s2.c_str())) return -1;
    }
    if (!type(s1[0]) && type (s2[0])) return -1;
    if (type(s1[0]) && !type(s2[0])) return 1;
    if (s1>s2) return 1;
    else if (s2>s1) return -1;
    else return 0;
}
int srav(string a,string b)
{
    if (a==b) return 0;
    string s1,s2;
    int f=0;
    while (!f) {
        get_lexem(a,s1);
        get_lexem(b,s2);
        f=srav_lexem(s1,s2);
    }
    return f;
}
void qs(int b,int e)
{ 
    string x=s[(b+e)/2];
    int l=b,r=e;
    do{
        while ((srav(s[l],x))==-1) l++;
        while ((srav(s[r],x))==1) r--;
        if (l<=r) {
            swap(s[l],s[r]);
            l++;r--;
        }
    } while (l<=r);
    if (b<r) qs(b,r);
    if (l<e) qs(l,e);
}
int main()
{
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);
    int n=0;
    string u;
    s.push_back(" ");
    while (!cin.eof())
    {
        n++;
        cin >> u;
        s.push_back(u);
    }
    qs(1,n);
    for (int i=1;i<=n;i++)
        cout << s[i] << endl;
    cout << endl;
    return 0;
}