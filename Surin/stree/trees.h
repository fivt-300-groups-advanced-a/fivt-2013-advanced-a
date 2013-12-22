#ifndef TREES_H
#include "stree.h"
#include "operations.h"

template<class T, T inf>
using AssMinTree = SegmentTree<T, Compare<T, inf, Less<T> >, SAssignPush<T> >;

template<class T, T inf>
using AssMaxTree = SegmentTree<T, Compare<T, inf, InvComp<T, Less<T> > >, SAssignPush<T> >;

template<class T, T zero>
using AssSumTree = SegmentTree<T, Sum<T, zero>, AssignPush<T> >;

template<class T, T zero, T inf>
using AddMinTree = SegmentTree<T, Compare<T, inf, Less<T> >, SAddPush<T, zero, Plus<T>> >;
 
template<class T, T zero, T minf>
using AddMaxTree = SegmentTree<T, Compare<T, minf, InvComp<T, Less<T> > >, SAddPush<T, zero, Plus<T> > >;

template<class T, T zero>
using AddSumTree = SegmentTree<T, Sum<T, zero>, AddPush<T, zero, Plus<T> > >;


class EqSegms {
private:
    int gl, gr;
    AssSumTree<int, 0> diff;
    class Push {
        int ptype; //0 - id 1 - add 2 - assign
        int idata;
    public:
        Push(int l, int r) {
            ptype = 0;
        }
        Push(int l, int r, int ptype, int idata): ptype(ptype), idata(idata) {
        }
        int apply(int x) {
            if (ptype == 0) return x;
            if (ptype == 1) return idata + x;
            if (ptype == 2) return idata;
        }
        Push mergeWith(Push b) {
           if (ptype == 0) return b;
           if (ptype == 2) return *this;
           if (ptype == 1) return Push(0, 0, std::max(b.ptype, 1), b.idata + idata);
        }
    };
    class Op{
    public:
        int operator() (int a, int b) {
            return std::min(a, b);
        }
        int zero() {
            return 2e+9;
        }
    };
    struct Init{
        static void init(int & x, int p) {
            x = 0;
        }
    };
    SegmentTree<int, Op, Push, Init> elems; 
    int gett(int i) {
        return elems.fold(i, i + 1);
    }
public: 
    EqSegms(int l, int r): gl(l), gr(r), diff(l, r), elems(l, r) {
    }
    void set(int l, int r, int e) {
        elems.push(l, r, Push(0, 0, 2, e));
        diff.push(l, r - 1, 0);
        if (l != gl)
            diff.push(l - 1, l, gett(l - 1) != gett(l)); 
        if (r != gr)
            diff.push(r - 1, r, gett(r - 1) != gett(r));
    }
    void add(int l, int r, int e) {
        elems.push(l, r, Push(0, 0, 1, e));
        if (l != gl)
            diff.push(l - 1, l, gett(l - 1) != gett(l)); 
        if (r != gl) 
            diff.push(r - 1, r, gett(r - 1) != gett(r));
    }
    int get(int l, int r) {
        return diff.fold(l, r - 1) + 1;
    }
};

class MaxSegmSum {
private:
    struct Segm{
        int l, r;
        bool zero;
        int maxsf, maxpr, segm;
        int all;
    };    
    struct Init {
        static void init(Segm & x, int p) {
            x.l = p; x.r = p + 1;
            x.maxsf = 0;
            x.maxpr = 0;
            x.segm = 0; 
            x.all = 0;
            x.zero = false;
        }
    };
    struct Push {
        Push(int l, int r) {
            idd = true;
        }
        Push(int z): l(l), r(r), z(z), idd(false) {
        }
        Push(int l, int r, int z): l(l), r(r), z(z), idd(false){
        }
        Segm apply(Segm x) {
            if (idd) return x;
            Segm y;
            y.l = x.l; y.r = x.r;
            y.segm = y.maxsf = y.maxpr = std::max(0, z * (y.r - y.l));
            y.all = z * (y.r - y.l);
            y.zero = false;
            return y;
        }
        Push mergeWith(Push x) {
            if (idd) return x;
            return *this;
        }
    private:
        bool idd;
        int z;
        int l, r;
    };
    struct Op {
        Segm zero() {
            Segm x;
            x.zero = true;
            return x;
        }
        Segm operator () (Segm a, Segm b) {
            if (a.zero) return b;
            if (b.zero) return a;
            Segm res;
            res.l = a.l;
            res.r = b.r;
            res.maxpr = std::max(a.maxpr, b.maxpr + a.all);
            res.maxsf = std::max(b.maxsf, a.maxsf + b.all);
            res.segm = std::max(a.segm, std::max(b.segm, a.maxsf + b.maxpr));
            res.all = a.all + b.all;
            res.zero = false;
            return res;
        }
    };
    SegmentTree<Segm, Op, Push, Init> s;
public:
    MaxSegmSum(int l, int r): s(l, r) {
    }
    void set(int l, int r, int c) {
        s.push(l, r, c);
    }
    int get(int l, int r) {
        return s.fold(l, r).segm;
    }
};

#endif
