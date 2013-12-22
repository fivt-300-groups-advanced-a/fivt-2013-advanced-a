#ifndef OPERATIONS_H
#define OPERATIONS_H
#include <utility>

template<class T1, class O1, class T2, class O2>
struct PairOperation {
    typedef std::pair<T1, T2> IType;
    typedef PairOperation<T1, O1, T2, O2> Push;
    IType operator ()(const IType & a, const IType & b) {
        return std::make_pair(p.first(a.first, b.first), p.second(b.first, b.second));
    }
    IType zero() const {
        return std::make_pair(p.first.zero(), p.second.zero());
    }
    PairOperation() {
    }
    PairOperation(O1 o1, O2 o2) {
        p = std::make_pair(o1, o2);
    }
private:
    std::pair<O1, O2> p;
};

template<class T1, class P1, class T2, class P2>
struct PairPush {
    typedef std::pair<T1, T2> IType;
    typedef PairPush<T1, P1, T2, P2> Push;
    IType apply(const IType & a) const {
        return std::make_pair(p.first.apply(a.first), p.second.apply(a.second));
    }
    Push mergeWith(const Push & a) const {
        return Push(p.first.mergeWith(a.first), p.second.mergeWith(a.second), a.l, a.r);
    }
    PairPush(int l, int r) : l(l), r(r) {
        p = std::make_pair(P1(l, r), P2(l, r));
    }
    PairPush(P1 a, P2 b, int l, int r): l(l), r(r) {
        p = std::make_pair(a, b);
    }
    PairPush(const std::pair<P1, P2> & p): p(p){
    }
private:
    std::pair<P1, P2> p;
    int l, r;
};


template<class T>
class Plus{
public:
    static T plus(const T & a, const T & b) {
        return a + b;
    }
    static T mult(const T & a, int c) {
        return a * c;
    }
};

template<class T>
class IdPlus{
public:
    static T operation(const T & a, const T & b) {
        return a + b;
    }
    static T mult(const T & a, int c) {
        return a;
    }
};

template<class T>
class Less{
public:
    static bool compare(const T & a, const T & b) {
        return a < b;
    }
};

template<class T, class comp=Less<T> >
class InvComp {
public:
    static T compare(const T & a, const T & b) {
        bool b1 = comp::compare(a, b);
        bool b2 = comp::compare(b, a);
        if (!b1 && !b2) return false;
        if (b1) return false;
        return true;
    }
};

template<class T, T id, class cmp=Less<T> >
class Compare{
public:
    T operator ()(const T & a, const T & b) {
        return cmp::compare(a, b)?a:b;
    }
    T zero() const {
        return id;
    }
};



template<class T, T id, class op=Plus<T> >
class Sum{
public:
    Sum() {
    }
    T zero() const {
        return id;
    }
    T operator() (const T & a, const T & b) {
        return op::plus(a, b);
    }
};


template<class T, T zero, class op=Plus<T> >
class AddPush{
private:
    T add;
    int l, r;
public:
    AddPush(const T & add): add(add) {
        l = -1e+9;
        r = 1e+9;
    }
    AddPush(T add, int l, int r) : add(add), l(l), r(r){
    }
    AddPush(int l, int r): add(zero), l(l), r(r) {
    }
    T apply (const T & a) {
        return op::plus(op::mult(add, r - l), a);
    }
    AddPush mergeWith(AddPush x) {
        return AddPush<T, zero, op>(op::plus(add, x.add), x.l, x.r);
    }
};


template<class T, T zero, class op=Plus<T> >
class SAddPush{
private:
    T add;
    int l, r;
public:
    SAddPush(const T & add) : add(add){
    }
    SAddPush(T add, int l, int r) : add(add), l(l), r(r){
    }
    SAddPush(int l, int r): add(zero), l(l), r(r) {
    }
    T apply (const T & a) {
        return op::plus(add, a);
    }
    SAddPush mergeWith(SAddPush x) {
        return SAddPush<T, zero, op>(op::plus(add, x.add), x.l, x.r);
    }
};


template<class T>
class SAssignPush {
    bool id;
    T ass;
public:
    SAssignPush(const T & ass): ass(ass) {
        id = false;
    }
    SAssignPush(int, int) {    
        id = true;
    }
    SAssignPush(int, int, T x): ass(x) {
        id = false;
    }
    T apply(const T & x) {
        if (id) return x;
        else return ass;
    }
    SAssignPush<T> mergeWith(SAssignPush<T> c) {
        if (id) return c;
        else return SAssignPush<T>(0, 0, ass);
    }
};

template<class T, class op=Plus<T> >
class AssignPush {
    bool id;
    T ass;
    int l, r;
public:
    AssignPush(const T & ass) : ass(ass){
        l = -1e+9;
        r = 1e+9;
        id = false;
    }
    AssignPush(int l, int r): l(l), r(r) {    
        id = true;
    }
    AssignPush(int l, int r, T x): ass(x), l(l), r(r) {
        id = false;
    }
    T apply(const T & x) {
        if (id) return x;
        else return op::mult(ass, r - l);
    }
    AssignPush<T> mergeWith(AssignPush<T> c) {
        if (id) return c;
        else return AssignPush<T>(c.l, c.r, ass);
    }
};

#endif
