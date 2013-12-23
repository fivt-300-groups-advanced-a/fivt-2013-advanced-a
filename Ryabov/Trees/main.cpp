#include <iostream>
#include <time.h>
#include <gtest/gtest.h>

#define TREE_DEBUG
#include "tree.h"
#include "some_trees.h"

using namespace std;

template <typename T>
class Min
{
    public:
    T operator()(T a, T b)
    {
        return min(a, b);
    }
};

template <typename T>
class Max
{
    public:
    T operator()(T a, T b)
    {
        return max(a, b);
    }
};

TEST (integrated_test, int_min_add_1000)
{
    int n = 1000;
    tree<tree_add<int, Min<int> >, int, int, int> tr(n);
    vector<int> v(n, 0);
    for (int i = 0; i < 10000; i++)
    {
        int l = 0, r = 0;
        while (l == r)
        {
            l = rand() % n;
            r = rand() % n;
            if (l > r)
                swap(l, r);
        }
        if (rand() % 2)
        {
            int x1 = v[l];
            for (int i = l + 1; i < r; i++)
                x1 = min(x1, v[i]);
            int x2 = tr.get(l, r);
            EXPECT_EQ(x1, x2);
        }
        else
        {
            int q = rand() % 100000;
            for (int i = l; i < r; i++)
                v[i] += q;
            tr.modify(l, r, q);
        }
    }
}

TEST (integrated_test, int_max_add_1000)
{
    int n = 1000;
    tree<tree_add<int, Max<int> >, int, int, int> tr(n);
    vector<int> v(n, 0);
    for (int i = 0; i < 10000; i++)
    {
        int l = 0, r = 0;
        while (l == r)
        {
            l = rand() % n;
            r = rand() % n;
            if (l > r)
                swap(l, r);
        }
        if (rand() % 2)
        {
            int x1 = v[l];
            for (int i = l + 1; i < r; i++)
                x1 = max(x1, v[i]);
            int x2 = tr.get(l, r);
            EXPECT_EQ(x1, x2);
        }
        else
        {
            int q = rand() % 100000;
            for (int i = l; i < r; i++)
                v[i] += q;
            tr.modify(l, r, q);
        }
    }
}

TEST (integrated_test, int_sum_add_1000)
{
    int n = 1000;
    tree<tree_add_sum<int>, int, int, int> tr(n);
    vector<int> v(n, 0);
    for (int i = 0; i < 10000; i++)
    {
        int l = 0, r = 0;
        while (l == r)
        {
            l = rand() % n;
            r = rand() % n;
            if (l > r)
                swap(l, r);
        }
        if (rand() % 2)
        {
            int x1 = v[l];
            for (int i = l + 1; i < r; i++)
                x1 += v[i];
            int x2 = tr.get(l, r);
            EXPECT_EQ(x1, x2);
        }
        else
        {
            int q = rand() % 100000;
            for (int i = l; i < r; i++)
                v[i] += q;
            tr.modify(l, r, q);
        }
    }
}

TEST (integrated_test, int_max_def_100)
{
    int n = 100;
    tree<tree_def<int, Max<int> >, int, int, int> tr(n);
    vector<int> v(n, 0);
    for (int i = 0; i < 10000; i++)
    {
        int l = 0, r = 0;
        while (l == r)
        {
            l = rand() % n;
            r = rand() % n;
            if (l > r)
                swap(l, r);
        }
        if (rand() % 2)
        {
            int x1 = v[l];
            for (int i = l + 1; i < r; i++)
                x1 = max(x1, v[i]);
            int x2 = tr.get(l, r);
            EXPECT_EQ(x1, x2);
        }
        else
        {
            int q = rand() % 100000;
            for (int i = l; i < r; i++)
                v[i] = q;
            tr.modify(l, r, q);
        }
    }
}


TEST (integrated_test, int_sum_add_10)
{
    int n = 10;
    tree<tree_add_sum<int>, int, int, int> tr(n);
    vector<int> v(n, 0);
    for (int i = 0; i < 100000; i++)
    {
        int l = 0, r = 0;
        while (l == r)
        {
            l = rand() % n;
            r = rand() % n;
            if (l > r)
                swap(l, r);
        }
        if (rand() % 2)
        {
            int x1 = v[l];
            for (int i = l + 1; i < r; i++)
                x1 += v[i];
            int x2 = tr.get(l, r);
            EXPECT_EQ(x1, x2);
        }
        else
        {
            int q = (rand() % 100) - (rand() % 100);
            for (int i = l; i < r; i++)
                v[i] += q;
            tr.modify(l, r, q);
        }
    }
}

TEST (integrated_test, int_sum_def_100)
{
    int n = 100;
    tree<tree_def_sum<int>, int, int, int> tr(n);
    vector<int> v(n, 0);
    for (int i = 0; i < 100000; i++)
    {
        int l = 0, r = 0;
        while (l == r)
        {
            l = rand() % n;
            r = rand() % n;
            if (l > r)
                swap(l, r);
        }
        if (rand() % 2)
        {
            int x1 = v[l];
            for (int i = l + 1; i < r; i++)
                x1 += v[i];
            int x2 = tr.get(l, r);
            EXPECT_EQ(x1, x2);
        }
        else
        {
            int q = (rand() % 100) - (rand() % 100);
            for (int i = l; i < r; i++)
                v[i] = q;
            tr.modify(l, r, q);
        }
    }
}

TEST (tree_check, test_create)
{
    int n = 100;
    vector<int> v(n, 1);
    tree<tree_def_sum<int>, int, int, int> tr(n, v);
    for (int i = 0; i < 100000; i++)
    {
        int l = 0, r = 0;
        while (l == r)
        {
            l = rand() % n;
            r = rand() % n;
            if (l > r)
                swap(l, r);
        }
        if (rand() % 2)
        {
            int x1 = v[l];
            for (int i = l + 1; i < r; i++)
                x1 += v[i];
            int x2 = tr.get(l, r);
            EXPECT_EQ(x1, x2);
        }
        else
        {
            int q = (rand() % 100) - (rand() % 100);
            for (int i = l; i < r; i++)
                v[i] = q;
            tr.modify(l, r, q);
        }
    }
}

TEST (tree_check, test_1000)
{
    int n = 100000;
    tree<fake_tree, int, int, int> tr(n);
    debug_k = 0;
    tr.modify(0, n, 0);
    tr.get(0, n / 2 - 1);
    tr.get(5, 6);
    EXPECT_LE(debug_k, 50);
}


TEST (hand_test, int_max_add_100)
{
    int n = 10;
    tree<tree_add<int, Max<int> >, int, int, int> tr(n);
    for (int i = 0; i < 100; i++)
        tr.modify(0, 3, 100);
    for (int i = 0; i < 100; i++)
        tr.modify(0, 3, -100);
    EXPECT_EQ(0, tr.get(0, n));
}



int main(int argc, char ** argv)
{
    srand(time(NULL));
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
