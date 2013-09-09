#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

#include <cstdlib>

using namespace std;

void myAssert(bool condition, const char *msg)
{
	if (!condition)
	{
		cerr << "ASSERT " << msg;
		abort();
	}
}
#define myAssert(x) myAssert(x, "at line " __LINE__ " on condition "#x)

class ComparableToken
{
	string _raw;
	bool _isNumber;
	int _number;

public:
	explicit ComparableToken(const string &s): _raw(s)
	{
		_isNumber = isdigit(s[0]);
		if (_isNumber)
			_number = atoi(s.c_str());
	}

	const string& rawString() const { return _raw; }
	bool isNumber() const { return _isNumber; }
	int number() const { return _number; }
	
	bool operator < (const ComparableToken &t) const
	{
		if (_isNumber && t._isNumber && _number != t._number)
			return _number < t._number;
		return _raw < t._raw;
	}
};

vector<string> tokenize(const string &s)
{
	vector<string> result;
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
			result.push_back(cur);
			isNumber = !isNumber;
			cur = string(1, c);
		}
	}
	result.push_back(cur);
	return result;
}

vector<ComparableToken> buildVectorOfComparableTokens(const vector<string> &v)
{
	vector<ComparableToken> result;
	result.reserve(v.size());
	for (size_t i = 0; i < v.size(); ++i)
		result.push_back(ComparableToken(v[i]));
	return result;
}

class Solver
{
public:
	void solve(istream &in, ostream &out)
	{
		vector< vector<ComparableToken> > tokenLines;
		string line;
		while (in >> line)
		{
			cin >> line;
			tokenLines.push_back(buildVectorOfComparableTokens(tokenize(line)));
		}
		
		sort(tokenLines.begin(), tokenLines.end());
	
		for (auto it = tokenLines.begin(); it != tokenLines.end(); ++it)
		{
			for (auto jt = it->begin(); jt != it->end(); ++jt)
				out << jt->rawString();
			out << endl;
		}
	}
	string solve(const string &text)
	{
		istringstream in(text);
		ostringstream out;
		solve(in, out);
		return out.toString();
	}
};

int main()
{
	Solver solver;
	solver.solve(cin, cout);

	return 0;
}
