#ifndef FIBONACCI_HEAP

#define FIBONACCI_HEAP

#include <cassert>
#include <cstdlib>

template<class T, class Comparator> class Node {
    public:
        Node(){
            key = 0;
            par = left = right = son = NULL;
            degree = 0;
            deleted = false;
        }

        explicit Node(T elem){
            key = elem;
            par = left = right = son = NULL;
            degree = 0;
            deleted = false;
        }

    private:
        T key;
        Node *par, *left, *right, *son;
        int degree;
        bool deleted;
};

template<class T, class Comparator> class FibonacciHeap {
    public:
        FibonacciHeap(){
            size = 0;
            root = NULL;
        }

        explicit FibonacciHeap(T elem){
            root = new Node<T, Comparator>(elem);
            int sz = 1;
        }


        T getMin(){
            assert(root);
            return root->key;
        }

        void merge(FibonacciHeap<T, Comparator> *to_merge){
            if (!to_merge->root)
                return;
            if (!root)
                root = to_merge->root;
            to_merge->root->right = root->right;
            to_merge->root->left = root;
            root->right->left = to_merge;
            root->right = to_merge;
            size += to_merge.size;
            if (getMin() > to_merge->getMin)
                root = to_merge->root;
        }

        void insert(T elem){
            FibonacciHeap<T, Comparator> new_heap = new FibonacciHeap(elem);
            merge(new_heap);
        }

        int extractMin(){
            int result = getMin();
            
            consolidate();
        }

    private:
        int size;
        Node<T, Comparator> *root;

        void consolidate(){

        }
};

#endif
