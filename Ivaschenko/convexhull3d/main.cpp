#include <bits/stdc++.h>

using std::vector;
using std::swap;
using std::sort;
using std::fill;

typedef double coord_t;

const coord_t INF = 1e9;

/* ugly shit instead c++11 */

#define nullptr NULL

struct Facet
{
    int pts[3];

    Facet(int a, int b, int c)
    {
        pts[0] = a;
        pts[1] = b;
        pts[2] = c;
    }

    void normalize()
    {
        std::rotate(pts, std::min_element(pts, pts + 3), pts + 3);
    }

    bool operator < (const Facet &f) const
    {
        for (int i = 0; i < 3; ++i)
            if (pts[i] != f.pts[i])
                return pts[i] < f.pts[i];
        return false;
    }
};

template<int n> int& get(Facet &f)
{
    return f.pts[n];
}

template<int n> const int& get(const Facet &f)
{
    return f.pts[n];
}

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

    bool operator < (const Point &p) const
    {
        return x < p.x;
    }
};

typedef Point Event;

namespace convex_hull
{
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

    Facet min_shift(Facet f)
    {
        if (get<1>(f) < get<0>(f) && get<1>(f) < get<2>(f))
        {
            swap(get<1>(f), get<0>(f));
            swap(get<1>(f), get<2>(f));
        }
        if (get<2>(f) < get<0>(f) && get<2>(f) < get<1>(f))
        {
            swap(get<2>(f), get<0>(f));
            swap(get<1>(f), get<2>(f));
        }
        return f;
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
                swap(get<0>(cur), get<1>(cur));
            answer.push_back(cur);
        }
        for (size_t i = 0; i < data.size(); ++i)
        {
            Point &p = data[i];
            p.next = p.prev = nullptr;
            p.z = -p.z;
        }
        events = down_hull(data, 0, n);
        for (size_t i = 0; i < events.size(); ++i)
        {
            Event *e = events[i];
            Facet cur(e->prev->id, e->id, e->next->id);
            if (e->perform_event())
                swap(get<0>(cur), get<1>(cur));
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

int main()
{
    int n;
    vector<Point> data;
    std::cin >> n;
    for (int i = 0; i < n; ++i)
    {
        int x, y, z;
        std::cin >> x >> y >> z;
        data.push_back(Point(x, y, z, i));
        rotateX(data.back(), 0.01);
        rotateY(data.back(), 0.01);
        rotateZ(data.back(), 0.01);
    }
    vector<Facet> hull = convex_hull::hull(data);
    for (size_t i = 0; i < hull.size(); ++i)
        hull[i].normalize();
    sort(hull.begin(), hull.end());
    std::cout << hull.size() << std::endl;
    for (size_t i = 0; i < hull.size(); ++i)
    {
        Facet &f = hull[i];
        std::cout << 3 << " " << get<0>(f) << " " << get<1>(f) <<
                                              " " << get<2>(f) << std::endl;
    }
    return 0;
}
