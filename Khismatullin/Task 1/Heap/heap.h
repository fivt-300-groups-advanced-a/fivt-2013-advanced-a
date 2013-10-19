#ifndef HEAP

#define HEAP

#include <vector>
#include <iostream>
#include <cassert>

template<class T, class comp>
class heap
{
	public:
		~heap(){
			delete[] h;
		}
		heap(unsigned int n = 0)
		{
			h = new T[n];
			size = 0;
		}
		T Extract_min()
		{
			assert(size > 0);
			T res = h[0];
			h[0] = h[--size];
			SiftDown(0);
			return res;
		}
		
		void add(T x) {
			h[size] = x;
			SiftUp(size++);
		}
		
	private:
		T* h; // заменить на массивы.
		unsigned int size; 
		comp cmp;
		
		void SiftDown(unsigned int pos) // заменить на перестановку?
		{
			unsigned int maxCH = pos;
			if (pos * 2 + 1 < size && cmp(h[pos * 2 + 1], h[pos])) {
				maxCH = 2 * pos + 1;
			}
			if (pos * 2 + 2 < size && cmp(h[pos * 2 + 2], h[maxCH])) {
				maxCH = 2 * pos + 2;
			}
			if (maxCH != pos) {
				swap(h[maxCH], h[pos]);
				SiftDown(maxCH);
			}
		}
		
		void SiftUp(unsigned int pos) // заменить на перестановку?
		{
			if (!pos) 
				return;
			if (cmp(h[pos], h[(pos - 1) >> 1])) {
				swap(h[pos], h[(pos - 1) >> 1]);
				SiftUp((pos - 1) >> 1);
			}
		}
		
};

#endif