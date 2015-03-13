#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <cstdio>
#include <cassert>

long double eps = 1e-9;

long double sqr(long double a) {
    return a * a;
}

bool check_interval(long double t1, long double t2, long double z, long double z1, int h) {
    long double z1_solve = z1 + t1 * z;
    long double z2_solve = z1 + t2 * z;
    return ((z1_solve >= 0 && z1_solve <= h) || (z2_solve >= 0 && z2_solve <= h));
}

bool is_cone_segment_intersects(long double x0, long double y0, long double r,
                                long double h, long double x1, long double y1,
                                long double z1, long double x2, long double y2,
                                long double z2) {
    long double x = x2 - x1, y = y2 - y1, z = z2 - z1;
    if (x == 0 && y == 0 && z == 0)
        return false;
    long double dx = x1 - x0, dy = y1 - y0, dz = z1 - h;
    long double A = sqr(z * r) - sqr(x * h) - sqr(y * h);
    long double B = 2 * z * dz * sqr(r) - 2  * sqr(h) * (x * dx + y * dy);
    long double C = sqr(r * dz) - sqr(dx * h) - sqr(dy * h);
    long double t1, t2;
    long double D = B * B - 4 * A * C;
    if (D < 0)
        return (A > 0) && (z1 < h || z2 < h);
    t1 = (-B + sqrtl(D)) / (2 * A);
    t2 = (-B - sqrtl(D)) / (2 * A);
    if (A < 0) {
        t1 = std::max(t1, eps);
        t2 = std::min(t2, 1 - eps);
        if (t2 < t1)
            return false;
        return check_interval(t1, t2, z, z1, h);
    } else
        return (t2 > 1 - eps || t1 < eps) && check_interval(eps, 1 - eps, z, z1, h)
                || (t2 > eps && t2 < 1 - eps && check_interval(eps, t2, z, z1, h))
                || (t1 < 1 - eps && t1 > eps && check_interval(t1, 1 - eps, z, z1, h));
}

int bfs(int n, const std::vector< std::vector<char> >& graph, long double start, long double finish) {
    std::queue<int> bfs_order;
    std::vector<char> used(n, 0);
    std::vector<int> distances(n);
    bfs_order.push(start);
    used[start] = 1;
    distances[start] = 0;
    while (!used[finish]) {
        int v = bfs_order.front();
        bfs_order.pop();
        for (int j = 0; j < n; j++) {
            if (graph[v][j] && !used[j]) {
                bfs_order.push(j);
                used[j] = 1;
                distances[j] = distances[v] + 1;
            }
        }
    }
    return distances[finish];
}

int main() {
    int n;
    std::cin >> n;
    std::vector<long long> x(n), y(n), r(n), h(n);
    for (int i = 0; i < n; i++)
        std::cin >> x[i] >> y[i] >> r[i] >> h[i];
    std::vector< std::vector<char> > graph(n, std::vector<char>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            graph[i][j] = 1;
            for (int k = 0; k < n; k++)
                if (is_cone_segment_intersects(x[k], y[k], r[k], h[k], x[i], y[i], h[i], x[j], y[j], h[j]))
                    graph[i][j] = 0;
        }
    std::cout << bfs(n, graph, 0, n - 1);
}
