#include <string>
#include <iostream>
#include <fstream>

using namespace std;

#define LOG

#ifdef LOG
	ofstream out("log.txt");
#endif

int solve(string &s1, int l1, int r1, string &s2, int l2, int r2)
{
	
	if (r2 - l2 == 0 || r1 - l1 == 0)
	{
		#ifdef LOG
		if (r2 - l2 == 0)
			for(int i = 0; i < r1 - l1; i++)
				out << "Delete " << s1[i + l1] << "\n";
		
		if (r1 - l1 == 0)
			for(int i = 0; i < r2 - l2; i++)
				out << "Insert " << s2[i + l2] << "\n";
		#endif


		return r1 - l1 + r2 - l2;
	}
	
	if (r1 - l1 == 1)
    {
		bool matched = false;
		for(int i = l2; i < r2 - 1; i++)
		{
			if (!matched && s1[l1] == s2[i]) {
				matched = true;
				#ifdef LOG
				out << "Match " << s1[l1] << "\n";
				#endif
			} else {
				#ifdef LOG
				out << "Insert " << s2[i] << "\n";
				#endif
			}
		}
		if (matched) {
			#ifdef LOG
			out << "Insert " << s2[r2 - 1] << "\n";
			#endif
			return r2 - l2 - 1;
		} else {
			if (s1[l1] == s2[r2 - 1]) {
				#ifdef LOG
				out << "Match " << s1[l1] << "\n";
				#endif
				return r2 - l2 - 1;
			} else {
				#ifdef LOG
				out << "Replace " << s1[l1] << " to " << s2[r2 - 1] << "\n";
				#endif
				return r2 - l2;
			}
		}
	}
	
	int m = (l1 + r1) / 2;
	
	int d[r2 - l2 + 1];
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
	int e[r2 - l2 + 1];
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
	return solve(s1, l1, m, s2, l2, l2 + k) + solve(s1, m, r1, s2, l2 + k, r2);
}
