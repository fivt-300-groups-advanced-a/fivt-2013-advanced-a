#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cassert>

#define pb push_back

using namespace std;


template <class T>
class Vertex
{
	
public:
	T key;
	Vertex * ancestor;
	Vertex * child;
	Vertex * next;
	int degree;

	Vertex(T key) : key(key), ancestor(NULL), child(NULL), next(NULL), degree(0) {}
};

template <class T>
class pointer
{
public:
	Vertex <T> * * link;

	pointer()
	{
		link = NULL;
	}
	/*pointer(Vertex <T> ** _link)
	{
		link = _link;
	}*/
	pointer(Vertex <T> * _link)
	{
		link = &_link;
	}
};


//template <class T>




template <class T, class Comparator>
class Heap
{
	//typedef Vertex <T> * * Pointer;
protected:
	vector <Vertex <T> *> list;
	Comparator cmp;
	public:
	Heap(Comparator _cmp)
	{
		list.clear();
		cmp = _cmp;
	}
	//explicit Heap(vector <Vertex <T> * > _list) : list(_list) {}

	Heap <T, Comparator> * innerMerge(Heap <T, Comparator> * secondHeap, bool flagInplace)
	{
		vector <Vertex <T> *  > new_list; 
		int curThis = 0;
		int curSecond = 0;
		while(curThis < list.size() || curSecond < secondHeap->list.size())
		{
			
			if (curSecond == secondHeap->list.size() || (curThis < list.size() && list[curThis]->degree < secondHeap->list[curSecond]->degree))
			{	
				new_list.pb(list[curThis]);
				curThis++;
			}
			else
			{
				new_list.pb(secondHeap->list[curSecond]);
				curSecond++;
				
			}
		}
		//int curResult = 0;
		//vector <Vertex <T> * > resultList;
		//list.clear();
		
		//while(curResult < new_list.size() - 1)
		Heap <T, Comparator> * resultHeap;
		if (flagInplace)
			list.clear();
		else resultHeap = new Heap <T, Comparator> (cmp);
		for (int curResult = 0; curResult < (int)new_list.size() - 1; curResult++)
		{
			if (new_list[curResult]->degree == new_list[curResult + 1]->degree)
			{
				Vertex <T> * A = new_list[curResult];
				Vertex <T> * B = new_list[curResult + 1];
				//Vertex <T> * tmp = curResult->next->next;
				if (!cmp(A->key, B->key))
				{
					swap(A, B);
				}	
				Vertex <T> * oldChild = A->child;
				A->child = B;
				B->ancestor = A;
				A->child->next = oldChild;
				new_list[curResult + 1] = A;
				A->degree++;
				if (curResult + 2 < new_list.size() && new_list[curResult + 2]->degree < new_list[curResult + 1]->degree)
					swap(new_list[curResult +1], new_list[curResult + 2]);
				
			}
			//else resultList.pb(new_list[curResult]);
			else if (flagInplace)
			{
				list.pb(new_list[curResult]);
			}
			else resultHeap->list.pb(new_list[curResult]);
		}
		if (!new_list.empty())
		{
			if (flagInplace)
			{
				list.pb(new_list.back());
			}
			else resultHeap->list.pb(new_list.back());
		}
		return (flagInplace ? NULL : resultHeap);
	}

	Heap <T, Comparator> * merge(Heap <T, Comparator> * secondHeap)
	{
		return innerMerge(secondHeap, false);
	}

	void inplaceMerge(Heap <T, Comparator> * secondHeap)
	{
		innerMerge(secondHeap, true);
	}

	//pointer <T> insert(T newElem)
	void insert(T newElem)
	{
		Heap <T, Comparator> * H2 = new Heap(cmp);
		Vertex <T> * v = new Vertex <T>(newElem);
		
		H2->list.pb(v);
		inplaceMerge(H2);
		//printf("v = %d &v = %d\n", v, &v);
		
		//pointer <T> ret = new pointer <T> (&v);
		//return pointer <T> (v);
	}
	T getMin()
	{
		if (list.empty())
			return T();
		int minIndex = 0;
		for (int j = 1; j < list.size(); j++)
			if (cmp(list[j]->key, list[minIndex]->key))
			{
				minIndex = j;
			}
		return list[minIndex]->key;
	}
	T extractMin()
	{
		if (list.empty())
			return T();
		int minIndex = 0;
		//T minElem = list[0]->key;
		for (int j = 1; j < list.size(); j++)
			//if (list[j]->key < minElem)
			if (cmp(list[j]->key, list[minIndex]->key))
			{
				//minElem = list[j]->key;
				minIndex = j;
			}
		Heap <T, Comparator> * splitted = new Heap <T, Comparator>(cmp);
		Vertex <T> * current = list[minIndex]->child;
		while(current != NULL)
		{
			splitted->list.pb(current);
			current->ancestor = NULL;
			current = current->next;
		}
		reverse(splitted->list.begin(), splitted->list.end());
		T minElem = list[minIndex]->key;
		list.erase(list.begin() + minIndex);
		inplaceMerge(splitted);
		//Heap <T, Comparator> * tmpHeap = merge(splitted);
		//list = tmpHeap->list;
		return minElem;
	}
	/*inline void deleteElem(Vertex <T> * v, T minusInf)
	{
		decreaseKey(v, minusInf);
		extractMin();
	}	*/
	
	int getSize()
	{
		int totalSize = 0;
		for(int j = 0; j < list.size(); j++)
			totalSize += (1 << list[j]->degree);
		return totalSize;
	}
	template <class T, class Comparator>
	friend class HeapChecker;

	/*void decreaseKey(pointer <T> &  v, T newValue)
	{
		Vertex <T> * p = *v.link;
		assert(!(p->key < newValue));
		p->key = newValue;
		while(p->ancestor != NULL)
		{
			if (p->key < p->ancestor->key)
			{
				swap(p->key, p->ancestor->key);
				v.link = &p->ancestor;
			}
			else break;
		}
	}*/

};

template <class T, class Comparator>
class HeapChecker
{
public:
	bool checkPowerOfTwo(Heap <T, Comparator> * H, int powerOfTwo)
	{
		return (H->list.size() == 1 && H->list[0]->degree == powerOfTwo);
	}
	bool checkDegreeInvariant(Heap <T, Comparator> * H, int normalDegree)
	{
		if (H->list.empty())
			return true;
		if (H->list[0]->degree > normalDegree)
			return false;
		for (int j = 1; j < H->list.size(); j++)
		{
			if (H->list[j]->degree <= H->list[j - 1]->degree || H->list[j]->degree > normalDegree)
				return false;
		}
		return true;
	}
};