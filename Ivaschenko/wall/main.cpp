#include <algorithm>
#include <iostream>
#include <vector>

struct point
{
    int x, y;

    explicit point(int nx = 0, int ny = 0): x(nx), y(ny) {}

    bool operator < (const point &p) const
    {
        return std::make_pair(x, y) < std::make_pair(p.x, p.y);
    }

    point operator - (const point &p) const
    {
        return point(x - p.x, y - p.y);
    }

    double operator ~ () const
    {
        return sqrtl(x * x + y * y);
    }
};

int cross_product(const point &a, const point &b)
{
    return a.x * b.y - a.y * b.x;
}

std::vector<point> data, hull;

bool good_turn(const point &a, const point &b, const point &c)
{
    return cross_product(b - a, c - b) > 0;
}

void build_hull(std::vector<point> &data, std::vector<point> &hull)
{
    int n = data.size();
    sort(data.begin(), data.end());
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
}

int main()
{
#ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
#endif
    int n;
    double r;
    std::vector<point> data, hull;
    std::cin >> n >> r;
    for (int i = 0; i < n; ++i)
    {
        point cur;
        std::cin >> cur.x >> cur.y;
        data.push_back(cur);
    }
    build_hull(data, hull);
    double length = 2 * acosl(-1.0) * r;
    for (size_t i = 0; i + 1 < hull.size(); ++i)
        length += ~(hull[i] - hull[i + 1]);
    std::cout << (int) (length + 0.5) << std::endl;
    return 0;
}
