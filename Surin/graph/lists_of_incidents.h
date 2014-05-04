#ifndef LISTS_OF_ICIDENTS_H
#define LISTS_OF_ICIDENTS_H
#include "graph.h"
#include <algorithm>

class VectorIntList : public ListOfIncedents {
    private:
        std::vector<int> v;
    public:
        class Iterator: public IntIterator {
            private:
                friend VectorIntList;
                std::vector<int>::const_iterator it, end_it;
                Iterator(std::vector<int>::const_iterator it, std::vector<int>::const_iterator end_it)
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
        VectorIntList(std::vector<int> && _v): v(_v) {
            std::sort(v.begin(), v.end());
            _v.resize(std::unique(v.begin(), v.end()) - v.begin());
        }
        virtual std::unique_ptr<IntIterator> begin() const {
            return std::unique_ptr<IntIterator>((IntIterator *)(new Iterator(v.begin(), v.end())));
        }
        virtual bool isConnected(int x) const {
            std::vector<int>::const_iterator it = std::lower_bound(v.cbegin(), v.cend(), x);
            if (it == v.end()) return false;
            return *it == x;
        }
};

class VectorBoolList : public ListOfIncedents {
    private:
        std::vector<bool> v;
    public:
        class Iterator: public IntIterator {
            private:
                friend VectorBoolList;
                int it;
                const std::vector<bool> & v;
                Iterator(const std::vector<bool> & v): v(v), it(0) {
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
        VectorBoolList(std::vector<bool> && _v): v(_v) {
        }
        virtual std::unique_ptr<IntIterator> begin() const {
            return std::unique_ptr<IntIterator>((IntIterator*) new Iterator(v));
        }
        virtual bool isConnected(int x) const {
            return v[x];
        }
};

class OneList : public ListOfIncedents {
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
        virtual std::unique_ptr<IntIterator> begin() const {
            return std::unique_ptr<IntIterator>((IntIterator*) new Iterator(val));
        }
        virtual bool isConnected(int x) const {
            return val == x;
        }
};


class FunctionalList: ListOfIncedents {
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
        virtual std::unique_ptr<IntIterator> begin() const {
            return std::unique_ptr<IntIterator>(new Iterator(pred, n));
        }
        virtual bool isConnected(int x) const {
            return pred(x);
        }
};

#endif
