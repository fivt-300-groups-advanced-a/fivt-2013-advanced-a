#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <ctime>
#include <cassert>
#include <memory.h>

#include <iostream>
#include <algorithm>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <list>

#define NAME "test"

#define EPS (1e-9)
#define INF ((int)(1e+9))
#define LINF ((long long)(1e+18))

using namespace std;

void solve(int test_number);

int main() {
#ifdef _GEANY
    assert(freopen(NAME ".in", "r", stdin));
    assert(freopen(NAME ".out", "w", stdout));
#else
#endif
    cout.setf(ios::fixed);
    cout.precision(9);
    cerr.setf(ios::fixed);
    cerr.precision(3);
    int n = 1;
    for (int i = 0; i < n; i++) {
        solve(i + 1);
    }
    return 0;
}

struct Point {
    long long x, y;

    void read() {
        cin >> x >> y;
    }
};

void read(vector<Point> &points, double &l) {
    int cnt_points;
    cin >> cnt_points >> l;
    points.resize(cnt_points);
    for (int i = 0; i < cnt_points; i++)
        points[i].read();
}

long long area(Point a, Point b) {
    return a.x * b.y - a.y * b.x;
}

long long area(Point a, Point b, Point c) {
    return area(a, b) + area(b, c) + area(c, a);
}

int rotation_dir(Point a, Point b, Point c) {
    long long prod = area(a, b, c);
    if (prod > 0)
        return 1;
    else if (prod < 0)
        return -1;
    else
        return 0;
}

bool cmp_by_x(Point a, Point b) {
    return a.x < b.x || (a.x == b.x && a.y < b.y);
}

vector<Point> build_convex_hull(vector<Point> &points) {
    int cnt_points = points.size();
    sort(points.begin(), points.end(), cmp_by_x);
    Point left = points[0];
    Point right = points[cnt_points - 1];
    
    vector<Point> upper, lower;
    upper.push_back(left), lower.push_back(left);
    for (int i = 1; i < cnt_points; i++) {
        if (i == cnt_points - 1 || rotation_dir(left, points[i], right) == -1) {
            while (upper.size() >= 2 && 
                    rotation_dir(upper[upper.size() - 2], upper[upper.size() - 1], points[i]) > -1)
                upper.pop_back();
            upper.push_back(points[i]);
        }
        if (i == cnt_points - 1 || rotation_dir(left, points[i], right) == 1) {
            while (lower.size() >= 2 && 
                    rotation_dir(lower[lower.size() - 2], lower[lower.size() - 1], points[i]) < 1)
                lower.pop_back();
            lower.push_back(points[i]);
        }
    }
    
    vector<Point> res;
    for (int i = 0; i < (int)upper.size(); i++)
        res.push_back(upper[i]);
    for (int i = lower.size() - 1; i >= 0; i--)
        res.push_back(lower[i]);
    return res;
}

double sqr(double a) {
    return a * a;
}

double dist(Point a, Point b) {
    return sqrt(sqr(a.x - b.x) + sqr(a.y - b.y));
}

double length(const vector<Point> &points) {
    double res = 0;
    for (int i = 1; i < (int)points.size(); i++)
        res += dist(points[i], points[i - 1]);
    res += dist(points[0], points[points.size() - 1]);
    return res;
}

void solve(int test_number) {
    vector<Point> points;
    double l;
    read(points, l);
    vector<Point> convex_hull = build_convex_hull(points);
    double len = length(convex_hull);
    cout << (int)(len + 2 * acos(-1) * l + 0.5);
}

