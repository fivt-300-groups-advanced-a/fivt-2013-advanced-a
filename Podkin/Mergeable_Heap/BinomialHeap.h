#ifndef BINOMIALHEAP_H
#define BINOMIALHEAP_H


template<Type>
class Tree{
    public:
        Tree(Type val){
            value = val;
            right = parent = son = NULL;
            degree = 0;
        }
        Tree(Type val, Tree *p){
            value = val;
            parent = p;
            right = son = NULL;
            degree = 0;
        }
    privat:
        Type value;
        Tree *right, *parent, *son;i
        int degree;
};

template<Type>
class BinomialHeap{
    public:
        template<Comparator>
        BinomialHeap(Comparator comp, Type value){
            heap = new Tree<Type>(value);
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
        Tree *heap = NULL;      
};

#endif BINOMIALHEAP_H
