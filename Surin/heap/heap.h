#ifndef HEAP_H
#define HEAP_H

#include "comparator.h"
#include <list>
#include <memory.h>
#include <algorithm>
#include <vector>

template<class T, typename cmp=CLess<T>, int szlog=43>
class Heap {
public : 
    class Iterator;
protected: 
    class vert{
    public:
        T val;
        bool inv;
        vert * parent;
        std::list<vert*> l;
        typename std::list<vert*>::iterator pos;
        vert(const T & _val, vert * _par = nullptr) {
            val = _val;
            inv = false;
            parent = _par;
        }
        ~vert() {
            for (auto i = l.begin(); i != l.end(); i++)
                delete *i;
        }
    };

    std::list<vert*> l;
    vert * min = nullptr;
    cmp * less;
    int sz = 0;
    
    vert * hang(vert * a, vert * b) {
        if (!(*less)(a->val, b->val)) std::swap(a, b);
        b->parent = a;
        b->pos = a->l.insert(a->l.end(), b);
        return a;
    }

    void add(std::vector<vert *> & arr, vert * v) {
        while (v != nullptr) {
            int s = v->l.size();
            if (arr[s] != nullptr) {
                v = hang(v, arr[s]);
                arr[s] = nullptr;
            } else {
                arr[s] = v;
                v = nullptr;
            }
        }
    }

    void findmin() {
        min = nullptr;
        for (auto i = l.begin(); i != l.end(); i++) {
            if (min == nullptr || (*less)((*i)->val, min->val))
                min = *i;
        }
    }
    //after norm vertices in root list must contain valid self-positions
    void norm() {
        std::vector<vert*> arr(szlog, nullptr);
        for (auto i = l.begin(); i != l.end(); i++) {
            add(arr, *i);
        }
        l.clear();
        for (int i = 0; i < szlog; i++)
            if (arr[i] != nullptr) {
                arr[i]->parent = nullptr;
                arr[i]->pos = l.insert(l.end(), arr[i]);
            }
        findmin();
    }

    void rcut(vert * v) {
        if (v == nullptr) return;
        if (v->inv) {
            rcut(v->parent);
            cut(v);
        } else {
            v->inv = true;
        }
    }

    void cut(vert * v) {
        v->inv = false;
        if (v->parent == nullptr) return;
        //if v->parent == nullptr then v->pos may be invalid 
        v->parent->l.erase(v->pos);
        v->parent = nullptr;
        v->pos = l.insert(l.end(), v);
    }
    
    void decreaseKey(vert * v) {
        rcut(v->parent);
        cut(v);
        min = v;
    }
public:
    class Iterator{
        friend Heap<T, cmp>;
        vert * pos;
    public:
        Iterator(vert * _pos) {
            pos = _pos;
        }
        const T & operator *() {
            return pos->val;
        }
        Iterator(){pos = nullptr;}
        bool valid() {
            return pos != nullptr;
        }
    };

    Heap(cmp * _less = new cmp()){
        less = _less;
    }

    Heap(const T & x, cmp * _less = new cmp()) {
        vert * p = new vert(x);
        p->pos = l.insert(l.end(), p);
        min = p;
        less = _less;
    }

    void merge(Heap<T, cmp> * h) {
        l.splice(l.end(), h->l);
        if (min == nullptr) min = h->min;
        else if (h->min != nullptr && !(*less)(min->val, h->min->val))
            min = h->min;
        *h = Heap();
        sz += h->sz;
        //while ((1 << (szlog/2)) <= sz) szlog++;
    }

    const T & getMin() {
        return min->val;
    }

    Iterator insert(const T & x) {
        vert * g = new vert(x);
        if (min == nullptr || (*less)(g->val, min->val))
            min = g;
        g->pos = l.insert(l.end(), g);
        sz++;
        //while ((1 << (szlog/2)) <= sz) szlog++;
        return Iterator(g);
    }

    void removeMin() {
        norm(); // must call because min->pos may be invalid
        l.erase(min->pos);
        l.splice(l.end(), min->l);
        min->l.clear();
        delete min;
        sz--;
        norm();
    }

    void decreaseKey(Iterator it, const T & val) {
       it.pos->val = val;
       rcut(it.pos->parent);
       cut(it.pos);
       if ((*less)(it.pos->val, min->val)) min = it.pos;
    }

    void remove(Iterator it) {
        norm();
        decreaseKey(it.pos);
        l.erase(min->pos);
        l.splice(l.end(), min->l);
        min->l.clear();
        delete min;
        sz--;
        norm();
    }
    //TODO: fix memory leaks
    //fixed
    void clear() {
        for (auto i = l.begin(); i != l.end(); i++) {
           delete *i; 
        }
        l.clear();
        min = 0;
    }

    ~Heap() {
        clear();    
    }

    bool empty() {
        return l.empty();
    }
};


#endif 
