#include <string>
#include <vector>
#include <cstdio>
#include "gtest/gtest.h"

using namespace std;

#define DEL_FIRST -1
#define DEL_SECOND -2
#define fore(i, b, e) for (int i = (int)b; i <= (int)e; i++)
#define forn(i, n) for (int i = 0; i < (int)n; i++)

typedef vector <pair<int, int> > pairVector;

const int inf =  1e9;


inline void upd(int & x, int y)
{
	x = min(x, y);
}

inline void printAns(const pairVector & ans, const string & t)
{
	forn(j, ans.size())
	{
		printf("symbol number %d ", ans[j].first);
		ASSERT_TRUE((ans[j].second >= 0 || ans[j].second == DEL_FIRST || ans[j].second == DEL_SECOND));
		if (ans[j].second >= 0)
			printf("changed to %c\n", t[ans[j].second]);
		else if (ans[j].second == DEL_FIRST)
			printf("deleted from first string\n");
		else
		{
			printf("deleted from second string\n");
		}
	}
}

inline int solveEasy(const string & s, const string & t)
{
	vector < vector <int> >  dp(s.length() + 1);
	fore(i, 0, s.length())
		dp[i].resize(t.length() + 1);
	dp[0][0] = 0;
	fore(i, 0, s.length())
	{
		fore(j, 0, t.length()) if (i != 0 || j != 0)
		{
			dp[i][j] = inf;
			if (i > 0 && j > 0)
				dp[i][j] = dp[i - 1][j - 1] + (s[i - 1] == t[j - 1] ? 0 : 1);
			if (i > 0)
				dp[i][j] = min(dp[i][j], dp[i - 1][j] + 1);
			if (j > 0)
				dp[i][j] = min(dp[i][j], dp[i][j - 1] + 1);
		}
	}
	return dp[s.length()][t.length()];
}

bool checkAns(const string & s, const string & t, pairVector & ans)
{
	vector <bool> to_del_first(s.length(), false);
	vector <bool> to_del_second(t.length(), false);
	string s_changed = s;
	fore(j, 0, ans.size() - 1)
	{
		int pos1 = ans[j].first;
		int pos2 = ans[j].second;
		if (pos2 == -1)
		{
			EXPECT_FALSE((pos1 < 0 || pos1 >= s.length()));
			to_del_first[pos1] = true;
		}
		else if (pos2 == -2)
		{
			EXPECT_FALSE(pos1 < 0 || pos1 >= t.length());
			to_del_second[pos1] = true;
		}
		else
		{
			EXPECT_FALSE(pos1 < 0 || pos1 >= s.length() || pos2 < 0 || pos2 >= t.length());
			s_changed[pos1] = t[pos2];
		}
	}
	string s1 = "";
	forn(j, s.length())
		if (!to_del_first[j])
			s1 += s_changed[j];
	string t1 = "";
	forn(j, t.length())
		if (!to_del_second[j])
			t1 += t[j];
	return s1 == t1;
}

