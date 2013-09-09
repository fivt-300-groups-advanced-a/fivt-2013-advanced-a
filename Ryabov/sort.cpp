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

vector<elem> build(string s)
{
	vector<elem> v;
	string e;
	for (int i = 0; i < s.length(); ++i)
	{
		if (i == 0 || (isdigit(e[0]) != isdigit(s[i])))
		{
			if (i != 0)
				v.push_back(elem(e));
			e = s[i];
		}
		else
			e += s[i];
	}
	v.push_back(elem(e));
	return v;
}

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
		v.push_back(build(s));
	}
	sort(v.begin(), v.end());
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < v[i].size(); ++j)
			cout << v[i][j].getstr();
		cout << endl;
	}
}
