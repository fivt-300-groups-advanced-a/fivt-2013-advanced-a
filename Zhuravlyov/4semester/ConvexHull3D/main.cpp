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
        return x < another.x;
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

// returns modifications vector
std::vector<Point*> build_upper_convex_hull(std::vector<Point>& points, int l, int r) {
    if (r == l)
        return std::vector<Point*>();
    int m = (l + r) / 2;
    std::vector<Point*> left_modifications = build_upper_convex_hull(points, l, m);
    std::vector<Point*> right_modifications = build_upper_convex_hull(points, m + 1, r);
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

void reverse_Oz(std::vector<Point>& points) {
    for (unsigned int i = 0; i < points.size(); i++) {
        points[i].z = -points[i].z;
        points[i].left = points[i].right = NULL;
    }
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

int main() {
    int n;
    std::cin >> n;
    std::vector<Point> points;
    for (int i = 0; i < n; i++) {
        int x, y, z;
        std::cin >> x >> y >> z;
        Point point = Point(x, y, z);
        rotate_point(point, 0.01, 0.01, 0.01);
        point.number = i;
        points.push_back(point);
    }
    std::sort(points.begin(), points.end());
    std::vector<Point*> history= build_upper_convex_hull(points, 0, n - 1);
    std::vector<Triple> answer;
    add_answer(history, answer, Deletion);
    reverse_Oz(points);
    history = build_upper_convex_hull(points, 0, points.size() - 1);
    add_answer(history, answer, Insertion);
    std::cout << answer.size() << std::endl;
    std::sort(answer.begin(), answer.end());
    for (unsigned int i = 0; i < answer.size(); i++) {
        std::cout << 3 << " " << answer[i].first << " " << answer[i].second << " " << answer[i].third << std::endl;
    }
    return 0;
}
