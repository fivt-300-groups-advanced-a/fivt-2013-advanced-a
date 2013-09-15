#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <iterator>

#include <cstdlib>
#include <cstring>

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
	void solve(istream &in, ostream &out)
	{
		vector< vector<ComparableToken> > tokenLines;
		string line;
		while (in >> line)
		{
			cin >> line;
			vector<string> tokenized = tokenize(line);
			tokenLines.push_back(vector<ComparableToken>(tokenized.begin(), tokenized.end()));
		}
		
		sort(tokenLines.begin(), tokenLines.end());
	
		for (auto it = tokenLines.begin(); it != tokenLines.end(); ++it)
		{
		    copy(it->begin(), it->end(), ostream_iterator<string>(out, ""));
			out << endl;
		}
	}
	string solve(const string &data)
	{
		istringstream in(data);
		ostringstream out;
		solve(in, out);
		return out.str();
	}
};

template<class T>
ostream& operator << (ostream &out, const vector<T> &v)
{
	out << "{\n";
	copy(v.begin(), v.end(), ostream_iterator<T>(out, "\n"));
	return out << "}";
}

template<class Input, class Output>
bool checkAnswer(const Input &input, const Output &output, const Output &expected)
{
	if (output != expected)
	{
		cerr << "\tTest failed!\n";
		cerr << "On input:\n" << input << "\n";
		cerr << "Function returned:\n" << output << "\n";
		cerr << "Expected:\n" << expected << "\n";

		return false;
	}
	return true;
}

void testTokenize()
{
	static const int testsNumber = 7;
	static const string input[testsNumber] = 
	{
		"sdf344r", 
		"t43td", 
		"df1344", 
		"12", 
		"we",
		"",
		".gerf@*($#rg849sd2ddd2r0gi"
	};
	static const vector<string> output[testsNumber] = 
	{
		{"sdf", "344", "r"}, 
		{"t", "43", "td"},
		{"df", "1344"},
		{"12"},
		{"we"},
		{},
		{".gerf@*($#rg", "849", "sd", "2", "ddd", "2", "r", "0", "gi"}
	};

	cerr << "Testing tokenize()...\n";
	int fails = 0;
	for (int i = 0; i < testsNumber; ++i)
		if (!checkAnswer(input[i], tokenize(input[i]), output[i]))
			++fails;
	cerr << "Finished testing tokenize(): ";
	if (fails)
		cerr << "FAILED " << fails << "/" << testsNumber << endl;
	else
		cerr << "SUCCESS\n";
}

int main(int argc, char **argv)
{
	if (argc > 1 && !strcmp(argv[1], "--test"))
	{
		testTokenize();
		return 0;
	}
	Solver solver;
	solver.solve(cin, cout);

	return 0;
}
