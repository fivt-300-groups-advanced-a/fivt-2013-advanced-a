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

    Apple(int x, std::string s) // +1 unused explicit
        :x(x),
         y(new char[s.size() + 1])
         /*y(s.c_str())*/ // +0.5 use after delete
    {
        strcpy((char*)y, s.c_str());
    }

    ~Apple() {
        delete[] y; // +0.5 double deletion
    }
};


template <class T>
struct PairRefs {
    // +1 strange template
    PairRefs(const T& a, const T& b) :a(a), b(b) { // +1 reference to local
    }

    const T& a;
    const T& b;
};


template <class T>
class VectorWithDefault {
 public:
    explicit VectorWithDefault(int n):a(n) { // +1 explicit
    }

    T operator[] (unsigned int i) const { // +0.5
        return get(i, T()); // +0.5 return reference to local
    }

    T get(unsigned int i, T def) const { // +0.5 const
        if (i >= a.size()) return def;// +0.5 return reference to local
        return a[i];
    }
 private:
    std::vector<T> a;
};


class Interface {
 public:
    virtual int x() const { // +0.5 virtual +0.5 const
        return 0;
    }
    virtual int y() const { // +0.5 virtual +0.5 const
        return 0;
    }

    virtual ~Interface() {} // +1
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
        delete[] a; // +1
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
    for(int i = 0; i * 3 < /*+1*/ px.size(); ++i) {
        PairRefs<int> p1(px[i * 3], py[i * 3]);
        PairRefs<int> p2(px[i * 3 + 1], py[i * 3 + 1]);
        PairRefs<int> p3(px[i * 3 + 2], py[i * 3 + 2]);
        assert(p1.a == px[i * 3] && p1.b == py[i * 3]);
        assert(p2.a == px[i * 3 + 1] && p2.b == py[i * 3 + 1]);
        assert(p3.a == px[i * 3 + 2] && p3.b == py[i * 3 + 2]);
    }

    Interface* x = new Impl(7, 3);
    assert(x->x() == 7 && x->y() == 3);
    delete x; // +1 destructor will not be called.
    return 0;
}

