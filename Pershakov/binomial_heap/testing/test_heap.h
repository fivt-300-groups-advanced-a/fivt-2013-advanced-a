#ifndef TEST_HEAP_H

#define TEST_HEAP_H

#include <gtest/gtest.h>

#include "../binomial_heap.h"

template<class T, class Comparator> class TestHeap : 
    public BinomialHeap<T, Comparator> {
    
    private:
        typedef Node<T> HeapNode;
        typedef std::list<HeapNode*> List;
        typedef typename List::const_iterator Iterator;
    
    public:
        bool checkInvariant(){
            if (!checkDegrees(this->roots))
                return false;
            for (Iterator it = this->roots.begin(); 
                    it != this->roots.end(); it++){
                if (!checkNode(*it))
                    return false;
            }
            for (Iterator it = this->roots.begin(); 
                    it != this->roots.end(); it++){
                if (!checkBinomialTree(*it, (*it)->degree))
                    return false;
            }
            return true;
        }

    private:
        bool checkDegrees(const List &to_check){
            if (to_check.empty())
                return true;
            Iterator it = to_check.cbegin(), next = to_check.cbegin();
            next++;
            while (next != to_check.end()){
                if ((*it)->degree >= (*next)->degree)
                    return false;
                it++;
                next++;
            }
            return true;
        }

        bool checkNode(HeapNode *node){
            if (!node)
                return false;
            if (!checkDegrees(node->child))
                return false;
            if (node->pred && node->pred->key > node->key)
                return false;
            if (node->id->node_ptr != node)
                return false;
            for (Iterator it = node->child.begin();
                    it != node->child.end(); it++)
                if (!checkNode(*it))
                    return false;
            return true;
        }

        bool checkBinomialTree(HeapNode *node, int order){
            if (order != node->degree)
                return false;
            int son_order = 0;
            for (Iterator it = node->child.begin(); 
                    it != node->child.end(); it++){
                if (!checkBinomialTree(*it, son_order))
                    return false;
                son_order++;
            }
            return true;
                
        }
};

#endif
