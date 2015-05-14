#include <iostream>
#include <vector>
#include <list>
#include <cstdio>
#include <cmath>
#include <algorithm>

const double inf = 10e100;

enum Action {Deletion, Insertion};

struct Triple {
    int first, second, third;
    Triple(int first_, int second_, int third_) {
        first = first_;
        second = second_;
        third = third_;
    }

    void normalize() {
        if (second < first && second < third) {
            std::swap(first, second);
            std::swap(second, third);
        } else if (third < first && third < second) {
            std::swap(second, third);
            std::swap(first, second);
        }
    }

    bool operator<(const Triple& another) const {
        return first < another.first || (first == another.first && second < another.second)
                || (first == another.first && second == another.second && third < another.third);
    }
};

struct Point {
    double x, y, z;
    int number;
    Point *left, *right;

    Point(double x_, double y_, double z_) {
        x = x_; y = y_; z = z_;
        left = right = NULL;
    }

    Point operator-(const Point& another) {
        return Point(x - another.x, y - another.y, z- another.z);
    }

    bool operator<(const Point& another) const {
        return x < another.x || x == another.x && y < another.y;
    }

    Action approve_modification() {
        if (left->right != this) {
            left->right = this;
            right->left = this;
            return Insertion;
        } else {
            left->right = right;
            right->left = left;
            return Deletion;
        }
    }
};

double rotation(Point a, Point b, Point c) {
    Point first = b - a, second = c - b;
    return first.x * second.y - first.y * second.x;
}

double rotation_time(Point *a, Point *b, Point *c) {
    if (a == NULL || b == NULL || c == NULL)
        return inf;
    Point first = *b - *a, second = *c - *b;
    return (first.x * second.z - second.x * first.z) / (first.x * second.y - second.x * first.y);
}

std::vector<Point*> build_convex_hull_3d(std::vector<Point>& points, int l, int r) {
    if (r == l)
        return std::vector<Point*>();
    int m = (l + r) / 2;
    std::vector<Point*> left_modifications = build_convex_hull_3d(points, l, m);
    std::vector<Point*> right_modifications = build_convex_hull_3d(points, m + 1, r);
    Point* bridge_left = &points[m];
    Point* bridge_right = &points[m + 1];
    bool flag;
    do {
        flag = true;
        if (bridge_right->right != NULL && rotation(*bridge_left, *bridge_right, *bridge_right->right) < 0) {
            bridge_right = bridge_right->right;
        } else if (bridge_left->left != NULL && rotation(*bridge_left->left, *bridge_left, *bridge_right) < 0) {
            bridge_left = bridge_left->left;
        } else
            flag = false;
    } while (flag);
    unsigned int cur_left = 0, cur_right = 0;
    double current_time = -inf;
    std::vector<Point*> current_modifications;
    do {
        std::vector<double> times(6);
        flag = true;
        Point *pending_left, *pending_right;
        if (cur_left < left_modifications.size()) {
            pending_left = left_modifications[cur_left];
            times[0] = rotation_time(pending_left->left, pending_left, pending_left->right);
        } else
            times[0] = inf;
        if (cur_right < right_modifications.size()) {
            pending_right = right_modifications[cur_right];
            times[1] = rotation_time(pending_right->left, pending_right, pending_right->right);
        } else
            times[1] = inf;
        times[2] = rotation_time(bridge_left, bridge_right, bridge_right->right);
        times[3] = rotation_time(bridge_left, bridge_right->left, bridge_right);
        times[4] = rotation_time(bridge_left->left, bridge_left, bridge_right);
        times[5] = rotation_time(bridge_left, bridge_left->right, bridge_right);
        double times_min = inf;
        int index;
        for (int i = 0; i < 6 ; i++)
            if (times[i] > current_time && times[i] < times_min) {
                times_min = times[i];
                index = i;
            }
        if (times_min == inf)
            flag = false;
        else {
            current_time = times_min;
            switch (index) {
                case 0:
                    if (pending_left->x < bridge_left->x)
                        current_modifications.push_back(pending_left);
                    pending_left->approve_modification();
                    cur_left++;
                    break;
                case 1:
                    if (pending_right->x > bridge_right->x)
                        current_modifications.push_back(pending_right);
                    pending_right->approve_modification();
                    cur_right++;
                    break;
                case 2:
                    current_modifications.push_back(bridge_right);
                    bridge_right = bridge_right->right;
                    break;
                case 3:
                    bridge_right = bridge_right->left;
                    current_modifications.push_back(bridge_right);
                    break;
                case 4:
                    current_modifications.push_back(bridge_left);
                    bridge_left = bridge_left->left;
                    break;
                case 5:
                    bridge_left = bridge_left->right;
                    current_modifications.push_back(bridge_left);
                    break;
            }
        }
    } while (flag);
    bridge_left->right = bridge_right;
    bridge_right->left = bridge_left;
    for (int i = current_modifications.size() - 1; i >= 0; i--) {
        Point* current = current_modifications[i];
        if (current->x > bridge_left->x && current->x < bridge_right->x) {
            bridge_left->right = bridge_right->left = current;
            current->left = bridge_left;
            current->right = bridge_right;
            if (current->x <= points[m].x)
                bridge_left = current;
            else
                bridge_right = current;
        } else {
            current->approve_modification();
            if (current == bridge_left)
                bridge_left = bridge_left->left;
            if (current == bridge_right)
                bridge_right = bridge_right->right;
        }
    }
    return current_modifications;
}

