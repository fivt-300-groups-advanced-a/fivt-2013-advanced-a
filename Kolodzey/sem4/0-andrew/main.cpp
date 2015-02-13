#include <iostream>
#include <cmath>

using namespace std;

struct Point {
	long long x;
	long long y;
};

const int MAX_N = 1005;
Point vertices[MAX_N];

int main() {
	
	int N; //number of vertices
	cin >> N;
	
	double L; //distance from the castle
	cin >> L;

	for (int i = 0; i < N; ++i)
		cin >> vertices[i].x >> vertices[i].y;

	

	return 0;
}