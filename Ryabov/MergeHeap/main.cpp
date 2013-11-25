#include <iostream>
#include <queue>
#include <gtest/gtest.h>
#include <utility>
#include <ctime>
#include <map>
#define HEAP_DEBUG
#include "heap.h"

using namespace std;

TEST(Stress1, heap_int_1000000)
{
    priority_queue<int, vector<int>, greater<int> > h1;
    Heap<int> h2;
    int a1 = 0;
    int a2 = 0;
    int t1, t2;
    for (int i = 0; i < 1000000; ++i)
    {
        int k = rand() % 100;
        if (k < 40 && !h2.Empty())
        {
            t1 = clock();
            h1.pop();
            t2 = clock();
            a1 += t2 - t1;
            t1 = clock();
            h2.pop();
            t2 = clock();
            a2 += t2 - t1;
        }
        else
        {
            int q = (rand() << 16) + rand();
            t1 = clock();
            h1.push(q);
            t2 = clock();
            a1 += t2 - t1;
            t1 = clock();
            h2.push(q);
            t2 = clock();
            a2 += t2 - t1;
        }
        if (!h2.Empty())
        {
            t1 = clock();
            int q = h2.getMin();
            t2 = clock();
            a2 += t2 - t1;
            t1 = clock();
            int w = h1.top();
            t2 = clock();
            a1 += t2 - t1;
            if (!h1.empty())
            EXPECT_EQ(q, w);
        }
    }
    while (!h2.Empty())
    {
        EXPECT_EQ(h1.top(), h2.getMin());
        h1.pop();
        h2.pop();
    }
    cout << (double)a1 / CLOCKS_PER_SEC << " " << (double)a2 / CLOCKS_PER_SEC << endl;
}

TEST(Stress2, heap_long_long_1000000_greater)
{
    priority_queue<long long, vector<long long>, less<long long> > h1;
    Heap<long long, greater<long long> > h2;
    int a1 = 0;
    int a2 = 0;
    int t1, t2;
    for (int i = 0; i < 1000000; ++i)
    {
        int k = rand() % 100;
        if (k < 40 && !h2.Empty())
        {
            t1 = clock();
            h1.pop();
            t2 = clock();
            a1 += t2 - t1;
            t1 = clock();
            h2.pop();
            t2 = clock();
            a2 += t2 - t1;
        }
        else
        {
            int q = (long long)((rand() << 16) + rand()) + (rand() << 16) + rand();
            t1 = clock();
            h1.push(q);
            t2 = clock();
            a1 += t2 - t1;
            t1 = clock();
            h2.push(q);
            t2 = clock();
            a2 += t2 - t1;
        }
        if (!h2.Empty())
        {
            t1 = clock();
            int q = h2.getMin();
            t2 = clock();
            a2 += t2 - t1;
            t1 = clock();
            int w = h1.top();
            t2 = clock();
            a1 += t2 - t1;
            if (!h1.empty())
            EXPECT_EQ(q, w);
        }
    }
    while (!h2.Empty())
    {
        EXPECT_EQ(h1.top(), h2.getMin());
        h1.pop();
        h2.pop();
    }
    cout << (double)a1 / CLOCKS_PER_SEC << " " << (double)a2 / CLOCKS_PER_SEC << endl;
}

