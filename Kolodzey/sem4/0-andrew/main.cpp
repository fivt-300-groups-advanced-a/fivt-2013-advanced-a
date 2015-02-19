//Dashk0 submitted this solution to Timus and successeded

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

const double PI = 3.1415965;

using namespace std;

struct Point {
    Point() : x(0), y(0) { }

    Point (long long x, long long y): x(x), y(y) {};
    
    Point operator - (const Point& b) const {
        return Point(x - b.x, y - b.y);
    }
    
    long long operator % (const Point& b) const {
        return x * b.y - y * b.x;
    }
    
    double len() {
        return sqrt(x * x + y * y);
    }

    long long x;
    long long y;
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

//input --- iterators to begin and to end of vector containing data about half of dots set
//output --- len of this half of convex hull
template<typename T>
double countLenghtOfConvexHullHalf(T begin, T end) {
    
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

    double len_of_convex_hull = countLenghtOfConvexHullHalf(upper.begin(), upper.end()) +
                                countLenghtOfConvexHullHalf(lower.rbegin(), lower.rend());
          
    cout << ((long long)(len_of_convex_hull + (L * 2 * PI + 0.5))) << endl;

    return 0;
}