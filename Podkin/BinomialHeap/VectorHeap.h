#ifndef VECTORHEAP_H

#define VECTORHEAP_H

#include <vector>

using namespace std;

template <class Type, class Comparator>
class VectorHeap{
    public:
        VectorHeap(){
        }

        void clearHeap(){
            heap.clear();
        }

		void insert(Type value){
            heap.push_back(value);
        }

        Type getMin(){
            if (heap.size() == 0)
				return Type();
            int index = 0;
            for (int i = 0; i < heap.size(); i++)
                if (comp(heap[i], heap[index]))
                    index = i;
            return heap[index];
        }
        
        void mergeHeap(VectorHeap<Type, Comparator> *to_merge){
            for (int i = 0; i < to_merge->heap.size(); i++)
				heap.push_back(to_merge->heap.at(i));
		}

        void extractMin(){
            Type value = getMin();
            for (int i = 0; i < (int)heap.size(); i++){
                if (!comp(value, heap.at(i)) && !comp(heap.at(i), value)){
                    heap.erase(heap.begin() + i);
                    return;
                }
            }
        }
    private:
        Comparator comp;
        vector<Type> heap;
};

#endif
