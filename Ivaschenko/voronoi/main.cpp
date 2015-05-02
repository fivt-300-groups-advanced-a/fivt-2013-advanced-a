#include <bits/stdc++.h>

/* Shity ejudge without c++11 */
#define nullptr NULL

using namespace std;

typedef long double coord_t;

struct Facet
{
    int pts[3];

    Facet(int a, int b, int c)
    {
        pts[0] = a;
        pts[1] = b;
        pts[2] = c;
    }

    int& operator [] (const int idx)
    {
        return pts[idx];
    }

    int operator [] (const int idx) const
    {
        return pts[idx];
    }
};

struct Point
{
    coord_t x, y, z;
    int id;
    Point *next, *prev;

    explicit Point(coord_t x = 0, coord_t y = 0, coord_t z = 0, int id = -1):
        x(x), y(y), z(z), id(id), next(nullptr), prev(nullptr)
    {}

    bool perform_event() // returns true if inserted
    {
        if (prev->next != this)
        {
            prev->next = this;
            next->prev = this;
            return true;
        }
        else
        {
            prev->next = next;
            next->prev = prev;
            return false;
        }
    }

    Point operator - (const Point &p) const
    {
        return Point(x - p.x, y - p.y, z - p.z, id);
    }

    bool operator < (const Point &p) const
    {
        return make_pair(x, y) < make_pair(p.x, p.y);
    }
};

namespace convex_hull
{
    const coord_t INF = 1e9;

    typedef Point Event;

    inline coord_t turn_sign(const Point *a, const Point *b, const Point *c)
    {
        if (a == nullptr || b == nullptr || c == nullptr)
            return INF;
        return (b->x - a->x) * (c->y - b->y) - (b->y - a->y) * (c->x - b->x);
    }

    inline coord_t turn_change(const Point *a, const Point *b, const Point *c)
    {
        if (a == nullptr || b == nullptr || c == nullptr)
            return INF;
        return ((b->x - a->x) * (c->z - b->z) - (b->z - a->z) * (c->x - b->x)) /
                                                             turn_sign(a, b, c);
    }

    vector<Event*> down_hull(vector<Point> &data, int l, int r)
    {
        if (r - l == 1)
            return vector<Event*>();

        int m = (l + r) / 2;
        vector<Event*> evs[2] = {
            down_hull(data, l, m),
            down_hull(data, m, r)
        };
        vector<Event*> result;

        Point *u = &data[m - 1], *v = &data[m];
        for (;;)
        {
            if (turn_sign(u, v, v->next) < 0) v = v->next;
            else if (turn_sign(u->prev, u, v) < 0) u = u->prev;
            else break;
        }

        size_t p1 = 0, p2 = 0;
        for (coord_t cur_time = -INF;;) // u ~ v is always tangent
        {
            Point *left = nullptr, *right = nullptr;
            coord_t next_time[6];
            fill(next_time, next_time + 6, INF);

            if (p1 < evs[0].size())
            {
                left = evs[0][p1];
                next_time[0] = turn_change(left->prev, left, left->next);
            }
            if (p2 < evs[1].size())
            {
                right = evs[1][p2];
                next_time[1] = turn_change(right->prev, right, right->next);
            }
            next_time[2] = turn_change(u, v, v->next);
            next_time[3] = turn_change(u, v->prev, v);
            next_time[4] = turn_change(u->prev, u, v);
            next_time[5] = turn_change(u, u->next, v);

            int min_index = -1;
            coord_t min_time = INF;
            for (int i = 0; i < 6; ++i)
                if (next_time[i] > cur_time && next_time[i] < min_time)
                {
                    min_time = next_time[i];
                    min_index = i;
                }
            if (min_index == -1 || min_time >= INF)
                break;

            switch (min_index)
            {
                case 0:
                    if (left->x < u->x)
                        result.push_back(left);
                    left->perform_event();
                    p1++;
                break;
                case 1:
                    if (right->x > v->x)
                        result.push_back(right);
                    right->perform_event();
                    p2++;
                break;
                case 2:
                    result.push_back(v);
                    v = v->next;
                break;
                case 3:
                    v = v->prev;
                    result.push_back(v);
                break;
                case 4:
                    result.push_back(u);
                    u = u->prev;
                break;
                case 5:
                    u = u->next;
                    result.push_back(u);
                break;
                default:
                    assert(false);
            }
            cur_time = min_time;
        }
        u->next = v;
        v->prev = u;
        for (int i = result.size() - 1; i >= 0; --i)
        {
            Point *cur = result[i];
            if (cur->x > u->x && cur->x < v->x)
            {
                u->next = v->prev = cur;
                cur->prev = u;
                cur->next = v;
                if (cur->x <= data[m - 1].x)
                    u = cur;
                else
                    v = cur;
            }
            else
            {
                cur->perform_event();
                if (cur == u)
                    u = u->prev;
                if (cur == v)
                    v = v->next;
            }
        }
        return result;
    }

