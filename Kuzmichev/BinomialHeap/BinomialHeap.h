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
void decreaseKey(Vertex <T> * v, T newValue)
{
	assert(!(v->key < newValue));
	v->key = newValue;
	while(v->ancestor != NULL)
	{
		if (v->key < v->ancestor->key)
		{
			swap(v->key, v->ancestor->key);
			v = v->ancestor;
		}
		else break;
	}
}



template <class T>
class Heap
{
public:
	vector <Vertex <T> *> list;
	Heap()
	{
		list.clear();
	}
	explicit Heap(vector <Vertex <T> * > _list) : list(_list) {}
	Heap * merge(Heap * secondHeap)
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
		int curResult = 0;
		vector <Vertex <T> * > resultList;
		//list.clear();
		
		//while(curResult < new_list.size() - 1)
		for (int curResult = 0; curResult < (int)new_list.size() - 1; curResult++)
		{
			if (new_list[curResult]->degree == new_list[curResult + 1]->degree)
			{
				Vertex <T> * A = new_list[curResult];
				Vertex <T> * B = new_list[curResult + 1];
				//Vertex <T> * tmp = curResult->next->next;
				if (A->key >= B->key)
				{
					swap(A, B);
				}	
				Vertex <T> * oldChild = A->child;
				A->child = B;
				B->ancestor = A;
				A->child->next = oldChild;
				new_list[curResult + 1] = A;
				A->degree++;
			}
			else resultList.pb(new_list[curResult]);
		}
		if (!new_list.empty())
			resultList.pb(new_list.back());
		return new Heap(resultList);
		
	}
	Vertex <T> * insert(T newElem)
	//void insert(T newElem)
	{
		Heap <T> * H2 = new Heap();
		Vertex <T> * v = new Vertex <T>(newElem);
		H2->list.pb(v);
		Heap <T>  * mergedHeap = merge(H2);
		list = mergedHeap->list;
		return v;
	}
	T getMin()
	{
		if (list.empty())
			return T();
		int minIndex = 0;
		for (int j = 1; j < list.size(); j++)
			if (list[j]->key < list[minIndex]->key)
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
			if (list[j]->key < list[minIndex]->key)
			{
				//minElem = list[j]->key;
				minIndex = j;
			}
		Heap <T> * splitted = new Heap <T>();
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
		Heap <T> * tmpHeap = merge(splitted);
		list = tmpHeap->list;
		return minElem;
	}
	inline void deleteElem(Vertex <T> * v, T minusInf)
	{
		decreaseKey(v, minusInf);
		extractMin();
	}	
};