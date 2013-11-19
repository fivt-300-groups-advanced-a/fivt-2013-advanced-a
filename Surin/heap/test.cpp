#include <gtest/gtest.h>
#include <cstdio>
#include <utility>
#include <list>
#include <ctime>
#include <cstdlib>
#include <queue>

template<class T, class cmp>
class TestAccess;
#include "heap.h"

template<class T, class cmp>
class TestHeap : public Heap<T, cmp>{
    friend TestAccess<T, cmp>;
};


template<class T, class cmp=CInvOp<T, CLess<T> > >
class TestAccess{
public:
    typedef TestHeap<T, cmp> Heap;
    typedef typename Heap::vert tvert;
    Heap & h;
    TestAccess(Heap & _h): h(_h) {}
    void checkHeap() {
        for (auto i = h.l.begin(); i != h.l.end(); i++) {
            checkVertex(*i);
            EXPECT_EQ((*i)->parent, nullptr);
        }
    }
private:
    void checkVertex(typename Heap::vert * v) {
        for (auto it = v->l.begin(); it != v->l.end(); it++) {
            EXPECT_TRUE((*h.less)(v->val, (*it)->val));
            EXPECT_EQ((*it)->parent, v);
            EXPECT_EQ(it, (*it)->pos);
            EXPECT_LE((*it)->l.size(), v->l.size() + 1);
            checkVertex(*it);
        }
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

TEST(Simple, AllTest) {
    
}

TEST(Hard, Invariants_tests) {
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

TEST(Hard, StressTest) {
    
}

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
