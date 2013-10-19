#include <vector>
#include <iostream>
#include "heap.h"

using namespace std;

int main() 
{
	int n;
	cin >> n;
	heap<pair<int, int>, std::less<pair<int, int> > > h(n);
	for(int i = 0; i < n; i++) {
		pair<int, int> x;
		cin >> x.first;
		x.second = i;
		h.add(x);
	}	
	for(int i = 0; i < n; i++) {
		cout << h.Extract_min().second << " ";
	}
}