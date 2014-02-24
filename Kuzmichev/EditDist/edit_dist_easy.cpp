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

int main()
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
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
	//printf("%d\n", dp[5][5]);
	printf("%d", dp[s.length()][t.length()]);
	return 0;
}