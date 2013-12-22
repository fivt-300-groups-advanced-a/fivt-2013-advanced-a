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
#include "trees.h"
#include <iostream>

TEST(HandMade, AssMinTree) {
    AssMinTree<int, (int)1e+9> s(0, 4);
    s.push(0, 4, 1);
    EXPECT_EQ(s.fold(2, 4), 1);
    s.push(0, 4, 0);
    EXPECT_EQ(s.fold(3, 4), 0);
    s.push(2, 4, 8);
    s.push(3, 4, 9);
    EXPECT_EQ(s.fold(2, 4), 8);
    s.push(0, 1, 7);
    EXPECT_EQ(s.fold(0, 1), 7);
}

TEST(Stress, AssMinTree) {
    std::vector<int> v(10, RAND_MAX);
    AssMinTree<int, RAND_MAX> s(0, 10);
    for (int i = 0; i < 40; i++) {
        int l = rand() % (v.size() - 1);
        int r = rand()%(9 - l) + l + 1;
        int ss = rand();
        if (i % 2 == 0) {
            s.push(l, r, ss);
            for(int i = l; i < r; i++)
                v[i] = ss;
        } else {
            int mx = RAND_MAX;
            for(int i = l; i < r; i++)
                mx = std::min(mx, v[i]);
            EXPECT_EQ(mx, s.fold(l, r));
        }
    }
}


TEST(Stress, AssMaxTree) {
    std::vector<int> v(10, -RAND_MAX);
    AssMaxTree<int, -RAND_MAX> s(0, 10);
    for (int i = 0; i < 40; i++) {
        int l = rand() % (v.size() - 1);
        int r = rand()%(9 - l) + l + 1;
        int ss = rand();
        if (i % 2 == 0) {
            s.push(l, r, ss);
            for(int i = l; i < r; i++)
                v[i] = ss;
        } else {
            int mx = -RAND_MAX;
            for(int i = l; i < r; i++)
                mx = std::max(mx, v[i]);
            EXPECT_EQ(mx, s.fold(l, r));
        }
    }
}


TEST(Stress, AssSumTree) {
    std::vector<int> v(10, 0);
    AssSumTree<int, 0> s(0, 10);
    for (int i = 0; i < 40; i++) {
        int l = rand() % (v.size() - 1);
        int r = rand()%(9 - l) + l + 1;
        int ss = rand()%20;
        if (i % 2 == 0) {
            s.push(l, r, ss);
            for(int i = l; i < r; i++)
                v[i] = ss;
        } else {
            int mx = 0;
            for(int i = l; i < r; i++)
                mx = mx + v[i];
            EXPECT_EQ(mx, s.fold(l, r));
        }
    }
}

TEST(Stress, AddMinTree) {
    std::vector<int> v(10, 0);
    AddMinTree<int, 0, RAND_MAX> s(0, 10);
    s.push(0, 10, -RAND_MAX);
    for (int i = 0; i < 40; i++) {
        int l = rand() % (v.size() - 1);
        int r = rand()%(9 - l) + l + 1;
        int ss = rand()%20;
        if (i % 2 == 0) {
            s.push(l, r, ss);
            for(int i = l; i < r; i++)
                v[i] += ss;
        } else {
            int mx = RAND_MAX;
            for(int i = l; i < r; i++)
                mx = std::min(mx, v[i]);
            EXPECT_EQ(mx, s.fold(l, r));
        }
    }
}

TEST(Stress, AddMaxTree) {
    std::vector<int> v(10, 0);
    AddMaxTree<int, 0, -RAND_MAX> s(0, 10);
    s.push(0, 10, RAND_MAX);
    for (int i = 0; i < 40; i++) {
        int l = rand() % (v.size() - 1);
        int r = rand()%(v.size() - 1 - l) + l + 1;
        int ss = rand()%20;
        if (i % 2 == 0) {
            s.push(l, r, ss);
            for(int i = l; i < r; i++)
                v[i] += ss;
        } else {
            int mx = -RAND_MAX;
            for(int i = l; i < r; i++)
                mx = std::max(mx, v[i]);
            EXPECT_EQ(mx, s.fold(l, r));
        }
    }
}

TEST(Stress, AddSumTree) {
    std::vector<int> v(10, 0);
    AddSumTree<int, 0> s(0, 10);
    for (int i = 0; i < 40; i++) {
        int l = rand() % (v.size() - 1);
        int r = rand()%(v.size() - 1 - l) + l + 1;
        int ss = rand()%20;
        if (i % 2 == 0) {
            s.push(l, r, ss);
            for(int i = l; i < r; i++)
                v[i] += ss;
        } else {
            int mx = 0;
            for(int i = l; i < r; i++)
                mx = mx + v[i];
            EXPECT_EQ(mx, s.fold(l, r));
        }
    }
}

