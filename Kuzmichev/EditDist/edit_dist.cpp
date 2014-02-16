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

#define DEL_FIRST -1
#define DEL_SECOND -2

typedef vector <int> vi;
typedef pair<int, int> pii;
typedef long long i64;

const int inf =  1e9;
const int maxn = 1005;

int dp_front[maxn][maxn];
int dp_back[maxn][maxn];
string s, t;
vector <pair<int, int> > ans;

inline void upd(int & x, int y)
{
	x = min(x, y);
}

inline void print_ans()
{
	for (int j = 0; j < ans.size(); j++)
		printf("%d %d\n", ans[j].first, ans[j].second);
	//printf("==================\n");
}

int solve(int start_s, int finish_s, int start_t, int finish_t)
{
	//printf("[%d;%d] [%d;%d]\n", start_s, finish_s, start_t, finish_t);
	if (start_t > finish_t)
	{
		if (start_s > finish_s)
			return 0;
		for(int pos = start_s; pos <= finish_s; pos++)
			ans.push_back(make_pair(pos, DEL_FIRST));
		//print_ans();
		return finish_s - start_s + 1;
	}
	if (start_s > finish_s)
	{
		for (int pos = start_t; pos <= finish_t; pos++)
			ans.push_back(make_pair(pos, DEL_SECOND));
		//print_ans();
		return finish_t - start_t + 1;
	}
	if (start_s == finish_s && start_t == finish_t)
	{
		if (s[start_s] != t[start_t])
		{
			ans.push_back(make_pair(start_s, start_t));
			return 1;
		}
		//print_ans();
		return 0;
	}

	int middle_s = (start_s + finish_s) / 2;

	for (int i = start_s; i <= middle_s; i++)
		for (int j = start_t; j <= finish_t; j++)
		{
			dp_front[i][j] = inf;
			int g;
			if (i - 1 < start_s)
				g = j - start_t;
			else if (j - 1 < start_t)
				g = i - start_s;
			else g = dp_front[i - 1][j - 1];
			upd(dp_front[i][j], g + (s[i] == t[j] ? 0 : 1));
			if (i > start_s)
				upd(dp_front[i][j], dp_front[i - 1][j] + 1);
			if (j > start_t)
				upd(dp_front[i][j], dp_front[i][j - 1] + 1);
		}
	for (int i = finish_s; i > middle_s; i--)
		for (int j = finish_t; j >= start_t; j--)
		{
			dp_back[i][j] = inf;
			int g;
			if (i + 1 > finish_s)
				g = finish_t - j;
			else if (j + 1 > finish_t)
				g = finish_s - i;
			else g = dp_back[i + 1][j + 1];
			upd(dp_back[i][j], g + (s[i] == t[j] ? 0 : 1));
			if (i < finish_s)
				upd(dp_back[i][j], dp_back[i + 1][j] + 1);
			if (j < finish_t)
				upd(dp_back[i][j], dp_back[i][j + 1] + 1);
		}
	int best_sum = inf;
	int middle_t;
	fore(i, start_t - 1, finish_t)
	{
		int front = (i == start_t - 1 ? middle_s - start_s + 1 : dp_front[middle_s][i]);
		int tmp = (middle_s + 1 <= finish_s ? dp_back[middle_s + 1][i + 1] : finish_t - i);
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
			for (int p = start_t; p <= finish_t; p++)
			{
				if (pos != p)
					ans.push_back(make_pair(p, DEL_SECOND));
			}
		}
		else
		{
			ans.push_back(make_pair(start_s, start_t));
			for (int p = start_t + 1; p <= finish_t; p++)
				ans.push_back(make_pair(p, DEL_SECOND));
		}
		//print_ans();
		return best_sum;
	}
	
	int cnt1 = solve(start_s, middle_s, start_t, middle_t);
	//printf("[%d;%d] [%d;%d] %d\n", start_s, middle_s, start_t, middle_t, cnt1);
	int cnt2 = solve(middle_s + 1, finish_s, middle_t + 1, finish_t);
	//printf("[%d;%d] [%d;%d] %d\n", middle_s + 1, finish_s, middle_t + 1, finish_t, cnt2);
	fflush(stdout);
	//printf("start_s = %d finish_s = %d start_t = %d finish_t = %d cnt1 = %d cnt2 = %d\n", start_s, finish_s, start_t, finish_t, cnt1, cnt2);
	return cnt1 + cnt2;
}



int main()
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	cin >> s >> t;
	int answer = solve(0, s.length() - 1, 0, t.length() - 1);
	cout << s << endl << t << endl;
	printf("%d\n", answer);
	print_ans();
	return 0;
}