TEST(Stress3, set_int_1000000)
{
    set<int> h1;
    Heap<int> h2;
    map<int, Heap<int>::ConstIterator> m;
    int a1 = 0;
    int a2 = 0;
    int t1, t2;
    for (int i = 0; i < 1000000; ++i)
    {
        int k = rand() % 100;
        if (k < 40 && !h2.Empty())
        {
            int q = (rand() << 16) + rand();
            if (h1.upper_bound(q) != h1.end())
            {
                q = *h1.upper_bound(q);
                t1 = clock();
                h1.erase(q);
                t2 = clock();
                a1 += t2 - t1;
                t1 = clock();
                h2.Erase(m[q]);
                t2 = clock();
                a2 += t2 - t1;
            }
        }
        else
        {
            int q = (rand() << 16) + rand();
            if (h1.find(q) == h1.end())
            {
                t1 = clock();
                h1.insert(q);
                t2 = clock();
                a1 += t2 - t1;
                t1 = clock();
                m[q] = h2.push(q);
                t2 = clock();
                a2 += t2 - t1;
            }

        }
        if (!h2.Empty())
        {
            t1 = clock();
            int q = h2.getMin();
            t2 = clock();
            a2 += t2 - t1;
            t1 = clock();
            int w = *(h1.begin());
            t2 = clock();
            a1 += t2 - t1;
            if (!h1.empty())
            EXPECT_EQ(q, w);
        }
    }
    while (!h2.Empty())
    {
        EXPECT_EQ(*(h1.begin()), h2.getMin());
        h1.erase(h1.begin());
        h2.pop();
    }
    cout << (double)a1 / CLOCKS_PER_SEC << " " << (double)a2 / CLOCKS_PER_SEC << endl;
}

TEST(Full_check, set_int_10000)
{
    set<int> h1;
    Heap<int> h2;
    map<int, Heap<int>::ConstIterator> m;
    for (int i = 0; i < 10000; ++i)
    {
        int k = rand() % 100;
        if (k < 40 && !h2.Empty())
        {
            int q = (rand() << 16) + rand();
            if (h1.upper_bound(q) != h1.end())
            {
                q = *h1.upper_bound(q);
                h1.erase(q);
                h2.Erase(m[q]);
            }
        }
        else
        {
            int q = (rand() << 16) + rand();
            if (h1.find(q) == h1.end())
            {
                h1.insert(q);
                m[q] = h2.push(q);
            }

        }
        if (!h2.Empty())
        {
            int q = h2.getMin();
            int w = *(h1.begin());
            if (!h1.empty())
            EXPECT_EQ(q, w);
            EXPECT_TRUE(h2.check());
        }
    }
    while (!h2.Empty())
    {
        EXPECT_EQ(*(h1.begin()), h2.getMin());
        h1.erase(h1.begin());
        h2.pop();
    }
}

TEST (MERGE_TEST, set_int)
{
    const int maxp = 100;
    vector<set<int>*> h1;
    vector<Heap<int>*> h2;
    vector<map<int, Heap<int>::ConstIterator>*> m;
    set<int> fs;
    for (int i = 0; i < maxp; ++i)
    {
        h1.push_back(new set<int>());
        h2.push_back(new Heap<int>());
        m.push_back(new map<int, Heap<int>::ConstIterator>());
    }
    while(h1.size() > 1)
    {
        int k = rand() % 100;
        int z = rand() % h1.size();
        if (k < 40 && !h2[z]->Empty())
        {
            int q = (rand() << 16) + rand();
            if (h1[z]->upper_bound(q) != h1[z]->end())
            {
                q = *(h1[z]->upper_bound(q));
                h1[z]->erase(q);
                h2[z]->Erase((*m[z])[q]);
                fs.erase(q);
            }
        }
        else
        {
            int q = (rand() << 16) + rand();
            if (fs.find(q) == fs.end())
            {
                h1[z]->insert(q);
                (*m[z])[q] = h2[z]->push(q);
                fs.insert(q);
            }

        }
        if (!h2[z]->Empty())
        {
            int q = h2[z]->getMin();
            int w = *(h1[z]->begin());
            if (!h1[z]->empty())
            EXPECT_EQ(q, w);
            EXPECT_TRUE(h2[z]->check());
        }
        k = rand() % 1000;

        if (k < 5 )
        {
            h1[h1.size() - 2]->insert(h1.back()->begin(), h1.back()->end());
            h1.pop_back();
            h2[h2.size() - 2]->Merge(*h2.back());
            h2.pop_back();
            for (auto it = m.back()->begin(); it != m.back()->end(); ++it)
                (*m[m.size() - 2])[it->first] = it->second;
            m.pop_back();
        }
    }
}

#define TEST_BUILD
#ifdef TEST_BUILD
int main(int argc, char **argv)
{
    srand(time(NULL));
    testing :: InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#else
int main()
{
    return 0;
}
#endif // TEST_BUILD
