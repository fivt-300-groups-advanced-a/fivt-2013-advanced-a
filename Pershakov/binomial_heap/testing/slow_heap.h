#ifndef SLOW_HEAP_H

#define SLOW_HEAP_H

#include <vector>

template <class T, class Comparator> class SlowHeap{
    public:
        void clear(){
            heap.clear();
        }

        int size(){
            return heap.size();
        }

        bool empty(){
            return size() == 0;
        }

        T getMin(){
            assert(size() != 0);
            T ans = heap[0];
            for (int i = 0; i < size(); i++)
                if (cmp(heap[i], ans))
                    ans = heap[i];
            return ans;
        }
        
        void merge(const SlowHeap &to_merge){
            std::vector<T> oth = to_merge.getHeap();
            heap.insert(heap.end(), oth.begin(), oth.end());
        }

        void insert(const T &value){
            heap.push_back(value);
        }

        T extractMin(){
            T value = getMin();
            for (int i = 0; i < (int)heap.size(); i++)
                if (isEqual(value, heap[i])){
                    for (int j = i; j + 1 < (int)heap.size(); j++)
                        heap[j] = heap[j + 1];
                    heap.pop_back();
                    return value;
                }
            assert(0);
        }
        
        void decreaseKey(int old_val, int new_val){
            assert(new_val <= old_val);
            for (int i = 0; i < (int)heap.size(); i++)
                if (isEqual(old_val, heap[i])){
                    heap[i] = new_val;
                    return;
                }
        }

    private:
        Comparator cmp;
        std::vector<T> heap;

        bool isEqual(const T &a, const T &b){
            return !cmp(a, b) && !cmp(b, a);
        }

        std::vector<T> getHeap() const{
            return heap;
        }

};

#endif
