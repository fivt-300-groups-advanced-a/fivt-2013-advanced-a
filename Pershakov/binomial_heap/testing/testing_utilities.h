#ifndef TESTING_UTILITIES_H

#define TESTING_UTILITIES_H

#include <cstdlib>
#include <unordered_map>

#include "../binomial_heap.h"
#include "slow_heap.h"
#include "point.h"

template <class T, class Comparator> 
void eraseFromVector(std::vector<T> &vect, const T &val, 
        Comparator cmp = Comparator()){
    for (int i = 0; i < (int)vect.size(); i++)
        if (!cmp(val, vect[i]) && !cmp(vect[i], val)){
            vect.erase(vect.begin() + i);
            return;
        }
}

template <class T, class Comparator, class RandomGenerator, 
         class Hash, class KeyEqual> 
class Tester {
    private:
        typedef BinomialHeap<T, Comparator> Heap;
        typedef SlowHeap<T, Comparator> Slow;
        typedef NodeId<T>* NodeIdPtr;
        typedef std::unordered_map<T, std::vector<NodeIdPtr>, 
                Hash, KeyEqual> MapId;
        
        MapId ids;
        std::vector<T> values;
        Comparator cmp;
        RandomGenerator rnd;

        bool isEqual(const T &a, const T &b){
            return !cmp(a, b) && !cmp(b, a);
        }

    public:
        Tester(){
            srand(time(0));
        }

        Tester(const Comparator &new_cmp){
            srand(time(0));
            cmp = new_cmp;
        }

        Tester(const RandomGenerator &new_rnd){
            srand(time(0));
            rnd = new_rnd;
        }

        Tester(const Comparator &new_cmp, const RandomGenerator &new_rnd){
            srand(time(0));
            cmp = new_cmp;
            rnd = new_rnd;
        }

        void check(Heap &heap, Slow &slow_heap){
            ASSERT_EQ(heap.size(), slow_heap.size());
            ASSERT_EQ(heap.empty(), slow_heap.empty());
            if (!heap.empty())
                ASSERT_TRUE(isEqual(heap.getMin(), slow_heap.getMin()));
        }

        void insert(Heap &heap, Slow &slow_heap){
            int cnt = rand() % 1000;
            for (int i = 0; i < cnt; i++){
                T val = rnd();
                NodeIdPtr id = heap.insert(val);
                slow_heap.insert(val);
                //ids[val].push_back(id);
                values.push_back(val);
                check(heap, slow_heap);
            }
        }

        void extractMin(Heap &heap, Slow &slow_heap){
            //int cnt = rand();
            //for (int i = 0; i < cnt; i++){
                //check(heap, slow_heap);
                //if (slow_heap.empty())
                    //break;
                //std::pair<T, NodeIdPtr> extracted = heap.extractMin();
                //T slow_extracted = slow_heap.extractMin();
                //ASSERT_TRUE(isEqual(extracted.first, slow_extracted));
                //eraseFromVector<T, Comparator>(values, extracted.first, cmp);
                //typename MapId::iterator it = ids.find(extracted.first);
                //eraseFromVector<NodeIdPtr, std::less<NodeIdPtr> >
                    //(it->second, extracted.second);
            //}
            //check(heap, slow_heap);
        }

        void merge(Heap &heap, Slow &slow_heap){
            //Heap new_heap;
            //Slow new_slow_heap;
            //insert(new_heap, new_slow_heap);
            //heap.merge(new_heap);
            //slow_heap.merge(new_slow_heap);
            //check(heap, slow_heap);
        }

        void decreaseKey(Heap &heap, Slow &slow_heap){
            //int cnt = rand() % 10000;
            //if (heap.empty())
                //return;
            //for (int i = 0; i < cnt; i++){
                //int pos = rand() % values.size();
                //T old_val = values[pos];
                //T new_val = rnd.decrease(old_val);
                //values[pos] = new_val;
                //slow_heap.decreaseKey(old_val, new_val);
                //NodeIdPtr id = *ids[old_val].begin();
                //ids[old_val].erase(ids[old_val].begin());
                //ids[new_val].push_back(id);
                //heap.decreaseKey(id, new_val);
                //check(heap, slow_heap);
            //}
        }

        void erase(Heap &heap, Slow &slow_heap){
            //int cnt = rand() % 1000;
            //check(heap, slow_heap);
            //for (int i = 0; i < cnt; i++){
                //if (heap.empty())
                    //break;
                //int pos = rand() % values.size();
                //T val = values[pos];
                //values.erase(values.begin() + pos);
                //slow_heap.erase(val);
                //heap.erase(*ids[val].begin());
                //ids[val].erase(ids[val].begin());
                //check(heap, slow_heap);
            //}
        }
};

#endif
