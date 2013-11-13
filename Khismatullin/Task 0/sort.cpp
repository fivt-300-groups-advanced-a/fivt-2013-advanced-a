#include <cstdio>
#include <limits.h>
#include <iostream>
//#include <cmath>
#include <algorithm>
#include <string.h>
#include <cstring>
#include <string>
#include <vector>
//#include <queue>
//#include <set>
//#include <map>

#define pb push_back


typedef long long ll;

using namespace std;


bool is_digit(char c) 
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
	for (int i = 0; i < min(a.size(), b.size()); i++) {
		if (is_digit(a[i][0]) != is_digit(b[i][0])) {
			return is_digit(b[i][0]);
		}
		if (a[i] == b[i]) 
			continue;
		if (!is_digit(a[i][0])) 
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

vector<string> parse(const string &s) {
	vector<string> cur;
	int i = 0;
	while (i < s.length()) {
		int l = i;
		while ((i < s.length()) && !is_digit(s[i])) {
			i++;
		}
		if (l != i) {
			cur.pb(s.substr(l, i - l));
		}
		l = i;
		while ((i < s.length()) && is_digit(s[i])) {
			i++;
		}
		if (l != i) {
			cur.pb(s.substr(l, i - l));
		}
	}
	
	return cur;
}

void solve(){
	int n;
	cin >> n;
	string s;
	for(int i = 0; i < n; ++i) {
		cin >> s;
		a.pb(parse(s));
	}
	
	sort(a.begin(), a.end(), &cmp);
	for (int i = 0; i < n; i++) {
		print(a[i]);
	}
}

void verdict(string wr, string v) {
	cout << wr;
	for(int i = 0; i < 30 - wr.size(); ++i) {
		cout << ' ';
	}
	cout << "| ";
	cout << v << endl;
	if (v != "OK")
		exit(0);
}

void test_is_digit(char c, bool t) {

	string wr = "Test: ";
	wr.pb(c);
	wr.pb(' ');
	if (is_digit(c) == t) {
		verdict(wr, "OK");
	} else {
		verdict(wr, "WA");
	}
}

void unit_tests_is_digit(){
	cout << ">> void is_digit(char c) testing." << endl;
	test_is_digit('0', true);
	test_is_digit('9', true);
	test_is_digit('1', true);
	test_is_digit('5', true);
	test_is_digit('a', false);
	test_is_digit('#', false);
	test_is_digit('O', false);
	cout << ">> void is_digit(char c) passed the tests successfully." << endl << endl;
	cout << endl;
	
}

void test_is_less(string a, string b, bool t) {

	string wr = "Test: ";
	wr.append(a);
	while (wr.size() < 15) 
		wr.pb(' ');
	wr.append(b);
	wr.pb(' ');
	if (is_less(a, b) == t) {
		verdict(wr, "OK");
	} else {
		verdict(wr, "WA");
	}
}

void unit_tests_is_less() {
	cout << ">> bool is_less(string &a, string &b) testing." << endl;
	test_is_less("0", "1", true);
	test_is_less("0", "0", false);
	test_is_less("1", "0", false);
	test_is_less("01", "1", true);
	test_is_less("001", "01", true);
	test_is_less("12", "2", false);
	test_is_less("000009", "5", false);
	test_is_less("010004", "5", false);
	cout << ">> bool is_less(string &a, string &b) passed the tests successfully." << endl << endl;
	cout << endl;
	
}

void test_cmp(vector<string> a, vector<string> b, bool t) {

	string wr = "Test: ";
	for (int i = 0; i < a.size(); i++) {
		wr.append(a[i]);
	}
	while (wr.size() < 15) {
		wr.pb(' ');
	}
	for (int i = 0; i < b.size(); i++) {
		wr.append(b[i]);
	}
	wr.pb(' ');
	if (cmp(a, b) == t) {
		verdict(wr, "OK");
	} else {
		verdict(wr, "WA");
	}
}

void unit_tests_cmp() {
	cout << ">> bool cmp(vector<string> a, vector<string> b) testing." << endl;
	test_cmp(parse("0"), parse("0"), false);
	test_cmp(parse("a"), parse("a"), false);
	test_cmp(parse("ab1"), parse("a12"), false);
	test_cmp(parse("ab1"), parse("ab12"), true);
	test_cmp(parse("ab12"), parse("ab12"), false);
	test_cmp(parse("ab12a"), parse("ab12"), false);
	test_cmp(parse("ab12a"), parse("ab12a1"), true);
	test_cmp(parse("ab1"), parse("ab12"), true);
	test_cmp(parse("1ab"), parse("ab12"), false);
	test_cmp(parse("ab12a"), parse("ab12b"), true);
	test_cmp(parse("aa"), parse("aa12a1"), true);
	test_cmp(parse("ab012a"), parse("ab12a1"), true);
	test_cmp(parse("a1a"), parse("a1"), false);
	test_cmp(parse("qwe"), parse("qwer"), true);
	cout << ">> bool cmp(vector<string> a, vector<string> b) passed the tests successfully." << endl << endl;
	cout << endl;
	
}

void test_parse(string a, vector<string> b) {
	string wr = "Test: ";
	wr.append(a);
	for(int i = 0; i < 10 - a.length(); i++) 
		wr.pb(' ');
	for (int i = 0; i < b.size(); i++) {
		wr.append(b[i]);
		wr.pb(' ');
	}
	vector<string> c = parse(a);
	bool flag = (c.size() == b.size());
	for (int i = 0; i < c.size(); ++i) {
		flag = (flag && (b[i] == c[i]));
	}
	if (flag) {
		verdict(wr, "OK");
	} else {
		verdict(wr, "WA");
	}
}

void unit_tests_parse() 
{
	cout << ">> vector<string> parse(const string &s) testing." << endl;
	vector<string> a;
	a.pb("1");
	test_parse("1", a);
	a.pb("a");
	test_parse("1a", a);
	a.pb("09");
	test_parse("1a09", a);
	a.pb("bcd");
	test_parse("1a09bcd", a);
	a.clear();
	a.pb("asd");
	test_parse("asd", a);
	a.pb("000");
	test_parse("asd000", a);
	a.pb("q");
	test_parse("asd000q", a);
	a.pb("5");
	test_parse("asd000q5", a);
	a.pb("r");
	test_parse("asd000q5r", a);	
	cout << ">> vector<string> parse(const string &s) passed the tests successfully." << endl << endl;
	cout << endl;

}

void unit_testing() {
	unit_tests_is_digit();
	unit_tests_is_less();
	unit_tests_parse();
	unit_tests_cmp();
	cout << "Unit tests passed!" << endl << "You are amazing!" << endl;
}

int main()
{
	//solve();
	unit_testing();
	return 0;
}
