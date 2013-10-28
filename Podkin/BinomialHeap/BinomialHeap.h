#ifndef BINOMIALHEAP_H
#define BINOMIALHEAP_H

#include <cstdlib>

template<typename Type, class Comparator>
class Tree{
    public:
        Tree(Type val){
            value = val;
            right = parent = son = NULL;
            degree = 0;
        }
        bool operator < (Tree *tr){
            Comparator comp;
            return comp(value, tr->value);
        }
    private:
        Type value;
        Tree *right, *parent, *son;
        int degree;
};

template<typename Type, class Comparator>
class BinomialHeap{
    public:
        BinomialHeap(Type value){
            heap = new Tree<Type, Comparator>(value);
        }
        void insert(){
            //TODO
        }
        void erase(){
            //TODO
        }
        Type getMin(){
            //TODO
        }
        void ExtractMin(){
            //TODO
        }
        void DecreaseKey(){
            //TODO
        }
        void IncreaseKey(){
            //TODO
        }
    private:
        void MergeHeap(){
            //TODO
        }
        void MergeTrees(){
            //TODO
        }
        Tree<Type, Comparator> *heap = NULL;
};

#endif // BINOMIALHEAP_H
