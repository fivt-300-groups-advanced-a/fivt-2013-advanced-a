#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <string.h>

typedef long long ll;

using namespace std;

ifstream cin("input.txt");
ofstream cout("output.txt");


class elem
{
private:
	string str;
	ll x;
	bool isString;
public:
	elem(string s)
	{
		isString = !isdigit(s[0]);
		if (!isString)
		{
			x = 0;
			for (int i = 0; i < s.length(); ++i)
				x = x * 10 + (ll)(s[i] - '0');
		}
		str = s;
	}

	bool isStr()
	{
		return isString;
	}

	string getstr()
	{
		return this->str;
	}

	bool operator<(const elem &el) const
	{
		if (this->isString && !el.isString)
			return true;
		if (!this->isString && el.isString)
			return false;
		if (this->isString)
			return this->str < el.str;
		return this->x < el.x;
	}
};

vector<elem> build(vector<string> v)
{
	vector<elem> r;
	for (int i = 0; i < v.size(); ++i)
		r.push_back(elem(v[i]));
	return r;
}

vector<string> split(string s)
{
	vector<string> v;
	string e;
	for (int i = 0; i < s.length(); ++i)
	{
		if (i == 0 || (isdigit(e[0]) != isdigit(s[i])))
		{
			if (i != 0)
				v.push_back(e);
			e = s[i];
		}
		else
			e += s[i];
	}
	if (e.size() > 0)
		v.push_back(e);
	return v;
}


#ifdef TEST_MODE
int main()
{
	vector<string> v;
	v.push_back("1");
	v.push_back("abs.,");
	v.push_back("234");
	v.push_back("jj");
	v.push_back("2");
	v.push_back("gZ");
	v.push_back("1");
	cout << "TEST1 -> " << (split("1abs.,234jj2gZ1") == v ? "OK" : "FAIL") << endl;
	v.clear();
	cout << "TEST2 -> " << (split("") == v ? "OK" : "FAIL") << endl;
	v.push_back("123");
	cout << "TEST3 -> " << (split("123") == v ? "OK" : "FAIL") << endl;
	cout << "TEST4 -> " << (elem("123").isStr() == false ? "OK" : "FAIL") << endl;
	cout << "TEST5 -> " << (elem("abc").isStr() == true ? "OK" : "FAIL") << endl;
	cout << "TEST6 -> " << (elem("123").getstr() == "123" ? "OK" : "FAIL") << endl;
	cout << "TEST7 -> " << ((elem("abc") < elem("123")) == true ? "OK" : "FAIL") << endl;
	cout << "TEST8 -> " << ((elem("011") < elem("123")) == true ? "OK" : "FAIL") << endl;
	cout << "TEST9 -> " << ((elem("1") < elem("123")) == true ? "OK" : "FAIL") << endl;
	cout << "TEST10 -> " << ((elem("abc") < elem("abd")) == true ? "OK" : "FAIL") << endl;
	return 0;
}
#else

int main()
{
	int n;
	cin >> n;
	string s;
	getline(cin, s);
	vector<vector<elem> > v;
	for (int i = 0; i < n; ++i)
	{
		getline(cin, s);
		v.push_back(build(split(s)));
	}
	sort(v.begin(), v.end());
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < v[i].size(); ++j)
			cout << v[i][j].getstr();
		cout << endl;
	}
}
#endif
