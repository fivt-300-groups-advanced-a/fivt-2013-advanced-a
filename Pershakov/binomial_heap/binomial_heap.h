#ifndef BINOMIAL_HEAP_H

#define BINOMIAL_HEAP_H

#include <cassert>
#include <list>

template<class T> struct Node { 
    Node(){
        key = 0;
        pred = NULL;
    }

    explicit Node(T value){
        key = value;
        pred = NULL;
    }
    
    T key;
    Node *pred;
    std::list<Node<T>*> child;
    int degree;
};

template<class T> struct cmpNodes {
    bool operator() (Node<T> *a, Node<T> *b){
        return a->degree < b->degree;
    }
};

template<class T, class Comparator> class BinomialHeap {
    public:
        explicit BinomialHeap(T value){
            roots.push_back(new Node<T>(value));
            sz = 1;
        }

        explicit BinomialHeap(const Node<T> &node){
            roots = node.child;
            roots.reverse();
            for (Iterator it = roots.begin(); it != roots.end(); it++)
                (*it)->pred = NULL;
            sz = 0;
        }

        BinomialHeap(){
            sz = 0;
        }

        bool empty(){
            return sz == 0;
        }
        
        int size(){
            return sz;
        }

        T getMin(){
            Iterator min_it = findMin();
            return (*min_it)->key;
        }


        void merge(BinomialHeap<T, Comparator> &oth){
            sz += oth.size();
            List res_list = roots;
            res_list.merge(oth.roots, cmpNodes<T>());

            Iterator it = res_list.begin();
            Iterator next = it;
            next++;
            Iterator after_next = next;
            after_next++;
            while (next != res_list.end()){
                if ((*it)->degree == (*next)->degree && 
                        (after_next == res_list.end() || 
                         (*after_next)->degree != (*next)->degree)){
                    if (cmp((*it)->key, (*next)->key)){
                        (*it)->child.push_front(*next);
                        (*next)->pred = *it;
                        (*it)->degree++;
                        next = roots.erase(next);
                        after_next = next;
                        after_next++;
                    } else {
                        (*next)->child.push_front(*it);
                        (*next)->degree++;
                        (*it)->pred = *next;
                        it = roots.erase(it);
                        next = it;
                        next++;
                        after_next = next;
                        after_next++;;
                    }
                } else {
                    it++;
                    next++;
                    after_next++;
                }
            }
            roots = res_list;
        }

        void insert(T value){
            BinomialHeap<T, Comparator> new_heap(value);
            merge(new_heap);
        }

        T extractMin(){
            int old_sz = sz;
            old_sz--;
            Iterator min_it = findMin();
            T min_val = (*min_it)->key;
            BinomialHeap<T, Comparator> new_heap(**min_it);
            roots.erase(min_it);
            merge(new_heap);
            sz = old_sz;
            return min_val;
        }

        void decreaseKey(Node<T> *node, int new_val){
            assert(node);
            assert(new_val <= node->key);
            node->key = new_val;
            Node<T> *cur_node = node;
            while (cur_node->pred){
                if (cur_node->pred->key > new_val){
                    swap(cur_node->key, cur_node->pred->key);
                    cur_node = cur_node->pred;
                } else 
                    break;
            }
        }

    private:
        typedef typename std::list<Node<T>*>::iterator Iterator;
        typedef std::list<Node<T>*> List;

        Comparator cmp;
        List roots;
        int sz;

        Iterator findMin(){
            assert(sz != 0);
            Iterator cur_min = roots.begin();
            for (Iterator it = roots.begin(); it != roots.end(); it++)
                if (cmp((*it)->key, (*cur_min)->key))
                    cur_min = it;
            return cur_min;
        }
};

#endif
