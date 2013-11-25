#include <gtest/gtest.h>
#include <cstdio>
#include <utility>
#include <list>
#include <ctime>
#include <cstdlib>
#include <queue>
#include <vector>
#include <algorithm>
#include <ctime>
template<class T, class cmp>
class TestAccess;
#include "heap.h"

template<class T, class cmp>
class TestHeap : public Heap<T, cmp>{
    friend TestAccess<T, cmp>;
};

int fib[300];

template<class T, class cmp=CInvOp<T, CLess<T> > >
class TestAccess{
public:
    typedef TestHeap<T, cmp> Heap;
    typedef typename Heap::vert tvert;
    Heap & h;
    TestAccess(Heap & _h): h(_h) {}
    void checkHeap() {
        int rr = 0; 
        for (auto i = h.l.begin(); i != h.l.end(); i++) {
            rr += checkVertex(*i);
            EXPECT_EQ((*i)->parent, nullptr);
        }
        EXPECT_EQ(rr, h.sz);
    }
private:
    int checkVertex(tvert * v) {
        int res = 1;
        int maxsz = 0;
        for (auto it = v->l.begin(); it != v->l.end(); it++) {
            EXPECT_TRUE((*h.less)(v->val, (*it)->val));
            EXPECT_EQ((*it)->parent, v);
            EXPECT_EQ(it, (*it)->pos);
            maxsz = std::max(maxsz, (int)(*it)->l.size());
            res += checkVertex(*it);
        }
        EXPECT_GE(maxsz, ((int)v->l.size()) - 2);
        EXPECT_LE(fib[v->l.size()], res);
        return res;
    }
};


TEST(Simple, HeapTest) {
    std::priority_queue<int> q;
    TestHeap<int, CInvOp<int, CLess<int> > > h;
    for (int i = 0; i < 20; i++) {
        int tmp = rand();
        ASSERT_NO_THROW(h.insert(tmp));
        q.push(tmp);
        EXPECT_EQ(h.getMin(), q.top());
    }
    while (!q.empty()) {
        ASSERT_FALSE(h.empty());
        EXPECT_EQ(h.getMin(), q.top());
        ASSERT_NO_THROW(h.removeMin());
        q.pop();
    }
}

TEST(Hard, HeapTest) {
    std::priority_queue<int> q;
    TestHeap<int, CInvOp<int, CLess<int> > > h;
    TestAccess<int> hh(h);
    hh.checkHeap();
    for (int i = 0; i < 20; i++) {
        int tmp = rand();
        ASSERT_NO_THROW(h.insert(tmp));
        q.push(tmp);
        EXPECT_EQ(h.getMin(), q.top());
        hh.checkHeap();
    }
    while (!q.empty()) {
        ASSERT_FALSE(h.empty());
        EXPECT_EQ(h.getMin(), q.top());
        ASSERT_NO_THROW(h.removeMin());
        q.pop();
        hh.checkHeap();
    }
}


TEST(Hard, DecreaseKeyTest) {
    const int n = 10;
    typedef std::pair<int, int> pii;
    typedef CInvOp<pii, CLess<pii> > cmp;
    typedef TestHeap<pii, cmp>  THeap;
    THeap h;
    TestAccess<pii, cmp> hh(h);
    THeap::Iterator it[n];
    std::set<pii> ss;
    for (int i = 0; i < n; i++) {
        it[i] = h.insert(std::make_pair(i, 0));
        ss.insert(std::make_pair(i, 0));
    }
    for (int i = 0; i < n * n; i++) {
        int cnt = (rand())%n;
        EXPECT_EQ(*(--ss.end()), h.getMin());
        EXPECT_EQ((*it[cnt]).first, cnt);
        ss.erase(*it[cnt]);
        pii pp(cnt, (*it[cnt]).second + 1);
        h.decreaseKey(it[cnt], pp);
        ss.insert(pp);
        hh.checkHeap();
    }
}
const int n = 1000000;
Heap<int, CLess<int> > hh[n];
Heap<int, CLess<int> >::Iterator it[n];

TEST(Nightmare, StressTest) {
    typedef Heap<int, CLess<int> > THeap;
    for (int i = 0; i < n; i++) {
        it[i] = hh[i].insert(rand());
    }
    int ll = n;
    const int operations = 4;
    std::function<void()> func[operations] = {
    [&](){
        int cnt = rand()%n;
        int bb;
        while ((bb = rand()%n) == cnt);
        hh[cnt].merge(&hh[bb]);
        hh[bb] = THeap();
        it[bb] = THeap::Iterator();
    },
    [&](){
        int cnt = rand()%n;
        it[cnt] = hh[cnt].insert(rand());
    },
    [&]() {
        int cnt = rand()%n;
        if (it[cnt].valid()) {
            hh[cnt].remove(it[cnt]);
        }
        it[cnt] = THeap::Iterator();
    },
    [&](){
        int cnt = rand()%n;
        if (it[cnt].valid())
            hh[cnt].decreaseKey(it[cnt], *(it[cnt]) - 1);
    }
    };
    for (int i = 0; i < n * 10; i++) {
        func[rand()%operations]();
    }
}

TEST(Nigtmare, DecreaseKeyStress) {
    Heap<int> h;
    for (int i = 0; i < n; i++) {
        it[i] = h.insert(rand());
    }
    int ll = 0;
    for (int i = 0; i < n; i++) {
        if (i % 10000 == 0) {
            h.remove(it[ll++]);
            it[ll - 1] = Heap<int>::Iterator();
        }
        int cc = rand()%(n - ll  - 1) + ll + 1;
        ASSERT_LT(cc, n);
        ASSERT_LT(ll, cc);
        h.decreaseKey(it[cc], *(it[cc]) - 1);
    }
}

int main(int argc, char ** argv) { //1231
    srand(/*time(0)*/1212311);
    fib[0] = 1;
    for (int i = 1; i < 100; ++i) {
        fib[i] = 2;
        for (int j = 0; j < i - 1; j++)
            fib[i] += fib[j];
    }
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
