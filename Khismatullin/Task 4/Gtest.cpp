#define TESTER
#include "gtest/gtest.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <string>
using namespace std;

#define pb push_back 

ofstream out;
ifstream in;
bool LOG = false;

int solve(string &s1, int l1, int r1, string &s2, int l2, int r2)
{
	
	if (r2 - l2 == 0 || r1 - l1 == 0)
	{
		if (LOG) {
			if (r2 - l2 == 0)
				for(int i = 0; i < r1 - l1; i++)
					out << "Delete " << s1[i + l1] << "\n";
			
			if (r1 - l1 == 0)
				for(int i = 0; i < r2 - l2; i++)
					out << "Insert " << s2[i + l2] << "\n";
		}


		return r1 - l1 + r2 - l2;
	}
	
	if (r1 - l1 == 1)
    {
		bool matched = false;
		for(int i = l2; i < r2 - 1; i++)
		{
			if (!matched && s1[l1] == s2[i]) {
				matched = true;
				if (LOG) {
					out << "Match " << s1[l1] << "\n";
				}
			} else {
				if (LOG) {
					out << "Insert " << s2[i] << "\n";
				}
			}
		}
		if (matched) {
			if (LOG) {
				out << "Insert " << s2[r2 - 1] << "\n";
			}
			return r2 - l2 - 1;
		} else {
			if (s1[l1] == s2[r2 - 1]) {
				if (LOG) {
					out << "Match " << s1[l1] << "\n";
				}
				return r2 - l2 - 1;
			} else {
				if (LOG) {
					out << "Replace " << s1[l1] << " to " << s2[r2 - 1] << "\n";
				}
				return r2 - l2;
			}
		}
	}
	
	int m = (l1 + r1) / 2;
	
	int* d = new int[r2 - l2 + 1];
	for(int i = 0; i <= r2 - l2; i++)
	{
		d[i] = i;
	}
	for(int i = l1; i < m; i++)
	{
		int last = i - l1 + 1;
		for(int j = 1; j <= r2 - l2; j++)
		{
			if (s1[i] == s2[l2 + j - 1]) {
				int current = d[j - 1];
				d[j - 1] = last;
				last = current;
			} else {
				int current = min(d[j - 1], min(last, d[j])) + 1;
				d[j - 1] = last;
				last = current;
			}
		}
		d[r2 - l2] = last;
	}
	int* e = new int[r2 - l2 + 1];
	for(int i = 0; i <= r2 - l2; i++)
	{
		e[i] = i;
	}
	for(int i = r1 - 1; i >= m; i--)
	{
		int last = r1 - i;
		for(int j = 1; j <= r2 - l2; j++)
		{
			if (s1[i] == s2[r2 - j]) {
				int current = e[j - 1];
				e[j - 1] = last;
				last = current;
			} else {
				int current = min(e[j - 1], min(last, e[j])) + 1;
				e[j - 1] = last;
				last = current;
			}
		}
		e[r2 - l2] = last;
	}
	
	int k = 0;
	for(int i = 0; i <= r2 - l2; i++)
	{
		if (d[i] + e[r2 - l2 - i] < d[k] + e[r2 - l2 - k])
			k = i;
	}
	delete[] d;
	delete[] e;
	return solve(s1, l1, m, s2, l2, l2 + k) + solve(s1, m, r1, s2, l2 + k, r2);
}


int stupid(string &s1, string &s2)
{
	
	int d[s2.length() + 1];
	for(int i = 0; i <= s2.length(); i++)
	{
		d[i] = i;
	}
	for(int i = 0; i < s1.length(); i++)
	{
		int last = i + 1;
		for(int j = 1; j <= s2.length(); j++)
		{
			if (s1[i] == s2[j - 1]) {
				int current = d[j - 1];
				d[j - 1] = last;
				last = current;
			} else {
				int current = min(d[j - 1], min(last, d[j])) + 1;
				d[j - 1] = last;
				last = current;
			}
		}
		d[s2.length()] = last;
	}
	return d[s2.length()];
}

bool check_log(string &s1, string &s2)
{
	in.open("log.txt");
	string s;
	int i = 0, j = 0;
	while (in >> s)
	{
		if (s == "Match") {
			char c;
			in >> c;
			if (s1[i] != c || s1[i] != s2[j])
				return false;
			i++, j++;
		}
		if (s == "Insert") {
			char c;
			in >> c;
			if (s2[j] != c)
				return false;
			j++;
		}
		if (s == "Replace") {
			char c1, c2;
			in >> c1;
			in >> s;
			in >> c2;
			if (s1[i] != c1 || s2[j] != c2)
				return false;
			i++, j++;
		}
		if (s == "Delete") {
			char c;
			in >> c;
			if (s1[i] != c)
				return false;
			i++;
		}
	}
	in.close();
	return (i == s1.length() || j == s2.length());
}

