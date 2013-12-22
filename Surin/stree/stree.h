#ifndef STREE_H 
#define STREE_H
/*
Operation interface :
    IType operator ()(IType a, IType b)
    must be associative
    IType zero() const
Push interface :
    IType apply(IType a) const
    Push mergeWith(Push a) const
    costructor without arguments: returns id transformation
*/
//#include "operations.h"

template<class IType>
class SimpleInitializer{
public:
    static void init(IType & x, int p) {
    }
};

template<class IType, class Operation, class Push, class Initializer=SimpleInitializer<IType> >
class SegmentTree {
private:
    Operation op;
    bool deleteop;
    struct iVert {
        SegmentTree * owner;
        int l, r;
        iVert * lt, * rt;
        IType val;
        Push adtn;
        void norm() {
            if (this == 0) return; //valid because iVert and SegmentTree are not virtual
            val = adtn.apply(val);
            if (lt != 0) lt->adtn = adtn.mergeWith(lt->adtn);
            if (rt != 0) rt->adtn = adtn.mergeWith(rt->adtn);
            adtn = Push(l, r);
        }
        void recalc() {
            val = owner->op((lt == 0) ? owner->op.zero() : lt->val, (rt == 0) ? owner->op.zero() : rt->val);
        }
        iVert(int _l, int _r, SegmentTree * owner) : l(_l), r(_r), adtn(_l, _r), val(owner->op.zero()), owner(owner) {
            lt = rt = 0;
        }
        ~iVert() {
            delete lt; delete rt;
        }
    };
    iVert * make(int l, int r) {
        iVert * ths = new iVert(l, r, this);
        if (r - l > 1) {
            int m = (l + r) / 2;
            ths->lt = make(l, m);
            ths->rt = make(m, r);
            ths->recalc();
        }
        if (r == l + 1) {
            ths->val = op.zero();
            Initializer::init(ths->val, l);
        }
        return ths;
    }
    iVert * root;
    IType fold(iVert * root, int l, int r) {
        root->norm(); //-> norm
        if (root == 0 || r <= root->l || root->r <= l) 
            return op.zero();
        if (l <= root->l && root->r <= r) 
            return root->val;
        int m = (root->l + root->r) / 2;
        return op(fold(root->lt, l, r), fold(root->rt, l, r));
    }
    void push(iVert * root, int l, int r, Push ad) {
        root->norm(); //-> norm
        if (root == 0 || r <= root->l || root->r <= l) 
            return;
        if (l <= root->l && root->r <= r) { 
            root->adtn = ad.mergeWith(root->adtn);
            root->norm();
            return;
        }
        int m = (root->l + root->r) / 2;
        push(root->lt, l, r, ad);
        push(root->rt, l, r, ad);
        root->recalc();
    }
public:
    //all intervals l inclusive and r exclusive
    SegmentTree(int l, int r, Operation op): op(op) {
        root = make(l, r);
    }
    SegmentTree(int l, int r): op() {
        root = make(l, r);
    }
    IType fold(int l, int r) {
        return fold(root, l, r);
    }
    void push(int l, int r, Push ad) {
        push(root, l, r, ad);
    }
    ~SegmentTree() {
        delete root;
    }
};


#endif
