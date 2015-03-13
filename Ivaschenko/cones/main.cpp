#include <bits/stdc++.h>

using namespace std;

typedef long double coord_t;

coord_t eps = 1e-9;

struct point
{
    coord_t x, y, z;

    explicit point(coord_t nx = 0, coord_t ny = 0, coord_t nz = 0):
        x(nx), y(ny), z(nz)
    {}

    point operator - (const point &p) const
    {
        return point(x - p.x, y - p.y, z - p.z);
    }
};

struct cone
{
    point c;
    coord_t r;
};

coord_t sqr(coord_t x)
{
    return x * x;
}

bool strict_cross(const point &a, const point &b, const cone &c)
{
    point dir = b - a, v = a - c.c;
    coord_t A = sqr(dir.x * c.c.z) + sqr(dir.y * c.c.z) - sqr(c.r * dir.z);
    coord_t B = 2 * sqr(c.c.z) * (dir.x * v.x + dir.y * v.y) +
                2 * sqr(c.r) * dir.z * (c.c.z - a.z);
    coord_t C = sqr(c.c.z * v.x) + sqr(c.c.z * v.y) - sqr(c.r * (c.c.z - a.z));
    coord_t D = sqr(B) - 4 * A * C;
    assert(A != 0);
    if (D < 0)
        return A < 0 && (a.z < c.c.z || b.z < c.c.z);
    coord_t t1 = (-B + sqrtl(D)) / (2 * A);
    coord_t t2 = (-B + sqrtl(D)) / (2 * A);
    if (t1 < eps) t1 = eps;
    if (t2 > 1 - eps) t2 = 1 - eps;
    if (t1 > t2) return false;
    if (a.z + dir.z * t1 >= 0 && a.z + dir.z * t1 <= c.c.z)
        return true;
    if (a.z + dir.z * t2 >= 0 && a.z + dir.z * t2 <= c.c.z)
        return true;
    return false;
}

int bfs(const vector<cone> &data)
{
    queue<size_t> q;
    vector<int> dist(data.size(), -1);
    q.push(0);
    dist[0] = 0;
    while (!q.empty())
    {
        size_t v = q.front();
        q.pop();
        for (size_t u = 0; u < data.size(); ++u)
        {
            if (dist[u] != -1 || u == v)
                continue;
            bool ok = true;
            for (size_t i = 0; i < data.size(); ++i)
            {
                ok &= !strict_cross(data[v].c, data[u].c, data[i]);
            }
            if (!ok)
                continue;
            dist[u] = dist[v] + 1;
            q.push(u);
        }
    }
    return dist.back();
}

int main()
{
#ifdef LOCAL
    freopen("input.txt", "r", stdin);
#endif
    int n;
    cin >> n;
    vector<cone> data(n);
    for (int i = 0; i < n; ++i)
        cin >> data[i].c.x >> data[i].c.y >> data[i].r >> data[i].c.z;
    cout << bfs(data) << endl;
    return 0;
}