inline void check(string &s1, string &s2)
{
	if (LOG)
	{
		out.open("log.txt");
		solve(s1, 0, s1.length(), s2, 0, s2.length());
		out.close();
		ASSERT_TRUE(check_log(s1, s2));
	} else 
		ASSERT_EQ(solve(s1, 0, s1.length(), s2, 0, s2.length()), stupid(s1, s2));
}

TEST(Manual_test, Small_test)
{
		string s1 = "";
		string s2 = "";
		check(s1, s2);
		s2 = "qwerty";
		check(s1, s2);
		s2 = "";
		s1 = "qwerty";
		check(s1, s2);
		s2 = "q";
		check(s1, s2);
		s2 = "qwerty";
		s1 = "q";
		check(s1, s2);
		s1 = "qwerty";
		
		check(s1, s2);
		
}

TEST(Stress_test, small_test_1)
{
	string s1;
	string s2;
	for(int test = 0; test < 10000; test++)
	{
		s1 = "";
		s2 = "";
		int n = rand()%15;
		int m = rand()%15;
		for(int i = 0; i < n; i++)
			s1.pb('a' + rand() % 26);
		for(int i = 0; i < m; i++)
			s2.pb('a' + rand() % 26);
		check(s1, s2);		
	}
}

TEST(Stress_test, small_test_2)
{
	string s1;
	string s2;
	for(int test = 0; test < 10000; test++)
	{
		s1 = "";
		s2 = "";
		int n = rand()%15;
		int m = rand()%15;
		for(int i = 0; i < n; i++)
			s1.pb('a' + rand() % 3);
		for(int i = 0; i < m; i++)
			s2.pb('a' + rand() % 3);
		check(s1, s2);		
	}
}

TEST(Stress_test, big_test_1)
{
	string s1;
	string s2;
	for(int test = 0; test < 100; test++)
	{
		s1 = "";
		s2 = "";
		int n = rand()%501 + 500;
		int m = rand()%501 + 500;
		for(int i = 0; i < n; i++)
			s1.pb('a' + rand() % 26);
		for(int i = 0; i < m; i++)
			s2.pb('a' + rand() % 26);
		check(s1, s2);		
	}
}

TEST(Stress_test, big_test_2)
{
	string s1;
	string s2;
	for(int test = 0; test < 100; test++)
	{
		s1 = "";
		s2 = "";
		int n = rand()%501 + 500;
		int m = rand()%501 + 500;
		for(int i = 0; i < n; i++)
			s1.pb('a' + rand() % 3);
		for(int i = 0; i < m; i++)
			s2.pb('a' + rand() % 3);
		check(s1, s2);		
	}
}

TEST(Stress_test, very_big_test)
{
	string s1;
	string s2;
	s1 = "";
	s2 = "";
	int n = rand()%501 + 9500;
	int m = rand()%501 + 9500;
	for(int i = 0; i < n; i++)
		s1.pb('a' + rand() % 26);
	for(int i = 0; i < m; i++)
		s2.pb('a' + rand() % 26);
	check(s1, s2);		
}

TEST(Check_log, small_test_1)
{
	LOG = true;
	string s1;
	string s2;
	for(int test = 0; test < 1000; test++)
	{
		s1 = "";
		s2 = "";
		int n = rand()%15;
		int m = rand()%15;
		for(int i = 0; i < n; i++)
			s1.pb('a' + rand() % 26);
		for(int i = 0; i < m; i++)
			s2.pb('a' + rand() % 26);
		//cout << s1 << endl << s2 << endl;
		check(s1, s2);		
	}
}

TEST(Check_log, small_test_2)
{
	string s1;
	string s2;
	for(int test = 0; test < 1000; test++)
	{
		s1 = "";
		s2 = "";
		int n = 100 + rand()%15;
		int m = 100 + rand()%15;
		for(int i = 0; i < n; i++)
			s1.pb('a' + rand() % 2);
		for(int i = 0; i < m; i++)
			s2.pb('a' + rand() % 2);
		check(s1, s2);		
	}
}

int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}