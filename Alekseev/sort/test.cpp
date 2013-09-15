#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>

#include "solution.h"

using namespace std;

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

void tokenizeTest()
{
	static const vector< pair<string, vector<string> > > tests
	{
		{"sdf344r", 
		{"sdf", "344", "r"}},

		{"t43td", 
		{"t", "43", "td"}},
		
		{"df1344", 
		{"df", "1344"}},

		{"12", 
		{"12"}},

		{"we", 
		{"we"}},

		{"", 
		{}},

		{".gerf@*($#rg849sd2ddd2r0gi", 
		{".gerf@*($#rg", "849", "sd", "2", "ddd", "2", "r", "0", "gi"}}
	};

	cerr << "Testing tokenize()...\n";
	int fails = 0;
	for (auto test : tests)
		if (!checkAnswer(test.first, tokenize(test.first), test.second))
			++fails;
	cerr << "Finished testing tokenize(): ";
	if (fails)
		cerr << "FAILED " << fails << "/" << tests.size() << "tests\n";
	else
		cerr << "SUCCESS\n";
}

void integrationTest()
{
	static const vector< pair< vector<string>, vector<string> > > tests
	{
		{
			{"acab.txt", "3.1415", "3.acab.1415"},
			{"3.1415", "3.acab.1415", "acab.txt"}
		},
		{
			{"", "a1b2c3.d4", ".32r"},
			{"", ".32r", "a1b2c3.d4"}
		}
	};

	cerr << "Testing integration...\n";
	int fails = 0;
	for (auto test : tests)
	{
		Solver solver;
		if (!checkAnswer(test.first, solver.sortNames(test.first), test.second))
			++fails;
	}
	cerr << "Finished integration test: ";
	if (fails)
		cerr << "FAILED " << fails << "/" << tests.size() << "tests\n";
	else
		cerr << "SUCCESS\n";
}

int main()
{
	tokenizeTest();
	integrationTest();

	return 0;
}