void add_answer(std::vector<Point*> history, std::vector<Triple>& answer, Action reverse_condition) {
    for (unsigned int i = 0; i < history.size(); i++) {
        Triple triple = Triple(history[i]->left->number, history[i]->number, history[i]->right->number);
        if (history[i]->approve_modification() == reverse_condition)
            std::swap(triple.first, triple.third);
        triple.normalize();
        answer.push_back(triple);
    }
}

void rotate_point(Point& point, double angle, double alpha, double beta) {
    point.x = point.x * cos(angle) + point.y * sin(angle);
    point.y = -point.x * sin(angle) + point.y * cos(angle);
    point.x = point.x * cos(alpha) + point.z * sin(alpha);
    point.z = -point.x * sin(alpha) + point.z * cos(alpha);
    point.y = point.y * cos(beta) + point.z * sin(beta);
    point.z = -point.y * sin(beta) + point.z * cos(beta);
}

void half_convex_hull_2d(const std::vector<Point>& source, std::vector<bool>& result) {
	std::vector<Point> convex_hull;
	convex_hull.push_back(source[0]);
	convex_hull.push_back(source[1]);
	for (unsigned int i = 2; i < source.size(); i++) {
		while (convex_hull.size() > 1 && rotation(*(convex_hull.end() - 1), *(convex_hull.end() - 2), source[i]) <= 0) {
			convex_hull.pop_back();
		}
		convex_hull.push_back(source[i]);
    }
	long double ans = 0;
	for (unsigned int i = 1; i < convex_hull.size(); i++) {
		result[convex_hull[i].number] = true;
	}
}

void convex_hull_2d(std::vector<Point>& verteces, std::vector<bool>& result) {
    std::sort(verteces.begin(), verteces.end());
    Point left = verteces[0];
    Point right = verteces.back();
    std::vector<Point> up_half, down_half;
    for (unsigned int i = 0; i < verteces.size(); i++) {
        int rot = rotation(left, right, verteces[i]);
        if (rot >= 0)
            up_half.push_back(verteces[i]);
        if (rot <= 0)
            down_half.push_back(verteces[i]);
    }
    std::reverse(down_half.begin(), down_half.end());
	result.assign(verteces.size(), false);
	half_convex_hull_2d(up_half, result);
	half_convex_hull_2d(down_half, result);
}

int main() {
    std::vector<Point> points;
	int i = 0;
	double x, y;
    while (std::cin >> x >> y) {
        Point point = Point(x, y, x * x + y * y);
        rotate_point(point, 0.01, 0.01, 0.01);
        point.number = i;
        points.push_back(point);
		i++;
    }
    std::sort(points.begin(), points.end());
    std::vector<Point*> history= build_convex_hull_3d(points, 0, points.size() - 1);
    std::vector<Triple> answer;
    add_answer(history, answer, Deletion);
    std::vector<bool> is_outer_point;
	convex_hull_2d(points, is_outer_point);
	std::vector<int> delaunay_triangulation_degrees(points.size(), 0);
	for (int i = 0; i < answer.size(); i++) {
		delaunay_triangulation_degrees[answer[i].first]++;
		delaunay_triangulation_degrees[answer[i].second]++;
		delaunay_triangulation_degrees[answer[i].third]++;
	}
	int voronoi_diagram_facets_count = 0;
	int voronoi_diagram_sum_edges_count = 0;
	for (int i = 0; i < points.size(); i++) {
		if (!is_outer_point[i]) {
			voronoi_diagram_facets_count++;
			voronoi_diagram_sum_edges_count += delaunay_triangulation_degrees[i];
		}
	}
	if (voronoi_diagram_facets_count == 0)
		std::cout << 0;
	else
		std::cout << double(voronoi_diagram_sum_edges_count) / voronoi_diagram_facets_count;
    return 0;
}