#ifndef SOLUTION_H
#define SOLUTION_H

#include <algorithm>
#include <string>
#include <vector>

using namespace std;

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
	
	operator const string&() const
	{
		return _raw;
	}
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

class Solver
{
public:
	vector<string> sortNames(const vector<string> &names)
	{
		vector< vector<ComparableToken> > tokenLines;
		tokenLines.reserve(names.size());
		for (auto name : names)
		{
			vector<string> tokenized = tokenize(name);
			tokenLines.push_back(vector<ComparableToken>(tokenized.begin(), tokenized.end()));
		}
		
		sort(tokenLines.begin(), tokenLines.end());
	
	    vector<string> result;
	    result.reserve(names.size());
		for (auto tokenizedName : tokenLines)
		{
			result.push_back("");
			for (ComparableToken token : tokenizedName)
				result.back().append(token.rawString());
		}
		return result;
	}
};

#endif // SOLUTION_H