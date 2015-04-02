#include <iostream>
#include <vector>
#include <cstdio>
#include <algorithm>
#include <cassert>

struct Point {
    long double x, y;

    Point(long double x_, long double y_): x(x_), y(y_) {}

    Point() {}

    bool operator<(const Point& another) const {
        return std::make_pair(x, y) < std::make_pair(another.x, another.y);
    }

    Point operator+(const Point& another) const {
        return Point(x + another.x, y + another.y);
    }

    Point operator-(const Point& another) const {
        return Point(x - another.x, y - another.y);
    }
};

long double vector_product(Point a, Point b) {
    return a.x * b.y - b.x * a.y;
}

long double rotation(Point a, Point b, Point c) {
    return vector_product(b - a, c - a);
}

void normalize_polygon(std::vector<Point> &points) {
    Point start_point(0, 0);
    int rotation_index;
    for (unsigned int i = 0; i < points.size(); i++) {
        if (i == 0 || points[i] < start_point) {
            start_point = points[i];
            rotation_index = i;
        }
    }
    std::rotate(points.begin(), points.begin() + rotation_index, points.end());
}

void build_minkovski_sum(const std::vector<Point>& first_polygon, const std::vector<Point>& second_polygon,
                         std::vector<Point>&result) {
    int first = 0;
    int second = 0;
    result.push_back(first_polygon[0] + second_polygon[0]);
    int n = first_polygon.size();
    int m = second_polygon.size();
    while (first < n || second < m) {
        Point first_candidate = first_polygon[(first + 1) % n] - first_polygon[first % n];
        Point second_candidate = second_polygon[(second + 1) % m] - second_polygon[second % m];
        if (first != n && vector_product(first_candidate, second_candidate) < 0 || second == m) {
            result.push_back(result.back() + first_candidate);
            first++;
        } else {
            result.push_back(result.back() + second_candidate);
            second++;
        }
    }
	result.pop_back();
}

bool point_in_polygon(Point p, const std::vector<Point>& polygon) {
    bool flag = true;
    int n = polygon.size();
    for (int i = 0; i < n; i++) {
        if (rotation(polygon[i], polygon[(i + 1) % n], p) > 0)
            flag = false;
	}
    return flag;
}

int main() {
    int n;
    std::cin >> n;
    std::vector<Point> first_polygon(n);
    for (int i = 0; i < n; i++) {
        std::cin >> first_polygon[i].x >> first_polygon[i].y;
    }
    normalize_polygon(first_polygon);
    int m;
    std::cin >> m;
    std::vector<Point> second_polygon(m);
    for (int i = 0; i < m; i++) {
        std::cin >> second_polygon[i].x >> second_polygon[i].y;
        second_polygon[i].x = -second_polygon[i].x;
        second_polygon[i].y = -second_polygon[i].y;
    }
    normalize_polygon(second_polygon);
    std::vector<Point> minkovski_sum;
    build_minkovski_sum(first_polygon, second_polygon, minkovski_sum);
    if (point_in_polygon(Point(0, 0), minkovski_sum)) {
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }
	return 0;
}