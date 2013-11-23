#ifndef BINOMIALHEAP_H
#define BINOMIALHEAP_H

#include <cstdlib>

template<typename Type, class Comparator>
class BinomialHeap {
    //template <typename T, class Comp> friend class Tree;
    public:
        explicit BinomialHeap(Type val){
            value = val;
            parent = son = right = NULL;
            head = this;
            degree = 0;
        }
        void insert(Type value){
            BinomialHeap<Type, Comparator> add_element(value);
            mergeHeap(&add_element);
        }
        void reverseList(BinomialHeap<Type, Comparator> *last, BinomialHeap<Type, Comparator> *current){
            if (current == NULL){
                current->right = last;
                current->head = current;
                return;
            }
            reverseList(current, current->right);
            last->head = current->head;
            last->right = current;
        }
        Type getMin(){
            BinomialHeap<Type, Comparator> *heap = this;
            Type min_element = heap->value;
            Comparator comp;
            while (heap != NULL){
                if (comp(heap->value, min_element))
                    min_element = heap->value;
                heap = heap->right;
            }
            return min_element;
        }
        void extractMin(){
            Type min_element = getMin();
            BinomialHeap<Type, Comparator> *heap = this;
            Comparator comp;
            while (heap != NULL){
                if (!comp(heap->value,min_element) && !comp(min_element, heap->value)){
                    reverseList(NULL, heap->son);
                    mergeHeap(heap->son->head);
                    return;
                }
            }
        }
        void mergeHeap(BinomialHeap<Type, Comparator> *add_heap){
            BinomialHeap<Type, Comparator> *new_heap = NULL;
            BinomialHeap<Type, Comparator> *heap = this;
            add_heap = add_heap->head;
            heap = heap->head;
            if (heap != NULL){
                if (add_heap != NULL){
                    if (heap->degree < add_heap->degree){
                        new_heap = heap;
                        new_heap->head = new_heap;
                        heap = heap->right;
                    }
                    else{
                        new_heap = add_heap;
                        new_heap->head = new_heap;
                        add_heap = add_heap->right;
                    }
                }
                else{
                    new_heap = heap;
                    new_heap->head = new_heap;
                    heap = heap->right;
                }
            }
            else{
                if (heap != NULL){
                    if (heap->degree < add_heap->degree){
                        new_heap = heap;
                        new_heap->head = new_heap;
                        heap = heap->right;
                    }
                    else{
                        new_heap = add_heap;
                        new_heap->head = new_heap;
                        add_heap = add_heap->right;
                    }
                }
                else{
                    new_heap = add_heap;
                    new_heap->head = new_heap;
                    add_heap = add_heap->right;
                }
            }
            BinomialHeap<Type, Comparator> *heap_head = new_heap->head;
            while (add_heap != NULL && heap != NULL){
                if (add_heap->degree < heap->degree){
                    new_heap->right = add_heap;
                    new_heap = add_heap;
                    add_heap = add_heap->right;
                }
                else{
                    new_heap->right = heap;
                    new_heap = heap;
                    heap = heap->right;
                }
                new_heap->head = heap_head;
            }
            if (add_heap == NULL){
                while (heap != NULL){
                    new_heap->right = heap;
                    new_heap = heap;
                    heap = heap->right;
                    new_heap->head = heap_head;
                }
            }
            else{
                while (add_heap != NULL){
                    new_heap->right = add_heap;
                    new_heap = add_heap;
                    add_heap = add_heap->right;
                    new_heap->head = heap_head;
                }
            }
            new_heap = new_heap->head;
            while (new_heap->right != NULL){
                if (new_heap->right->degree == new_heap->degree){
                    if (new_heap->right->right != NULL){
                        if (new_heap->right->right->degree != new_heap->degree){
                            BinomialHeap<Type, Comparator> *element = new_heap->right->right;
                            new_heap = mergeTrees(new_heap, new_heap->right);
                            element->head = new_heap->head = new_heap;
                            new_heap->right = element;
                        }
                        else{
                            new_heap = new_heap->right;
                        }
                    }
                }
                else{
                    new_heap = new_heap->right;
                }
            }
            value = new_heap->value;
            son = new_heap->son;
            head = new_heap->head;
            degree = new_heap->degree;
        }
    private:
        BinomialHeap<Type, Comparator>* mergeTrees(BinomialHeap<Type, Comparator> *left, BinomialHeap<Type, Comparator> *right){
            Comparator comp;
            if (!comp(left->value, right->value))
                swap(left, right);
            if (left->son != NULL)
                right->right = left->son;
            right->parent = left;
            left->son = right;
            right->head = right;
            left->degree++;
            return left;
        }
        Type value;
        BinomialHeap<Type, Comparator> *son, *right, *parent, *head;
        int degree;
};

#endif // BINOMIALHEAP_H
