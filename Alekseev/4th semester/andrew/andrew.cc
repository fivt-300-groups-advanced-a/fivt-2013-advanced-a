#include <bits/stdc++.h>
using namespace std;

#ifdef moskupols
    #define debug(...) fprintf(stderr, __VA_ARGS__) // thank Skird it's friday!
#else
    #define debug(...) 42
#endif

#define timestamp(x) debug("["#x"]: %.3f\n", (double)clock() / CLOCKS_PER_SEC) // thank PavelKunyavskiy i am not pregnant!

typedef long long int64;
typedef unsigned long long uint64;

struct Point
{
    typedef long long Coord;

    Coord x, y;

    Point(): x(0), y(0) {}
    Point(Coord x, Coord y): x(x), y(y) {}
    Point(Point a, Point b): x(b.x - a.x), y(b.y - a.y) {}

    long double range() const
    {
        return sqrtl(x * x + y * y);
    }
};

Point::Coord operator % (const Point &a, const Point &b)
{
    return a.x * b.y - a.y * b.x;
}

namespace impl
{
class OnTheLeftSide
{
public:
    OnTheLeftSide(Point A, Point B): A(A), B(B) {}

    bool operator() (Point C) const
    {
        return Point(A, B) % Point(A, C) >= 0;
    }
private:
    Point A, B;
};

template<class InputIt>
vector<Point> wrapHull(InputIt begin, InputIt end)
{
    vector<Point> ret;
    size_t size = 0;

    for (; begin != end; ++begin)
    {
        Point cur = *begin;
        while (size > 1 && Point(ret[size-2], ret[size-1]) % Point(ret[size-1], cur) >= 0)
        {
            --size;
            ret.pop_back();
        }
        ret.push_back(cur);
        ++size;
    }

    return ret;
}

}

template<class InputIt>
vector<Point> buildHull(InputIt ptsBegin, InputIt ptsEnd)
{
    vector<Point> points(ptsBegin, ptsEnd);
    sort(points.begin(), points.end(), [](Point a, Point b){ return a.x < b.x; });

    Point leftmost = points[0], rightmost = points.back();

    vector<Point> upper, lower;
    copy_if(points.begin(), points.end(), back_inserter(upper), impl::OnTheLeftSide(leftmost, rightmost));
    copy_if(points.rbegin(), points.rend(), back_inserter(lower), impl::OnTheLeftSide(rightmost, leftmost));

    upper = impl::wrapHull(upper.begin(), upper.end());
    lower = impl::wrapHull(lower.begin(), lower.end());

    vector<Point> ret(upper.begin(), upper.end() - 1);
    ret.insert(ret.end(), lower.begin(), lower.end() - 1);

    return ret;
}

template<class InputIt>
long double perimeter(InputIt a, InputIt b)
{
    if (a == b)
        return 0.;

    long double ret = 0.;

    Point first = *a;
    Point last = first;
    for (++a; a != b; ++a)
    {
        ret += Point(last, *a).range();
        last = *a;
    }
    ret += Point(last, first).range();

    return ret;
}

int main()
{
	cin.sync_with_stdio(false);

    int n, l;
    cin >> n >> l;

    vector<Point> points(n);
    for (Point& p : points)
        cin >> p.x >> p.y;

    auto hull = buildHull(points.begin(), points.end());

    long double answerFloating = perimeter(hull.begin(), hull.end()) + 2. * 3.1415926 * l;
    long long answerFeet = answerFloating + 0.5;

    cout << answerFeet << endl;

	timestamp(end);
	return 0;
}

