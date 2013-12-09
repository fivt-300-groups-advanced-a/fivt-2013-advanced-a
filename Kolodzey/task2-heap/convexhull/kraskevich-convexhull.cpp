//1877 informatics
#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <map>
#include <stdlib.h>
#include <set>
#include <fstream>

const int N = 100 * 1000;

long long sqr(long long a)
{
    return a * a;
}

struct point
{
    long long x, y;
    point() {}
    point(long long _x, long long _y): x(_x), y(_y) {}
    point operator - (point p)
    {
        return point(x - p.x, y - p.y);
    }
    long long operator * (point p)
    {
        return x * p.y - p.x * y;
    }
    long long dist(point p)
    {
        return sqr(x - p.x) + sqr(y - p.y);
    }
};

point p0;

bool cmp(point a, point b)
{
    long long s = (a - p0)  * (b - p0);
    if (s > 0)
        return true;
    if (s == 0 && p0.dist(a) < p0.dist(b))
        return true;
    return false;
}

bool cmpx(point a, point b)
{
    return (a.x < b.x) || ((a.x == b.x) && (a.y < b.y));
}

point p[N];
std::vector<point> ans;
std::vector<point> t;

int main()
{
    std::ifstream in;
    std::ofstream out;
    for (int it = 1; it <= 41; ++it)
    {
        in.clear();
        out.clear();
        char ch[7] = {0, 0, 0, 0, 0, 0, 0};
        sprintf(ch, "%02d.in", it);
        in.open(ch);
        std::cerr << ch << std::endl;
        sprintf(ch, "%02d.out", it);
        out.open(ch);
        ans.clear();
        t.clear();
        bool rot = false;
        int n;
        in >> n;
        std::cerr << it << " " << n << std::endl;
        for (int i = 0; i < n; ++i)
            in >> p[i].x >> p[i].y;

        if (n % 2)
            rot = true;

        std::sort(p, p + n, cmpx);
        for (int i = 0; i < n; ++i)
            if (i == 0 || p[i].x != p[i - 1].x || p[i].y != p[i - 1].y)
                t.push_back(p[i]);
        for (int i = 0; i < t.size(); ++i)
            p[i] = t[i];
        n = t.size();

        if (n == 1)
        {
            out << 1 << std::endl << p[0].x << " " << p[0].y << std::endl;
            std::cerr << "gened1 " << it << std::endl;
        }
        else if (n == 2)
        {
            out << 2 << std::endl;
            for (int i = 0; i < 2; ++i)
                out << p[i].x << " " << p[i].y << std::endl;
            std::cerr << "gened2 " << it << std::endl;
        }
        else
        {
            int j = 0;
            for (int i = 1; i < n; ++i)
                if ((p[i].y < p[j].y) || ((p[i].y == p[j].y) && (p[i].x < p[j].x)))
                    j = i;

            std::swap(p[0], p[j]);
            p0 = p[0];
            std::sort(p + 1, p + n, cmp);

            ans.push_back(p[0]);
            ans.push_back(p[1]);
            for (int i = 2; i < n; ++i)
            {
                while (ans.size() >= 2)
                {
                    point ppp = ans.back();
                    point pppp = ans[ans.size() - 2];
                    if ((p[i] - ppp) * (ppp - pppp) < 0)
                        break;
                    ans.pop_back();
                }
                ans.push_back(p[i]);
            }

            if (rot)
                reverse(ans.begin(), ans.end());

            out << ans.size() << std::endl;
            for (int i = 0; i < ans.size(); ++i)
                out << ans[i].x << " " << ans[i].y << std::endl;
            std::cerr << "gened " << it << std::endl;
        }        
        in.close();
        out.close();
    }
    return 0;
}