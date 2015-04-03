#include <bits/stdc++.h>
using namespace std;

#ifdef moskupols
    #define debug(...) fprintf(stderr, __VA_ARGS__) // thank Skird it's friday!
#else
    #define debug(...) 42
#endif

typedef long double coord_t;
const coord_t eps = 1e-7;

struct Point
{
    coord_t x, y;

    Point(): x(0), y(0) {}
    Point(coord_t x, coord_t y): x(x), y(y) {}
    Point(Point a, Point b): x(b.x-a.x), y(b.y-a.y) {}
    explicit Point(istream &in)
    {
        in >> x >> y;
    }
};

inline Point operator + (const Point &a, const Point &b)
{
    return Point(a.x + b.x, a.y + b.y);
}

inline Point& operator += (Point &a, const Point &b)
{
    a.x += b.x;
    a.y += b.y;
    return a;
}

inline coord_t operator % (const Point &a, const Point &b)
{
    return a.x * b.y - a.y * b.x;
}

inline bool operator < (const Point &a, const Point &b)
{
    return a.x == b.x ? a.y < b.y : a.x < b.x;
}

template<class InputItA, class InputItB, class OutputIt>
OutputIt minkowski_sum(
        InputItA beginA, InputItA endA,
        InputItB beginB, InputItB endB,
        OutputIt output)
{
    vector<Point> a(beginA, endA), b(beginB, endB);
    rotate(a.begin(), min_element(a.begin(), a.end()), a.end());
    rotate(b.begin(), min_element(b.begin(), b.end()), b.end());

    size_t i = 0, j = 0;
    const size_t n = a.size(), m = b.size();

    Point last;
    *output++ = last = a[0] + b[0];

    while (n - i + m - j > 1)
    {
        Point pa(a[i%n], a[(i+1)%n]), pb(b[j%m], b[(j+1)%m]);

        if ((i != n && pa % pb < -eps) || j == m)
        {
            last += pa;
            ++i;
        }
        else
        {
            last += pb;
            ++j;
        }
        *output++ = last;
    }

    return output;
}

template<class InputIt>
bool inside_polygon(const Point &p, InputIt begin, InputIt end)
{
    Point prev, first;
    prev = first = *begin++;
    while (begin != end)
    {
        Point cur = *begin++;
        if (Point(prev, cur) % Point(prev, p) > eps)
            return false;
        prev = cur;
    }
    return Point(prev, first) % Point(prev, p) < eps;
}

int main()
{
    cin.sync_with_stdio(false);

    size_t n;
    cin >> n;
    vector<Point> a(n);
    for (size_t i = 0; i < n; ++i)
        a[i] = Point(cin);

    size_t m;
    cin >> m;
    vector<Point> b(m);
    for (size_t i = 0; i < m; ++i)
        b[i] = Point(Point(cin), Point(0, 0)); // opposite vector

    vector<Point> sum;
    sum.reserve(max(a.size(), b.size()));
    minkowski_sum(a.begin(), a.end(), b.begin(), b.end(), back_inserter(sum));

    cout << (inside_polygon(Point(0, 0), sum.begin(), sum.end()) ? "YES\n" : "NO\n");

    return 0;
}

