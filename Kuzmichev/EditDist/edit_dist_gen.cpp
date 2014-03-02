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
#include <ctime>

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

int main(int argv, char ** argc)
{
	srand(time(0));
	int n = atoi(argc[1]);
	int m = atoi(argc[2]);
	int mod = atoi(argc[3]);
	forn(j, n)
		printf("%c", 'A' + rand() % mod);
	printf("\n");
	forn(j, m)
		printf("%c", 'A' + rand() % mod);
	printf("\n");
	return 0;
}