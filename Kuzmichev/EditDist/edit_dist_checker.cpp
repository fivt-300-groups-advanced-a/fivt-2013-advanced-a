#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <string>
#include <vector>
#include <cstdio>
#include <set>
#include <map>
#include <queue>
#include <memory.h>
#include <cmath>

using namespace std;

#define forn(i, n) for (int i = 0; i < (int)n; i++)
#define fore(i, b, e) for (int i = (int)b; i <= (int)e; i++)
#define all(x) (x).begin(), (x).end()
#define se second
#define fi first
#define mp make_pair
#define pb push_back
#define op operator
typedef vector <int> vi;
typedef pair<int, int> pii;
typedef long long i64;

const int maxn = 1005;
const int inf = 1e9;

string s, t;
int dp[maxn][maxn];
bool to_del_first[maxn];
bool to_del_second[maxn];

int main()
{
	freopen("output.txt", "r", stdin);
	cin >> s >> t;
	dp[0][0] = 0;
	fore(i, 0, s.length())
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
	int right_ans = dp[s.length()][t.length()];
	int given_ans;
	cin >> given_ans;
	printf("right = %d given = %d\n", right_ans, given_ans);
	if (right_ans != given_ans)
		return 1;
	for (int j = 1; j <= given_ans; j++)
	{
		int pos1, pos2;
		scanf("%d%d", &pos1, &pos2);
		if (pos2 == -1)
		{
			if (pos1 < 0 || pos1 >= s.length())
				return 1;
			to_del_first[pos1] = true;
		}
		else if (pos2 == -2)
		{
			if (pos1 < 0 || pos1 >= t.length())
				return 1;
			to_del_second[pos1] = true;
		}
		else
		{
			if (pos1 < 0 || pos1 >= s.length() || pos2 < 0 || pos2 >= t.length())
				return 1;
			s[pos1] = t[pos2];
		}
	}
	string s1 = "";
	for (int j = 0; j < s.length(); j++)
		if (!to_del_first[j])
			s1 += s[j];
	string t1 = "";
	for (int j = 0; j < t.length(); j++)
		if (!to_del_second[j])
			t1 += t[j];
	if (s1 != t1)
		return 1;
	//cout << "s1 = " << s1 << " t1 = " << t1 << endl << "OK" << endl;
	return 0;
}