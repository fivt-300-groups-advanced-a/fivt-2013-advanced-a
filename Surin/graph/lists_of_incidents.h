#ifndef LISTS_OF_ICIDENTS_H
#define LISTS_OF_ICIDENTS_H
#include "graph.h"

class VectorIntList : public ListOfIncidents {
    private:
        std::vector<int> v;
    public:
        class Iterator: public IntIterator {
            private:
                friend VectorIntList;
                std::vector<int>::iterator it, end_it;
                Iterator(std::vector<int>::iterator it, std::vector<int>::iterator end_it)
                    : it(it), end_it(end_it) {
                }
            public:
                virtual void operator ++ () {
                    it++;
                }
                virtual int operator * () {
                    return *it;
                }
                virtual bool end() {
                    return end_it == it;
                }
        };
        VectorIntList(std::vector<int> _v) {
            v = std::move(_v);
            std::sort(v.begin(), v.end());
        }
        virtual std::unique_ptr<IntIterator> begin() {
            return std::unique_ptr<IntIterator>((IntIterator *)(new Iterator(v.begin(), v.end())));
        }
        virtual bool isConnected(int x) {
            std::vector<int>::iterator it = std::lower_bound(v.begin(), v.end(), x);
            if (it == v.end()) return false;
            return *it == x;
        }
};

class VectorBoolList : public ListOfIncidents {
    private:
        std::vector<bool> v;
    public:
        class Iterator: public IntIterator {
            private:
                friend VectorBoolList;
                int it;
                std::vector<bool> & v;
                Iterator(std::vector<bool> & v): v(v), it(0) {
                    while (it < v.size() && !v[it]) {
                        it++;
                    }
                }
            public:
                virtual void operator ++ () {
                    it++;
                    while (it < v.size() && !v[it])
                        it++;
                }
                virtual int operator * () {
                    return it;
                }
                virtual bool end() {
                    return v.size() == it;
                }
        };
        VectorBoolList(std::vector<bool> _v) {
            v = std::move(_v);
        }
        virtual std::unique_ptr<IntIterator> begin() {
            return std::unique_ptr<IntIterator>((IntIterator*) new Iterator(v));
        }
        virtual bool isConnected(int x) {
            return v[x];
        }
};

class OneList : public ListOfIncidents {
    private:
        int val;
    public:
        class Iterator : public IntIterator {
            private:
                friend OneList;
                int val;
                bool endf;
                Iterator(int val): val(val), endf(false){
                }
            public:
                virtual void operator ++ () {
                    endf = true;
                }
                virtual int operator * () {
                    return val;
                }
                virtual bool end() {
                    return endf;
                }
        };
        OneList(int val): val(val) {
        }
        virtual std::unique_ptr<IntIterator> begin() {
            return std::unique_ptr<IntIterator>((IntIterator*) new Iterator(val));
        }
        virtual bool isConnected(int x) {
            return val == x;
        }
};


class FunctionalList: ListOfIncidents {
    private:
        std::function<bool(int)> pred;
        int n;
    public:
        class Iterator : public IntIterator {
            private:
                friend FunctionalList;
                std::function<bool(int)> pred;
                int it, endf;
                Iterator(std::function<bool(int)> pred, int n): it(0), endf(n), pred(pred) {
                    while (it < endf && !pred(it)) {
                        it++;
                    }
                }
            public:
                virtual void operator ++ () {
                    it++;
                    while (it < endf && !pred(it))
                        it++;
                }
                virtual int operator * () {
                    return it;
                }
                virtual bool end() {
                    return it == endf;
                }
        };
        FunctionalList(std::function<bool(int)> pred, int n): pred(std::move(pred)), n(n) {
        }
        virtual std::unique_ptr<IntIterator> begin() {
            return std::unique_ptr<IntIterator>(new Iterator(pred, n));
        }
        virtual bool isConnected(int x) {
            return pred(x);
        }
};

std::vector<std::unique_ptr<ListOfIncidents> > map(int n, std::function<ListOfIncidents*(int)> pred);

#endif
