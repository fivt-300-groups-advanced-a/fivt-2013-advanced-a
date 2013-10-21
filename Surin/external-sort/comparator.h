#ifndef COMPARATOR_H
#define COMPARATOR_H
#include <utility>
#include <type_traits>

template<class T>
class Comp {
public:
    virtual bool operator ()(const T &, const T &) = 0;
};

template<class T>
class CEqual {
public:
    virtual bool operator()(const T & a, const T & b) {
        return false;
    }
};

template<class T>
class CLess: public Comp<T>{
public:
    virtual bool operator()(const T & a, const T & b) {
        return a < b;
    }
};

template<class T1, class T2, class cmp1=CLess<T1>, class cmp2=CLess<T2> >
class CPair: public Comp<std::pair<T1, T2> >{
    cmp1 & cc1;
    cmp2 & cc2;
public:
    CPair(cmp1 & _c1, cmp2 & _c2): cc1(_c1), cc2(_c2) {}
    CPair(cmp1 & _c1): cc1(_c1), cc2(*(new cmp2())) {}
    CPair(): cc1(*(new cmp1())), cc2(*(new cmp2())) {}
    virtual bool operator()(const std::pair<T1, T2> & a, const std::pair<T1, T2> & b) {
        if (cc1(a.first, b.first)) return true;
        if (cc1(b.first, a.first)) return false;
        return cc2(a.second, b.second);
    }
};

template<bool = false>
class Condition{};
template<>
class Condition<false>{private: virtual void INVALID_TYPE() = 0;};

// better than Ivaschenko
template<class T, class cmp=CLess<T> >
class CInvOp: Condition<std::is_base_of<Comp<T>, cmp>::value>, Comp<T> {
    cmp & C;
public:
    CInvOp(): C(*(new cmp())) {}
    CInvOp(cmp & _p): C(_p) {}
    virtual bool operator()(const T & a, const T & b) {
        if (!C(a, b) && !C(b, a)) return false;
        return !C(a, b);
    }
};

#endif
