#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <map>
#include <stdlib.h>
#include <set>
#include <fstream>
#include "gtest/gtest.h"
#include "heapsort.h"

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
    long long dist(point p) const
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

//sample passed all test, realtest — where sort is replaced with heapsort
/*
TEST(convex, sample)
{
    std::ifstream in;
    std::ifstream right;
    std::cerr << "entered" << std::endl;
    for (int it = 1; it <= 41; ++it)
    {  
        std::cerr << "it = " << it << std::endl;      
        char ch[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        sprintf(ch, "convexhull/%02d.in", it);
        in.open(ch);
        sprintf(ch, "convexhull/%02d.out", it);
        right.open(ch);
        ans.clear();
        t.clear();

        bool rot = false;
        int n;
        in >> n;
        std::cerr << "n = " << n << std::endl;      
        for (int i = 0; i < n; ++i)
            in >> p[i].x >> p[i].y;

        if (n % 2)
            rot = true;

        std::sort(p, p + n, cmpx);
        for (int i = 0; i < n; ++i)
            if (i == 0 || p[i].x != p[i - 1].x || p[i].y != p[i - 1].y)
                t.push_back(p[i]);
        for (size_t i = 0; i < t.size(); ++i)
            p[i] = t[i];
        n = t.size();
        std::cerr << "n after deleting the same " << n << std::endl;

        if (n == 1)
        {
            int tmp;
            right >> tmp;
            EXPECT_EQ(tmp, 1);
            right >> tmp;
            EXPECT_EQ(tmp, p[0].x);
            right >> tmp;
            EXPECT_EQ(tmp, p[0].y);
        }
        else if (n == 2)
        {
            int tmp;
            right >> tmp;
            EXPECT_EQ(tmp, 2);
            for (int i = 0; i < 2; ++i)
            {
                right >> tmp;
                EXPECT_EQ(tmp, p[i].x);
                right >> tmp;
                EXPECT_EQ(tmp, p[i].y);
            }
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

            int tmp;
            right >> tmp;
            EXPECT_EQ(tmp, ans.size());
            for (size_t i = 0; i < ans.size(); ++i)
            {
                right >> tmp;
                EXPECT_EQ(tmp, ans[i].x);
                right >> tmp;
                EXPECT_EQ(tmp, ans[i].y);
            }
        }        
        in.close();
        right.close();
    }
}
*/

struct CmpWithState
{
    CmpWithState()
    {
        _p = point(0,0);
    }
    CmpWithState(point new_p)
    {
        _p = new_p;
    }
    bool operator()(point a, point b) const
    {
        long long s = (a - _p)  * (b - _p);
        if (s > 0)
            return true;
        if (s == 0 && _p.dist(a) < _p.dist(b))
            return true;
        return false;
    }
    point _p;
};

TEST(convex, comparatorWithState)
{
    std::ifstream in;
    std::ifstream right;
    std::cerr << "entered" << std::endl;
    for (int it = 1; it <= 41; ++it)
    {  
        std::cerr << "it = " << it << std::endl;      
        char ch[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
        sprintf(ch, "convexhull/%02d.in", it);
        in.open(ch);
        sprintf(ch, "convexhull/%02d.out", it);
        right.open(ch);
        ans.clear();
        t.clear();

        bool rot = false;
        int n;
        in >> n;
  //      std::cerr << "n = " << n << std::endl;      
        for (int i = 0; i < n; ++i)
            in >> p[i].x >> p[i].y;

        if (n % 2)
            rot = true;

        heapsort(p, p + n, cmpx);
        for (int i = 0; i < n; ++i)
            if (i == 0 || p[i].x != p[i - 1].x || p[i].y != p[i - 1].y)
                t.push_back(p[i]);
        for (size_t i = 0; i < t.size(); ++i)
            p[i] = t[i];
        n = t.size();
        //std::cerr << "n after deleting the same " << n << std::endl;

        if (n == 1)
        {
            int tmp;
            right >> tmp;
            EXPECT_EQ(tmp, 1);
            right >> tmp;
            EXPECT_EQ(tmp, p[0].x);
            right >> tmp;
            EXPECT_EQ(tmp, p[0].y);
        }
        else if (n == 2)
        {
            int tmp;
            right >> tmp;
            EXPECT_EQ(tmp, 2);
            for (int i = 0; i < 2; ++i)
            {
                right >> tmp;
                EXPECT_EQ(tmp, p[i].x);
                right >> tmp;
                EXPECT_EQ(tmp, p[i].y);
            }
        }
        else
        {
            int j = 0;
            for (int i = 1; i < n; ++i)
                if ((p[i].y < p[j].y) || ((p[i].y == p[j].y) && (p[i].x < p[j].x)))
                    j = i;

            std::swap(p[0], p[j]);
            p0 = p[0];
            heapsort(p + 1, p + n, CmpWithState(p0));

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

            int tmp;
            right >> tmp;
            EXPECT_EQ(tmp, ans.size());
            for (size_t i = 0; i < ans.size(); ++i)
            {
                right >> tmp;
                EXPECT_EQ(tmp, ans[i].x);
                right >> tmp;
                EXPECT_EQ(tmp, ans[i].y);
            }
        }        
        in.close();
        right.close();
    }
}