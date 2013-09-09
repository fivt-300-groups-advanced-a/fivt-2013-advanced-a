#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

#define EPS (1e-9)
#define INF (1000000000)

#define pb push_back
#define sz(a) (int)(a).size()

using namespace std;

void solve();

int main(){
    int n = 1;
    for (int i = 0; i < n; i++)
        solve();
}

vector <string> s[100100];
int n, type = INF;
string t, h;

bool check(char ch){
	if (ch >= '0' && ch <= '9')
		return 1;
	else
		return 0;
}

void save(int x){
	s[x].pb(h);
	h = "";
}

void tokenize(int x){
	h = "";
	type = INF;
	for (int j = 0; j < (int)t.length(); j++){
		if (check(t[j])){
			if (!type && h != "")
				save(x);
			h += t[j];
			type = 1;
		}
		if (!check(t[j])){
			if (type && h != "")
				save(x);
			h += t[j];
			type = 0;
		}
	}
	save(x);
}

bool comp(vector <string> x, vector <string> y){
	for (int i = 0; i < min(sz(x), sz(y)); i++){
		if (check(x[i][0]) && !check(y[i][0]))
			return 0;
		if (!check(x[i][0]) && check(y[i][0]))
			return 1;
		if (!check(x[i][0]) && !check(y[i][0])){
			if (x[i].compare(y[i]) > 0)
				return 0;
			if (x[i].compare(y[i]) < 0)	
				return 1;
		}
		if (check(x[i][0]) && check(y[i][0])){
			double a = (double)atoi(x[i].c_str()), b = (double)atoi(y[i].c_str());
			int cc = 0;
			while (cc < (int)x[i].length() && x[i][0] == '0')
				cc++;
			a -= EPS * cc;
			cc = 0;
			while (cc < (int)y[i].length() && y[i][0] == '0')
				cc++;
			b -= EPS * cc;
			if (a > b)
				return 0;
			if (b > a)
				return 1;
		}
	}
	if (sz(x) < sz(y))
		return 1;
	else
		return 0;
}

void out(vector <string> x){
	for (int i = 0; i < sz(x); i++)
		cout << x[i];
	cout << endl;
}

void solve(){
	cin >> n;
	for (int i = 0; i < n; i++){
		cin >> t;
		tokenize(i);
	}
	sort(s, s + n, comp);
	for (int i = 0; i < n; i++){
		out(s[i]);
	}
}