    // Alas, points are to be non-degenerate in some sense. Beware
    vector<Facet> hull(vector<Point> data)
    {
        int n = data.size();
        vector<Facet> answer;
        sort(data.begin(), data.end());
        vector<Event*> events = down_hull(data, 0, n);
        for (size_t i = 0; i < events.size(); ++i)
        {
            Event *e = events[i];
            Facet cur(e->prev->id, e->id, e->next->id);
            if (!e->perform_event())
                swap(cur[0], cur[1]);
            answer.push_back(cur);
        }
        return answer;
    }
}

void rotateZ(Point &p, double ang)
{
    coord_t nx = p.x * cos(ang) + p.y * sin(ang);
    coord_t ny = -p.x * sin(ang) + p.y * cos(ang);
    p.x = nx, p.y = ny;
}

void rotateY(Point &p, double ang)
{
    coord_t nx = p.x * cos(ang) + p.z * sin(ang);
    coord_t nz = -p.x * sin(ang) + p.z * cos(ang);
    p.x = nx, p.z = nz;
}

void rotateX(Point &p, double ang)
{
    coord_t nz = p.z * cos(ang) + p.y * sin(ang);
    coord_t ny = -p.z * sin(ang) + p.y * cos(ang);
    p.z = nz, p.y = ny;
}

coord_t cross_product(const Point &a, const Point &b)
{
    return a.x * b.y - a.y * b.x;
}

bool good_turn(const Point &a, const Point &b, const Point &c)
{
    return cross_product(b - a, c - b) > 0;
}

int main()
{
    vector<Point> data;
    coord_t x, y;

    for (int id = 0; cin >> x >> y; ++id)
        data.push_back(Point(x, y, x * x + y * y, id));
    for (size_t i = 0; i < data.size(); ++i)
    {
        rotateX(data[i], 0.01);
        rotateY(data[i], 0.01);
        rotateZ(data[i], 0.01);
    }

    int n = data.size();
    set< pair<int, int> > edges;
    vector<int> cnt(n);
    vector<bool> inHull(n, false);
    vector<Facet> hull3d = convex_hull::hull(data);

    for (size_t i = 0; i < hull3d.size(); ++i)
    {
        const Facet &f = hull3d[i];
        for (int j = 0; j < 3; ++j)
        {
            pair<int, int> e(f[j], f[(j + 1) % 3]);
            if (e.first > e.second)
                swap(e.first, e.second);
            edges.insert(e);
        }
    }
    for (auto it = edges.begin(); it != edges.end(); ++it)
    {
        const auto &e = *it;
        cnt[e.first]++;
        cnt[e.second]++;
    }

    sort(data.begin(), data.end());
    vector<Point> hull;
    for (int i = 0; i < n; ++i)
    {
        while (hull.size() >= 2)
        {
            if (good_turn(hull[hull.size() - 2], hull.back(), data[i]))
                break;
            hull.pop_back();
        }
        hull.push_back(data[i]);
    }
    for (int i = n - 2, bottom = hull.size(); i >= 0; --i)
    {
        while ((int) hull.size() > bottom)
        {
            if (good_turn(hull[hull.size() - 2], hull.back(), data[i]))
                break;
            hull.pop_back();
        }
        hull.push_back(data[i]);
    }
    for (size_t i = 0; i < hull.size(); ++i)
        inHull[hull[i].id] = true;

    int inner = 0, sumDeg = 0;
    for (int i = 0; i < n; ++i)
        if (!inHull[i])
        {
            sumDeg += cnt[i];
            inner++;
        }
    cout << (inner == 0 ? 0.0 : double(sumDeg) / inner) << endl;
    return 0;
}