class DebugType{
public:
    int ptime;
    int l, r;
    bool zero;
    DebugType(int time, int l, int r): ptime(time), l(l), r(r), zero(false) {
    }
};

class DebugInit {
public:
    static void init(DebugType & x, int p) {
        x.l = p;
        x.r = p + 1;
        x.ptime = 0;
        x.zero = false;
    }
};

class DebugPush{
public:
    int ptime;
    bool id;
    int l, r;
    DebugPush(int l, int r): l(l), r(r) {
        id = true;
    }
    DebugPush(int l, int r, int ptime) : l(l), r(r), ptime(ptime) {
        id = false;
    }
    DebugType apply(DebugType t) {
        if (id) return t;
        EXPECT_LT(t.ptime, ptime);
        EXPECT_EQ(l, t.l);
        EXPECT_EQ(r, t.r);
        return DebugType(ptime, t.l, t.r);
    }
    DebugPush mergeWith(DebugPush p) {
        if (id) return p;
        if (!p.id)
            EXPECT_LT(p.ptime, ptime);
        return DebugPush(p.l, p.r, ptime);
    }
};

class DebugOp{
public:
    DebugType zero() {
        DebugType s(0, 0, 0);
        s.zero = true;
        return s;
    }
    DebugType operator () (DebugType a, DebugType b) {
        if (a.zero) return b;
        if (b.zero) return a;
        EXPECT_EQ(a.r, b.l);
        return DebugType(std::max(a.ptime, b.ptime), a.l, b.r);
    }
};

TEST(Stress, Internal) {
    std::vector<int> dtimes(20, 0);
    SegmentTree<DebugType, DebugOp, DebugPush, DebugInit> debugtree(0, dtimes.size(), DebugOp());
    int T = 0;
    for (int i = 0; i < 40; i++) {
        T++;
        int l = rand() % (dtimes.size() - 1);
        int r = rand()%(dtimes.size() - 1 - l) + l + 1;
        if (i % 2 == 0) {
            debugtree.push(l, r, DebugPush(l, r, T));
            for (int i = l; i < r; i++) {
                dtimes[i] = T;
            }
        } else {
            DebugType s = debugtree.fold(l, r);
            EXPECT_EQ(s.l, l);
            EXPECT_EQ(s.r, r);
            int mx = 0;
            for (int i = l; i < r; i++)
                mx = std::max(mx, dtimes[i]);
            EXPECT_EQ(mx, s.ptime);
        }
    }
}

TEST(HandMade, EqSegms) {
    EqSegms s(0, 4);
    s.set(0, 4, 0);
    EXPECT_EQ(1, s.get(0, 2));
    s.set(0, 2, 3);
    EXPECT_EQ(2,  s.get(1, 3));
    s.add(1, 3, 1);
    EXPECT_EQ(4, s.get(0, 4));
}

TEST(Algorithm, EqSegms) {
    std::vector<int> v(20, 0);
    EqSegms s(0, v.size());
    for (int i = 0; i < 40; i++) {
        int l = rand() % (v.size() - 1);
        int r = rand()%(v.size() - 1 - l) + l + 1;
        int ss = rand()%20;
        if (i % 3 == 0) {
            s.set(l, r, ss);
            for (int i = l; i < r; i++)
                v[i] = ss;
        } else if (i % 3 == 1) {
            s.add(l, r, ss);
            for (int i = l; i < r; i++)
                v[i] += ss;
        } else {
            int ans = 0;
            for (int i = l; i < r - 1; i++)
                ans += v[i] != v[i + 1];
            EXPECT_EQ(ans + 1, s.get(l, r));
        }
    }
}

TEST(HandMade, MaxSegm) {
    MaxSegmSum s(0, 10);
    s.set(0, 10, 0);
    s.set(0, 4, 1);
    s.set(4, 10, -1);
    EXPECT_EQ(s.get(0, 10), 4);
}

TEST(Algorithm, MaxSegm) {
    std::vector<int> v(30, 0);
    MaxSegmSum s(0, v.size());
    for (int i = 0; i < 80; i++) {
        int l = rand() % (v.size() - 1);
        int r = rand()%(v.size() - 1 - l) + l + 1;
        int ss = rand()%20;
        if (i % 2 == 0) {
            s.set(l, r, ss);
            for (int i = l; i < r; i++)
                v[i] = ss;
        } else {
            int ans = 0;
            for (int i = l; i < r; i++) {
                int sum = 0;
                for (int j = i; j < r; j++) {
                    sum += v[j];
                    ans = std::max(ans, sum);
                }
            }
            EXPECT_EQ(ans, s.get(l, r));
        }
    }
}

int main(int argc, char ** argv) { //1231
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
