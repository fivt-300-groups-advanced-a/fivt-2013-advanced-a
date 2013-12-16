#ifndef BINOMIAL_HEAP_H

#define BINOMIAL_HEAP_H

#include <cassert>
#include <list>
#include <iostream>
#include <algorithm>

template<class T> class NodeId;

template<class T> class Node { 

    template <class Type, class Comparator> friend class BinomialHeap;
    template <class Type, class Comparator> friend class TestHeap;
    template <class Type> friend class CmpNodesDegree;
    template <class Type> friend class NodeId;

    private:
        typedef std::list<Node<T>*> List;
        
        Node(){
            key = 0;
            pred = NULL;
            id = NULL;
            degree = 0;
        }

        explicit Node(const T &value){
            key = value;
            pred = NULL;
            id = new NodeId<T>(this);
            degree = 0;
        }

        ~Node(){
            delete id;
        }

        T key;
        Node *pred;
        List child;
        int degree;
        NodeId<T> *id;
};

template<class T> class NodeId {

    template <class Type, class Comparator> friend class BinomialHeap;
    template <class Type, class Comparator> friend class TestHeap;
    template <class Type> friend class Node;

    private:
        Node<T> *node_ptr;

        NodeId(){
            node_ptr = NULL;
        }

        explicit NodeId(Node<T> *ptr){
            node_ptr = ptr;
        }

        ~NodeId(){
        }
};

template<class T> struct CmpNodesDegree {
    bool operator() (Node<T> *a, Node<T> *b){
        return a->degree < b->degree;
    }
};

template<class T, class Comparator> class BinomialHeap {
    private:
        typedef Node<T> HeapNode;
        typedef std::list<HeapNode*> List;
        typedef typename List::iterator Iterator;
        typedef CmpNodesDegree<T> DegreeCmp;
        typedef NodeId<T>* NodeIdPtr;


    public:
        BinomialHeap(){
            sz = 0;
        }

        explicit BinomialHeap(const Comparator &new_cmp){
            cmp = new_cmp;
            sz = 0;
        }

        explicit BinomialHeap(const T &value){
            roots.push_back(new HeapNode(value));
            sz = 1;
        }

        explicit BinomialHeap(const HeapNode &node){
            roots = node.child;
            for (Iterator it = roots.begin(); it != roots.end(); it++)
                (*it)->pred = NULL;
            sz = 0;
        }

        ~BinomialHeap(){
            clear();
        }

        void clear(){
            for (Iterator it = roots.begin(); it != roots.end(); it++)
                destructorDfs(*it);
            roots.clear();
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
            res_list.merge(oth.roots, DegreeCmp());

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
                        (*it)->child.push_back(*next);
                        (*next)->pred = *it;
                        (*it)->degree++;
                        next = roots.erase(next);
                        after_next = next;
                        after_next++;
                    } else {
                        (*next)->child.push_back(*it);
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

        NodeIdPtr insert(const T &value){
            BinomialHeap<T, Comparator> new_heap(value);
            NodeIdPtr ans = (*new_heap.roots.begin())->id;
            merge(new_heap);
            return ans;
        }

        std::pair<T, NodeIdPtr> extractMin(){
            int old_sz = sz;
            old_sz--;
            Iterator min_it = findMin();
            T min_val = (*min_it)->key;
            BinomialHeap<T, Comparator> new_heap(**min_it);
            NodeIdPtr id = (*min_it)->id;
            delete *min_it;
            roots.erase(min_it);
            merge(new_heap);
            sz = old_sz;
            return std::make_pair(min_val, id);
        }

        void decreaseKey(NodeIdPtr id, const T &new_val){
            assert(id);
            assert(id->node_ptr);
            assert(!cmp(id->node_ptr->key, new_val));
            HeapNode *cur_node = id->node_ptr;
            cur_node->key = new_val;
            while (cur_node->pred){
                if (cmp(new_val, cur_node->pred->key)){
                    swap(cur_node, cur_node->pred);
                    cur_node = cur_node->pred;
                } else 
                    break;
            }
        }

        void erase(NodeIdPtr id){
            assert(id);
            assert(id->node_ptr);
            HeapNode *cur_node = id->node_ptr;
            while (cur_node->pred){
                swap(cur_node, cur_node->pred);
                cur_node = cur_node->pred;
            }
            int old_sz = sz - 1;
            BinomialHeap<T, Comparator> new_heap(*cur_node);
            for (Iterator it = roots.begin(); it != roots.end(); it++)
                if (*it == cur_node){
                    roots.erase(it);
                    break;
                }
            delete cur_node;
            merge(new_heap);
            sz = old_sz;
        }

    protected:
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

        void swap(HeapNode *a, HeapNode *b){ 
            std::swap(a->key, b->key);
            NodeIdPtr buf = a->id;
            a->id = b->id;
            b->id = buf;
            a->id->node_ptr = a;
            b->id->node_ptr = b;
        }

        void destructorDfs(HeapNode *node){
            for (Iterator it = node->child.begin(); 
                    it != node->child.end(); it++)
                destructorDfs(*it);
            delete node;
        }
};

#endif
