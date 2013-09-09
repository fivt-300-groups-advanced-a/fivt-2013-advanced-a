#include <cstdio>
#include <limits.h>
#include <iostream>
//#include <cmath>
#include <algorithm>
#include <string>
#include <vector>
//#include <queue>
//#include <set>
//#include <map>

#define pb push_back


typedef long long ll;

using namespace std;

bool dig(char c) 
{
	return (c >= '0' && c <= '9');
}

bool is_less(string &a, string &b) {
	if (a.length() > b.length())
		return (!is_less(b, a));
	for (int i = 0; i < b.length() - a.length(); i++) {
		if (b[i] != '0') {
			return true;
		}
	}
	return (a < b.substr(b.length() - a.length(), a.length()));
}

bool cmp(vector<string> a, vector<string> b) 
{
	for (int i = 0; i <= min(a.size(), b.size()); i++) {
		if (dig(a[i][0]) != dig(b[i][0])) {
			return dig(b[i][0]);
		}
		if (a[i] == b[i]) 
			continue;
		if (!dig(a[i][0])) 
			return (a[i] < b[i]);
		return (is_less(a[i], b[i]));
	}
	return (a.size() < b.size());
}

vector< vector<string> > a;

void print(vector<string> a) {
	for (int i = 0; i < a.size(); i++) {
		cout << a[i];
	}
	cout << endl;
}


int main()
{
	int n;
	cin >> n;
	string s;
	for (int i = 0; i < n; ++i) {
		vector<string> cur;
		cin >> s;
		int i = 0;
		while (i < s.length()) {
			int l = i;
			while ((i < s.length()) && !dig(s[i])) {
				i++;
			}
			if (l != i) {
				cur.pb(s.substr(l, i - l));
			}
			l = i;
			while ((i < s.length()) && dig(s[i])) {
				i++;
			}
			if (l != i) {
				cur.pb(s.substr(l, i - l));
			}
		}
		a.pb(cur);
	}
	
	sort(a.begin(), a.end(), &cmp);
	for (int i = 0; i < n; i++) {
		print(a[i]);
	}
	
	return 0;
}
