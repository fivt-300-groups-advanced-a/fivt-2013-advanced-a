#include <bits/stdc++.h>

using std::vector;
using std::rotate;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::make_pair;

typedef long double coord_t;

struct Point
{
    coord_t x, y;

    Point(coord_t nx, coord_t ny): x(nx), y(ny)
    {
    }

    Point operator - (const Point &p) const
    {
        return Point(x - p.x, y - p.y);
    }

    Point operator - () const
    {
        return Point(-x, -y);
    }

    Point operator + (const Point &p) const
    {
        return Point(x + p.x, y + p.y);
    }

    coord_t operator % (const Point &p) const
    {
        return x * p.y - y * p.x;
    }

    bool operator < (const Point &p) const
    {
        return make_pair(x, y) < make_pair(p.x, p.y);
    }
};

vector<Point> minkowskiSum(const vector<Point> &first,
                           const vector<Point> &second)
{
    vector<Point> result;
    result.push_back(first[0] + second[0]);
    int n = first.size(), m = second.size();
    for (int i = 0, ptr = 0; i < n; ++i)
    {
        Point v1 = first[(i + 1) % n] - first[i];
        while (ptr < m)
        {
            Point v2 = second[(ptr + 1) % m] - second[ptr];
            if (v1 % v2 < 0)
                break;
            result.push_back(result.back() + v2);
            ptr++;
        }
        result.push_back(result.back() + v1);
    }
    return result;
}

void normalize(vector<Point> &poly)
{
    rotate(poly.begin(), min_element(poly.begin(), poly.end()), poly.end());
}

bool inside(const vector<Point> &poly, const Point &p)
{
    int n = poly.size();
    for (int i = 0; i < n; ++i)
        if ((poly[i] - p) % (poly[(i + 1) % n] - p) > 1e-9)
            return false;
    return true;
}

vector<Point> readPoly()
{
    int n;
    cin >> n;
    vector<Point> poly;
    for (int i = 0; i < n; ++i)
    {
        coord_t x, y;
        cin >> x >> y;
        poly.emplace_back(x, y);
    }
    return poly;
}

int main()
{
    vector<Point> first = readPoly();
    vector<Point> second = readPoly();
    for (size_t i = 0; i < second.size(); ++i)
        second[i] = -second[i];
    normalize(first);
    normalize(second);
    vector<Point> sum = minkowskiSum(first, second);
    cout << (inside(sum, Point(0, 0)) ? "YES" : "NO") << endl;
}
