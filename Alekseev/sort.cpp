#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include <cstdlib>
#include <cassert>

using namespace std;

class Token
{
	string _raw;
	bool _isNumber;
    int _number;

public:
	explicit Token(const string &s): _raw(s)
	{
		assert(!s.empty());
		_isNumber = true;
		for (size_t i = 0; i < s.size(); ++i)
		{
			if (isdigit(s[i]))
				assert(_isNumber);
			else
			{
				assert(!i || !_isNumber);
				_isNumber = false;
			}
		}
		if (_isNumber)
			_number = atoi(s.c_str());
	}

	const string& rawString() const { return _raw; }
	bool isNumber() const { return _isNumber; }
	int number() const { return _number; }
	
	bool operator < (const Token &t) const
	{
		if (_isNumber && t._isNumber && _number != t._number)
			return _number < t._number;
		return _raw < t._raw;
	}
};

vector<Token> tokenize(const string &s)
{
	vector<Token> result;
	if (s.empty())
		return result;

	string cur;
	bool isNumber = isdigit(s[0]);
	for (size_t i = 0; i < s.size(); ++i)
	{
		char c = s[i];
		if ((bool)isdigit(c) == isNumber)
			cur += c;
		else
		{
			result.push_back(Token(cur));
			isNumber = !isNumber;
			cur = string(1, c);
		}
	}
	if (!cur.empty())
		result.push_back(Token(cur));
	return result;
}

int main()
{
	int n;
	cin >> n;

	vector< vector<Token> > tokenLines;
	tokenLines.reserve(n);
	for (int i = 0; i < n; ++i)
	{
		string line;
		cin >> line;
		tokenLines.push_back(tokenize(line));
	}
	
	sort(tokenLines.begin(), tokenLines.end());

	for (int i = 0; i < n; ++i)
	{
		for (vector<Token>::iterator it = tokenLines[i].begin(), end = tokenLines[i].end(); it != end; ++it)
			cout << it->rawString();
		cout << endl;
	}		

	return 0;
}
