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
    long double x, y, z;

    Point() {
        x = y = z;
    }

    Point(long double _x, long double _y, long double _z) {
        x = _x, y = _y, z = _z;
    }

    void read() {
        cin >> x >> y;
        z = 0;
    }
};

struct Cone {
    Point c;
    long double r;
    long double h;

    void read() {
        c.read();
        cin >> r;
        cin >> h;
    }
};

void read(vector<Cone> &cones) {
    int n;
    cin >> n;
    cones.resize(n);
    for (int i = 0; i < n; i++)
        cones[i].read();
}

long double sqr(long double x) {
    return x * x;
}

bool check_intersection(Point from, Point to, Cone cone) {
    cone.c.z = cone.h;
    Point dir(to.x - from.x, to.y - from.y, to.z - from.z);
    Point v(from.x - cone.c.x, from.y - cone.c.y, from.z - cone.c.z);

    long double A = sqr(dir.x * cone.c.z) + sqr(dir.y * cone.c.z) - sqr(cone.r * dir.z);
    long double B = 2 * sqr(cone.c.z) * (dir.x * v.x + dir.y * v.y) + 
        2 * sqr(cone.r) * dir.z * (cone.c.z - from.z);
    long double C = sqr(cone.c.z * v.x) + sqr(cone.c.z * v.y) - sqr(cone.r * (cone.c.z - from.z));

    long double D = B * B - 4 * A * C;
    if (D < 0) {
        return A < 0 && (from.z < cone.c.z || to.z < cone.c.z);
    }
    long double t1 = (-B - sqrtl(D)) / (2 * A);
    long double t2 = (-B + sqrtl(D)) / (2 * A);
    if (t1 < EPS) {
        t1 = EPS;
    }
    if (t2 > 1 - EPS) {
        t2 = 1 - EPS;
    }

    if (t1 > t2) {
        return false;
    }
    if (from.z + dir.z * t1 >= 0 && from.z + dir.z * t1 <= cone.c.z) {
        return true;
    }
    if (from.z + dir.z * t2 >= 0 && from.z + dir.z * t2 <= cone.c.z) {
        return true;
    }
    return false;
}

bool check_edge(const vector<Cone> &cones, size_t from, size_t to) {
    for (size_t i = 0; i < cones.size(); i++) {
        Point from_point = cones[from].c;
        Point to_point = cones[to].c;
        from_point.z += cones[from].h;
        to_point.z += cones[to].h;
        if (check_intersection(from_point, to_point, cones[i])) {
            return false;
        }
    }
    return true;
}

void build_graph(const vector<Cone> &cones, vector<vector<int> > &graph) {
    graph.resize(cones.size());
    for (size_t i = 0; i < cones.size(); i++) {
        for (size_t j = 0; j < cones.size(); j++) {
            if (i != j && check_edge(cones, i, j)) {
                graph[i].push_back(j);
            }
        }
    }
}

int bfs(vector<vector<int> > graph, int start, int end) {
    queue<int> q;
    q.push(start);
    vector<int> dist(graph.size(), -1);
    dist[start] = 0;
    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (size_t i = 0; i < graph[v].size(); i++) {
            int to = graph[v][i];
            if (dist[to] == -1) {
                dist[to] = dist[v] + 1;
                q.push(to);
            }
        }
    }
    return dist[end];
}

void solve(int test_number) {
    vector<Cone> cones;
    read(cones);
    vector<vector<int> > graph;
    build_graph(cones, graph);  
    cout << bfs(graph, 0, cones.size() - 1) << endl;
}