int solveHirshberg(const string & s, const string & t, int start_s, int finish_s, int start_t, int finish_t, pairVector & ans)
{
	if (start_t > finish_t)
	{
		if (start_s > finish_s)
			return 0;
		for(int pos = start_s; pos <= finish_s; pos++)
			ans.push_back(make_pair(pos, DEL_FIRST));
		return finish_s - start_s + 1;
	}
	if (start_s > finish_s)
	{
		for (int pos = start_t; pos <= finish_t; pos++)
			ans.push_back(make_pair(pos, DEL_SECOND));
		return finish_t - start_t + 1;
	}
	if (start_s == finish_s && start_t == finish_t)
	{
		if (s[start_s] != t[start_t])
		{
			ans.push_back(make_pair(start_s, start_t));
			return 1;
		}
		return 0;
	}
	
	vector <int> dp_front0(finish_t + 2, 0), dp_front1(finish_t + 2, 0), dp_back0(finish_t + 2, 0), dp_back1(finish_t + 2, 0);

	int middle_s = (start_s + finish_s) / 2;
	//dp_front0.resize(finish_t + 1);
	//forn(j, finish_t + 1)
	//	dp_front0[j] = 0;
	fore(i, start_s, middle_s)
	{
		fore(j, start_t, finish_t)
		{
			dp_front1[j] = inf;
			int g;
			if (i - 1 < start_s)
				g = j - start_t;
			else if (j - 1 < start_t)
				g = i - start_s;
			else g = dp_front0[j - 1];
			upd(dp_front1[j], g + (s[i] == t[j] ? 0 : 1));
			if (i > start_s)
				upd(dp_front1[j], dp_front0[j] + 1);
			if (j > start_t)
				upd(dp_front1[j], dp_front1[j - 1] + 1);
		}
		dp_front0 = dp_front1;
	}
	//dp_back0.resize(finish_t + 1);
	//forn(j, finish_t + 1)
	//	dp_back0[j] = 0;
	for (int i = finish_s; i > middle_s; i--)
	{
		for (int j = finish_t; j >= start_t; j--)
		{
			dp_back1[j] = inf;
			int g;
			if (i + 1 > finish_s)
				g = finish_t - j;
			else if (j + 1 > finish_t)
				g = finish_s - i;
			else g = dp_back0[j + 1];
			upd(dp_back1[j], g + (s[i] == t[j] ? 0 : 1));
			if (i < finish_s)
				upd(dp_back1[j], dp_back0[j] + 1);
			if (j < finish_t)
				upd(dp_back1[j], dp_back1[j + 1] + 1);
		}
		dp_back0 = dp_back1;
	}
	int best_sum = inf;
	int middle_t;
	fore(i, start_t - 1, finish_t)
	{
		int front = (i == start_t - 1 ? middle_s - start_s + 1 : dp_front0[i]);
		int tmp = (middle_s + 1 <= finish_s ? dp_back0[i + 1] : finish_t - i);
		int back = (i == finish_t ? finish_s - middle_s : tmp);
		int new_sum = front + back;
		if (new_sum < best_sum)
		{
			best_sum = new_sum;
			middle_t = i;
		}
	}
	if (start_s == finish_s && middle_t == finish_t)
	{
		int pos = -1;
		for (int p = start_t; p <= finish_t; p++)
			if (s[start_s] == t[p])
			{
				pos = p;
				break;
			}
		if (pos != -1)
		{
			fore(p, start_t, finish_t)
			{
				if (pos != p)
					ans.push_back(make_pair(p, DEL_SECOND));
			}
		}
		else
		{
			ans.push_back(make_pair(start_s, start_t));
			fore(p, start_t + 1, finish_t)
				ans.push_back(make_pair(p, DEL_SECOND));
		}
		return best_sum;
	}
	
	int cnt1 = solveHirshberg(s, t, start_s, middle_s, start_t, middle_t, ans);
	int cnt2 = solveHirshberg(s, t, middle_s + 1, finish_s, middle_t + 1, finish_t, ans);
	return cnt1 + cnt2;
}

int solveHirshberg(const string & s, const string & t, pairVector & ans)
{
	int solveResult = solveHirshberg(s, t, 0, s.length() - 1, 0, t.length() - 1, ans);
	EXPECT_TRUE(checkAns(s, t, ans));
	return solveResult;
}



TEST(handy_tests, handy_test_1)
{
	pairVector ans;
	int solveRes = solveHirshberg("AEDDA", "BACCD", ans);
	ASSERT_EQ(solveRes, 4);
}

TEST(handy_tests, handy_test_2)
{
	pairVector ans;
	int solveRes = solveHirshberg("ABCDEFGH", "ACDEXGIH", ans);
	ASSERT_EQ(solveRes, 3);
}

TEST(handy_tests, handy_test_3)
{
	pairVector ans;
	int solveRes = solveHirshberg("FGH", "XGIH", ans);
	ASSERT_EQ(solveRes, 2);
}

TEST(handy_tests, handy_test_4)
{
	pairVector ans;
	int solveRes = solveHirshberg("BAEASDAS", "AE", ans);
	ASSERT_EQ(solveRes, 6);
}

TEST(handy_tests, handy_test_5)
{
	pairVector ans;
	int solveRes = solveHirshberg("AE", "BAEASDAS", ans);
	ASSERT_EQ(solveRes, 6);
}

inline void genTest1(int mod, int n, string & s)
{
	ASSERT_TRUE(mod >= 1 && mod <= 26);
	fore(j, 0, n - 1)
		s += ('A' + rand() % mod);
}

const int stress_count_1 = 100;
const int len_mod_1 = 70;

