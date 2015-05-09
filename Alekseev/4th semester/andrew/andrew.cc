#include <bits/stdc++.h>

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
std::vector<Point> wrapHalf(InputIt ptsBegin, InputIt ptsEnd, Point pivotA, Point pivotB)
{
    std::vector<Point> pts, ret;
    copy_if(ptsBegin, ptsEnd, back_inserter(pts), OnTheLeftSide(pivotA, pivotB));
    std::size_t size = 0;

    for (Point cur : pts)
    {
        while (size > 1 && Point(ret[size-2], ret.back()) % Point(ret.back(), cur) >= 0)
        {
            --size;
            ret.pop_back();
        }
        ret.push_back(cur);
        ++size;
    }

    return ret;
}

template<class RAIt, class Compare>
void sort(RAIt begin, RAIt end, Compare cmp, unsigned int concurrency = 1)
{
    // assert(concurrency == 1);
    std::sort(begin, end, cmp);
}

}

template<class InputIt>
std::vector<Point> buildHull(InputIt ptsBegin, InputIt ptsEnd, unsigned int concurrency = 1)
{
    std::vector<Point> points(ptsBegin, ptsEnd);
    impl::sort(points.begin(), points.end(),
            [](Point a, Point b){ return a.x < b.x; },
            concurrency);

    Point leftmost = points[0], rightmost = points.back();
    std::future<std::vector<Point>> upper_future = std::async(
            concurrency > 1 ? std::launch::async : std::launch::deferred,
            &impl::wrapHalf<std::vector<Point>::iterator>,
            points.begin(), points.end(), leftmost, rightmost);

    std::vector<Point> lower =
        impl::wrapHalf(points.rbegin(), points.rend(), rightmost, leftmost);
    std::vector<Point> upper = upper_future.get();

    upper.pop_back();
    upper.insert(upper.end(), lower.begin(), lower.end() - 1);

    return upper;
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
    std::cin.sync_with_stdio(false);

    int n, l;
    std::cin >> n >> l;

    std::vector<Point> points(n);
    for (Point& p : points)
        std::cin >> p.x >> p.y;

    auto hull = buildHull(points.begin(), points.end(), std::thread::hardware_concurrency());

    long double answerFloating = perimeter(hull.begin(), hull.end()) + 2. * M_PI * l;
    long long answerFeet = answerFloating + 0.5;

    std::cout << answerFeet << std::endl;

    timestamp(end);
    return 0;
}

