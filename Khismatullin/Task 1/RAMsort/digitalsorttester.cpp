#include<vector>
#include<cstdio>
#include<iostream>
#include "ramsort.h"

using namespace std;

struct point {
	int x, y;
};

vector<int> v;

bool cmp(const point &a, const point &b)
{
	return ((a.x < b.x) || ((a.x == b.x) && (a.y < b.y)));
}


int main() {
	int n;
	cin >> n;
	for(int i = 0; i < n; i++) {
		int x;		
		cin >> x;
		v.push_back(x);
	}
	head_sort<int, std::less<int> > (v);
	for(int i = 0; i < n; i++) {
		cout << v[i] << "\n";
	}
	return 0;
}