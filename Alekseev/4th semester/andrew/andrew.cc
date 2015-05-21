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

template<class InputItA, class InputItB, class OutputIt, class Pred>
OutputIt merge(InputItA beginA, InputItA endA,
        InputItB beginB, InputItB endB,
        OutputIt out, Pred cmp, unsigned int concurrency)
{
    if (concurrency <= 1)
            // || !isRAIt(beginA) || !isRAIt(beginB) || !isRAIt(out))
        return std::merge(beginA, endA, beginB, endB, out, cmp);

    std::size_t dist = std::distance(beginA, endA);

    if (dist < 10)
        return std::merge(beginA, endA, beginB, endB, out, cmp);

    InputItA midA = std::next(beginA, dist / 2);
    InputItB midB = std::lower_bound(beginB, endB, *midA, cmp);

    std::size_t leftSize = std::distance(beginA, midA) + std::distance(beginB, midB);

    auto leftFuture = std::async(std::launch::async,
            &std::merge<InputItA, InputItB, OutputIt, Pred>,
            beginA, midA, beginB, midB, out, cmp);
    OutputIt ret = std::merge(
            midA, endA, midB, endB, std::next(out, leftSize), cmp);
    leftFuture.wait();
    return ret;
}

template<class InputIt, class OutputIt>
OutputIt copy(InputIt begin, InputIt end, OutputIt out, unsigned int concurrency)
{
    if (concurrency <= 1)
            // || !isRAIt(begin) || !isRAIt(out))
        return std::copy(begin, end, out);
    std::size_t leftSize = std::distance(begin, end);
    InputIt mid = next(begin, leftSize);
    auto leftFuture = std::async(std::launch::async,
            &std::copy<InputIt, OutputIt>, begin, mid, out);
    OutputIt ret = std::copy(mid, end, out);
    leftFuture.wait();
    return ret;
}

template<class InputIt, class Pred>
void inplace_merge(InputIt begin, InputIt mid, InputIt end,
        Pred cmp, unsigned int concurrency)
{
    std::vector<typename std::iterator_traits<InputIt>::value_type> tmp;
    tmp.resize(distance(begin, end));
    merge(begin, mid, mid, end, tmp.begin(), cmp, concurrency);
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

        size_t pairs = bounds.size() / 2;

        newBounds.reserve((bounds.size() + 1) / 2);
        merge_futures.reserve(pairs);

        unsigned int conc_left = concurrency;
        unsigned int conc_step = concurrency / pairs;

        for (std::size_t i = 0; i + 2 < bounds.size(); i += 2)
        {
            if (i + 4 >= bounds.size())
                conc_step = conc_left;
            merge_futures.push_back(std::async(std::launch::async,
                &inplace_merge<RAIt, Pred>,
                bounds[i], bounds[i+1], bounds[i+2], cmp, conc_step));
            conc_left -= conc_step;
            newBounds.push_back(std::move(bounds[i]));
            if (i + 4 == bounds.size())
                newBounds.push_back(std::move(bounds[i+3]));
        }
        for (auto& f : merge_futures)
            f.wait();
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

int main(int argc, char **argv)
{
    std::cin.sync_with_stdio(false);

    int n;//, l;
    std::cin >> n;// >> l;

    std::vector<Point> points(n);
    for (Point& p : points)
        std::cin >> p.x >> p.y;

    unsigned int concurrency = std::thread::hardware_concurrency();
    if (argc >= 2)
        concurrency = atoi(argv[1]);

    auto hull = buildHull(points.begin(), points.end(), concurrency);

    // long double answerFloating = perimeter(hull.begin(), hull.end()) + 2. * M_PI * l;
    // long long answerFeet = answerFloating + 0.5;

    // std::cout << answerFeet << std::endl;
    std::cout << hull.size() << std::endl;
    for (Point &p : hull)
        std::cout << p.x << ' ' << p.y << std::endl;

    return 0;
}

