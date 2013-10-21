#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <string.h>
#include <queue>
#include <stack>
#include <deque>
#include <map>
#include <set>
#include <cmath>
#include <sstream>
#include <ctime>

#define pb push_back
#define mp make_pair
#define PI 3.1415926535897932384626433832795
#define ALL(x) x.begin(), x.end()
#define F first
#define S second
#define m0(x) memset(x,0,sizeof(x))
#define m1(x) memset(x,-1,sizeof(x))
#define pw(x) (1ull<<(x))

using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;
typedef pair<int,int> pii;
const int INF = 2147483647;
const ll LLINF = 9223372036854775807LL;

struct block {
  bool is_string;
  string str;
  ll num;

  block() {}
  block(string s) : str(s), is_string(true) { }
  block(ll n) : num(n), is_string(false) { }

  bool operator < (const block &a) const {
    if (is_string==a.is_string) {
      if (is_string) return str<a.str;
      return num<a.num;
    }
    else {
      if (is_string) return true;
      return false;
    }
  }

  void print() {
    if (is_string) cout << str; else cout << num;
  }

};

struct part {
  vector<block> arr;

  part(string s) {
    ll curnum = 0;
    string curstr = "";
    for (int i=0;i<s.size();i++) {
      if (s[i]>='0' && s[i]<='9') {
        if (curstr!="") arr.pb(block(curstr));
        curstr="";
        curnum = curnum*10+(s[i]-'0');
      }
      else {
        if (curnum!=0) arr.pb(block(curnum));
        curnum = 0;
        curstr+=s[i];
      }
    }
    if (curnum!=0) arr.pb(block(curnum));
    if (curstr!="") arr.pb(block(curstr));
  }

  bool operator < (const part &a) const {
    for (int i=0;i<min((int)arr.size(), (int)a.arr.size());i++) {
      if (arr[i]<a.arr[i]) return true;
      if (a.arr[i]<arr[i]) return false;
    }
    if (arr.size()>a.arr.size()) return false;
    return true;
  }

  void print() {
    for (int j=0;j<arr.size();j++) arr[j].print();
  }

};

int co = 0;
vector< part > strings;

void read_data() {
  cin >> co;
  for (int i=0;i<co;i++) {
    string s; cin >> s;
    strings.pb(part(s));
  }
}

void sort_data() {
  sort(strings.begin(), strings.end());
}

void output_data() {
  for (int i=0;i<co;i++) {
    strings[i].print(); cout << "\n";
  }
}

int main() {
  //freopen("input.txt", "r", stdin);
  //freopen("output.txt", "w", stdout);
  read_data();
  sort_data();
  output_data();
  return 0;
}
