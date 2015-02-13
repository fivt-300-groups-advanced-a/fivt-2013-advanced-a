#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

struct Point {
	Point() : x(0), y(0) { }
	Point (long long x, long long y): x(x), y(y) {};
	long long x;
	long long y;
	Point operator - (const Point& b) const {
		return Point(x - b.x, y - b.y);
	}
	long long operator % (const Point& b) const {
		return x * b.y - y * b.x;
	}
	double len() {
		return sqrt(x * x + y * y);
	}
};

bool compareX(Point a, Point b) {
	return a.x < b.x;
}

//input --- vector of verteces sorted by x
//output --- vector of vertices no lower than [vertices.front(), vertices.back()]
vector<Point> chooseUpper(const vector<Point>& vertices){
	vector<Point> ans;
	ans.push_back(vertices.front());
	for (auto i = vertices.begin() + 1; i != vertices.end() - 1; ++i) {
		if (((vertices.back() - vertices.front()) % (*i - vertices.front())) >= 0)
			ans.push_back(*i);
	}
	ans.push_back(vertices.back());
	return ans;
}

//input --- vector of verteces sorted by x
//output --- vector of vertices lower than [vertices.front(), vertices.back()]
vector<Point> chooseLower(const vector<Point>& vertices){
	vector<Point> ans;
	ans.push_back(vertices.front());
	for (auto i = vertices.begin() + 1; i != vertices.end() - 1; ++i) {
		if (((vertices.back() - vertices.front()) % (*i - vertices.front())) < 0)
			ans.push_back(*i);
	}
	ans.push_back(vertices.back());
	return ans;
}

double countLenghtOfConvexHullHalf(forward_iterator_tag begin, forward_iterator_tag end) {
	vector<Point> stack;
	for (auto i = begin; i != end; ++i) {
		while((stack.size() > 1) &&
			  (((stack.back() - stack[stack.size() - 2]) % (*i - stack.back())) >= 0))
			stack.pop_back();
		stack.push_back(*i);
	}
	double ans = 0;
	for (auto i = 0; i < stack.size() - 1; ++i)
		ans  += (stack[i + 1] - stack[i]).len();
	return ans;
}

int main() {
	
	int N; //number of vertices
	cin >> N;
	
	double L; //distance from the castle
	cin >> L;

	vector<Point> vertices(N);

	for (int i = 0; i < N; ++i)
		cin >> vertices[i].x >> vertices[i].y;

	sort(vertices.begin(), vertices.end(), compareX);

	vector<Point> upper = chooseUpper(vertices);
	vector<Point> lower = chooseLower(vertices);

	/*
	Testing lower and upper
	cout << "upper" << endl;
	for (auto i = upper.begin(); i!=upper.end(); ++i)
		cout << i->x << " " << i->y << endl;
	cout << "lower" << endl;
	for (auto i = lower.begin(); i!=lower.end(); ++i)
		cout << i->x << " " << i->y << endl;
	*/

	cout << countLenghtOfConvexHullHalf(upper.begin(), upper.end()) << endl;
	cout << countLenghtOfConvexHullHalf(lower.rbegin(), lower.rend()) << endl;

	return 0;
}