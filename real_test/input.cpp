#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <cstring>
#include <cassert>
#include <cstdlib>
#include <cstdio>

struct Apple {
    int x;
    const char* y;

    explicit Apple(int x, std::string s):x(x), y(s.c_str()) {
    }

    ~Apple() {
        delete[] y;
    }
};


template <class T>
struct PairRefs {
    template <class U>
    PairRefs(U a, U b) :a(a), b(b) {
    }

    const T& a;
    const T& b;
};


template <class T>
class VectorWithDefault {
 public:
    VectorWithDefault(int n):a(n) {
    }

    const T& operator[] (unsigned int i) {
        return get(i, T());
    }

    const T& get(unsigned int i, T def) {
        if (i >= a.size()) return def;
        return a[i];
    }
 private:
    std::vector<T> a;
};


class Interface {
 public:
    int x() {
        return 0;
    }
    int y() {
        return 0;
    }

};

class Impl : public Interface {
 public:
    Impl (int x, int y) {
        a = new int[2];
        a[0] = x;
        a[1] = y;
    }
    virtual int x() const { return a[0]; }
    virtual int y() const { return a[1]; }
    virtual ~Impl() {
    }
 private:
    int* a;
};


int main() {
    Apple a(5, "five");
    assert(a.x == 5);
    assert(std::string(a.y) == "five");

    std::vector<int> px, py;
    for(int i = 0; i < 120; ++i) {
        px.push_back(i + 9);
        py.push_back(px.back() * 7);
    }
    for(int i = 0; i * 3 <= px.size(); ++i) {
        PairRefs<int> p1(px[i * 3], py[i * 3]);
        PairRefs<int> p2(px[i * 3 + 1], py[i * 3 + 1]);
        PairRefs<int> p3(px[i * 3 + 2], py[i * 3 + 2]);
        assert(p1.a == px[i * 3] && p1.b == py[i * 3]);
        assert(p2.a == px[i * 3 + 1] && p2.b == py[i * 3 + 1]);
        assert(p3.a == px[i * 3 + 2] && p3.b == py[i * 3 + 2]);
    }

    Interface* x = new Impl(7, 3);
    assert(x->x() == 7 && x->y() == 3);
    free(x);
    return 0;
}