TEST(stress_tests, stress_test_first_variant_1)
{
	for (int it = 0; it < stress_count_1; it++)
	{
		string s, t;
		genTest1(26, rand() % len_mod_1, s);
		genTest1(26, rand() % len_mod_1, t);
		pairVector ans;
		int solveRes = solveHirshberg(s, t, ans);
		int rightAns = solveEasy(s, t);
		ASSERT_EQ(solveRes, rightAns);
	}
	printf("(stress tests) all %d tests completed! (with length mod %d)\n", stress_count_1, len_mod_1);
}

inline void genTest2(int mod, int n, int changes, string & s, string & t)
{
	ASSERT_TRUE(mod >= 1 && mod <= 26);
	ASSERT_TRUE(changes <= n);
	fore(j, 0, n - 1)
		s += ('A' + rand() % mod);
	t = s;
	fore(change, 1, changes)
	{
		int typ = rand() % 3;
		int pos1 = rand() % t.length();
		char new_char = ('A' + rand()) % mod;
		switch(typ)
		{
		case 0:
			{
				t = t.substr(0, pos1) + (new_char) + t.substr(pos1, t.length());
				break;
			}
		case 1:
			{
				t = t.substr(0, pos1) + t.substr(pos1 + 1, t.length());
				break;
			}
		case 2:
			{
				t[pos1] = new_char;
				break;
			}
		}
	}
}

TEST(stress_tests, stress_test_second_variant_1)
{
	//srand(time(0));
	for (int it = 0; it < stress_count_1; it++)
	{
		string s, t;
		int t1 = rand() % len_mod_1 + 1;
		int t2 = rand() % t1;
		genTest2(26, t1, t2, s, t);
		pairVector ans;
		int solveRes = solveHirshberg(s, t, ans);
		int rightAns = solveEasy(s, t);
		ASSERT_EQ(solveRes, rightAns);
	}
	printf("(stress tests) all %d tests completed! (with length mod %d)\n", stress_count_1, len_mod_1);
}

const int stress_count_2 = 5;
const int len_mod_2 = 700;

TEST(stress_tests, stress_test_first_variant_2)
{
	for (int it = 0; it < stress_count_2; it++)
	{
		string s, t;
		genTest1(26, rand() % len_mod_2, s);
		genTest1(26, rand() % len_mod_2, t);
		pairVector ans;
		int solveRes = solveHirshberg(s, t, ans);
		int rightAns = solveEasy(s, t);
		ASSERT_EQ(solveRes, rightAns);
	}
	printf("(stress tests) all %d tests completed! (with length mod %d)\n", stress_count_2, len_mod_2);
}

TEST(stress_tests, stress_test_second_variant_2)
{
	//srand(time(0));
	for (int it = 0; it < stress_count_2; it++)
	{
		string s, t;
		int t1 = rand() % len_mod_2 + 1;
		int t2 = rand() % t1;
		genTest2(26, t1, t2, s, t);
		pairVector ans;
		int solveRes = solveHirshberg(s, t, ans);
		int rightAns = solveEasy(s, t);
		ASSERT_EQ(solveRes, rightAns);
	}
	printf("(stress tests) all %d tests completed! (with length mod %d)\n", stress_count_2, len_mod_2);
}

const int time_count_1 = 5;
const int len_mod_3 = 700;

TEST(time_tests, time_test_first_variant)
{
	for (int it = 0; it < time_count_1; it++)
	{
		string s, t;
		genTest1(26, rand() % len_mod_3, s);
		genTest1(26, rand() % len_mod_3, t);
		pairVector ans;
		int solveRes = solveHirshberg(s, t, ans);
	}
	printf("(time tests) all %d tests completed! (with length mod %d)\n", time_count_1, len_mod_3);
}

TEST(time_tests, time_test_second_variant)
{
	//srand(time(0));
	for (int it = 0; it < time_count_1; it++)
	{
		string s, t;
		int t1 = rand() % len_mod_3 + 1;
		int t2 = rand() % t1;
		genTest2(26, t1, t2, s, t);
		pairVector ans;
		int solveRes = solveHirshberg(s, t, ans);
	}
	printf("(time tests) all %d tests completed! (with length mod %d)\n", time_count_1, len_mod_3);
}

int main(int argc, char ** argv)
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
}