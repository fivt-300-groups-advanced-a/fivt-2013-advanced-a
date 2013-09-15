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
		cerr << "FAILED " << fails << "/" << testsNumber << "tests\n";
	else
		cerr << "SUCCESS\n";
}

void integrationTest()
{
	static const int testsNumber = 2;
	static const vector<string> input[testsNumber] = 
	{
		{"acab.txt", "3.1415", "3.acab.1415"},
		{"", "a1b2c3.d4", ".32r"}
	};
	static const vector<string> output[testsNumber] = 
	{
		{"3.1415", "3.acab.1415", "acab.txt"},
		{"", ".32r", "a1b2c3.d4"}
	};

	cerr << "Testing integration...\n";
	int fails = 0;
	for (int i = 0; i < testsNumber; ++i)
	{
		Solver solver;
		if (!checkAnswer(input[i], solver.sortNames(input[i]), output[i]))
			++fails;
	}
	cerr << "Finished integration test: ";
	if (fails)
		cerr << "FAILED " << fails << "/" << testsNumber << "tests\n";
	else
		cerr << "SUCCESS\n";
}

int main()
{
	tokenizeTest();
	integrationTest();

	return 0;
}
