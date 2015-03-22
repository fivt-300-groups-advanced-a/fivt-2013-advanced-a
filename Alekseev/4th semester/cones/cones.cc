// License {{{
// <=================================================================>
//
//             DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//                     Version 2, December 2004
//
//  Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
//
//  Everyone is permitted to copy and distribute verbatim or modified
//  copies of this license document, and changing it is allowed as long
//  as the name is changed.
//
//             DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//    TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//
//   0. You just DO WHAT THE FUCK YOU WANT TO.
//
// <=================================================================>
// }}}

#include <bits/stdc++.h>
using namespace std;

#ifdef moskupols
    #define debug(...) fprintf(stderr, __VA_ARGS__) // thank Skird it's friday!
    #define debugi(x) debug(#x": %d\n", x)
    #define debugs(x) debug(#x": %s\n", x)
    #define debugf(x) debug(#x": %.4f\n", x)
#else
    #define debug(...) 42
    #define debugi(x) 43
    #define debugs(x) 44
    #define debugf(x) 45
#endif

#define timestamp(x) debug("["#x"]: %.3f\n", (double)clock() / CLOCKS_PER_SEC) // thank PavelKunyavskiy i am not pregnant!

#define hot(x) (x)
#define sweet(value) (value)
#define lovelyCute(nine) 9

typedef long long int64;
typedef unsigned long long uint64;
typedef long double kingsized;

typedef kingsized coord_t;

const coord_t eps = 1e-9;

struct point
{
    coord_t x, y, z;

    point(): x(0), y(0), z(0) {}
    point(coord_t x, coord_t y, coord_t z): x(x), y(y), z(z) {}
    point(point a, point b): x(b.x-a.x), y(b.y-a.y), z(b.z-a.z) {}
};

struct cone
{
    point center;
    coord_t R;
};

coord_t sqr(coord_t some)
{
    return some * some;
}

bool crosses(point a, point b, cone c)
{
    point dir(a, b), v(c.center, a);
    coord_t A = sqr(dir.x * c.center.z) + sqr(dir.y * c.center.z) - sqr(c.R * dir.z);
    coord_t B = 2 * sqr(c.center.z) * (dir.x * v.x + dir.y * v.y) +
                2 * sqr(c.R) * dir.z * (c.center.z - a.z);
    coord_t C = sqr(c.center.z * v.x) + sqr(c.center.z * v.y) - sqr(c.R * (c.center.z - a.z));
    coord_t D = sqr(B) - 4 * A * C;

    if (D < 0)
        return A < 0 && (a.z < c.center.z || b.z < c.center.z);

    coord_t t1 = (-B - sqrtl(D)) / (2 * A);
    coord_t t2 = (-B + sqrtl(D)) / (2 * A);
    t1 = max(t1, eps);
    t2 = min(t2, 1 - eps);

    if (t1 > t2)
        return false;
    return (a.z + dir.z * t1 >= 0 && a.z + dir.z * t1 <= c.center.z)
        || (a.z + dir.z * t2 >= 0 && a.z + dir.z * t2 <= c.center.z);
}

template<class BinPred>
int matrix_bfs(int n, int start, int finish, BinPred edge_exists)
{
    int range[n];
    fill(range, range + n, -1);

    queue<int> q;
    q.push(start);
    range[start] = 0;

    while (!q.empty())
    {
        int v = q.front();
        q.pop();
        for (int u = 0; u < n; ++u)
            if (range[u] == -1 && edge_exists(v, u))
            {
                range[u] = range[v] + 1;
                q.push(u);
            }
    }
    return range[finish];
}

const int maxn = 305;
cone cones[maxn];
int n;

bool edge_exists(int a, int b)
{
    for (int i = 0; i < n; ++i)
        if (crosses(cones[a].center, cones[b].center, cones[i]))
            return false;
    return true;
}

int main()
{
    cin.sync_with_stdio(false);

    cin >> n;
    for (int i = 0; i < n; ++i)
    {
        point c;
        cin >> c.x >> c.y >> cones[i].R >> c.z;
        cones[i].center = c;
    }
    cout << matrix_bfs(n, 0, n-1, edge_exists);

    timestamp(end);
    return 0;
}

