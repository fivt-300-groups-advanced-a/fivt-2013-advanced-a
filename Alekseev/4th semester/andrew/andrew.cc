#include <bits/stdc++.h>

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

template<class FwdIt, class OutputIt, class Pred>
OutputIt merge(FwdIt beginA, FwdIt endA,
        FwdIt beginB, FwdIt endB,
        OutputIt out, Pred cmp, unsigned int/*  concurrency */)
{
    return std::merge(beginA, endA, beginB, endB, out, cmp);
}

template<class FwdIt, class OutputIt>
OutputIt copy(FwdIt begin, FwdIt end, OutputIt out, unsigned int/*  concurrency */)
{
    return std::copy(begin, end, out);
}

template<class FwdIt, class Pred>
void inplace_merge(FwdIt begin, FwdIt mid, FwdIt end,
        Pred cmp, unsigned int concurrency)
{
    std::vector<typename std::iterator_traits<FwdIt>::value_type> tmp;
    tmp.reserve(distance(begin, end));
    merge(begin, mid, mid, end, back_inserter(tmp), cmp, concurrency);
    copy(std::make_move_iterator(tmp.begin()), std::make_move_iterator(tmp.end()),
            begin, concurrency);
}

template<class RAIt, class Pred>
void sort(RAIt begin, RAIt end, Pred cmp, unsigned int concurrency = 1)
{
    std::size_t distance = std::distance(begin, end);
    concurrency = std::min<std::size_t>(concurrency, distance / 2);

    if (concurrency <= 1)
        return std::sort(begin, end, cmp);

    std::vector<RAIt> bounds; // bounds of sorted chunks
    for (unsigned int i = 0; i < concurrency; ++i)
        bounds.push_back(begin + i * distance / concurrency);
    bounds.push_back(end);

    { // sort chunks
        std::vector<std::future<void>> sort_futures;
        sort_futures.reserve(bounds.size()-1);
        for (std::size_t i = 0; i + 1 < bounds.size(); ++i)
            sort_futures.push_back(std::async(std::launch::async,
                        &std::sort<RAIt, Pred>, bounds[i], bounds[i + 1], cmp));
        for (auto& f : sort_futures)
            f.wait();
    }

    while (bounds.size() > 2) // merge chunks
    {
        std::vector<RAIt> newBounds;
        std::vector<std::future<void>> merge_futures;
        newBounds.reserve((bounds.size() + 1) / 2);
        merge_futures.reserve(bounds.size() / 2);
        for (std::size_t i = 0; i + 2 < bounds.size(); i += 2)
        {
            inplace_merge(bounds[i], bounds[i+1], bounds[i+2], cmp, 1); // FIXME!!
            newBounds.push_back(std::move(bounds[i]));
            if (i + 2 == bounds.size())
                newBounds.push_back(std::move(bounds[i+1]));
        }
        bounds.swap(newBounds);
    }
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
    // auto hull = buildHull(points.begin(), points.end(), 1);

    long double answerFloating = perimeter(hull.begin(), hull.end()) + 2. * M_PI * l;
    long long answerFeet = answerFloating + 0.5;

    std::cout << answerFeet << std::endl;

    return 0;
}

