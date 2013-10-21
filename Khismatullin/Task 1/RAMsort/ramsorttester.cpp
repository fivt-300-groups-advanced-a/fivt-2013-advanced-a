#include<vector>
#include<cstdio>
#include<iostream>
#include "ramsort.h"

using namespace std;

vector<unsigned long long> v;

int main() {
	int n;
	cin >> n;
	for(int i = 0; i < n; i++) {
		unsigned long long	x;
		cin >> x;
		v.push_back(x);
	}
	head_sort(v, less<unsigned long long>());
	for(int i = 0; i < n; i++) {
		cout << v[i] << " ";
	}
	return 0;
}