#ifndef BINOMIALHEAP_H
#define BINOMIALHEAP_H

#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;

template <class T, class Comp>
class BinomialTree {
	template <class Type, class Comparator> friend class BinomialHeap;
public:
	explicit BinomialTree(const T &element){
		value = element;
		sons.clear();
		degree = 0;
	}
	~BinomialTree(){
	}
private:
	T value;
	vector < BinomialTree <T, Comp> *> sons; 
	int degree;
};

template<class Type, class Comparator>
class BinomialHeap {
public:
	BinomialHeap(){
		clearHeap();
	}
	explicit BinomialHeap(const Type &element){
		clearHeap();
		heap.push_back(new BinomialTree<Type, Comparator>(element));
	}
	explicit BinomialHeap(const vector <BinomialTree <Type, Comparator> *> &new_heap){
		clearHeap();
		for (int i = 0; i < (int)new_heap.size(); i++)
			heap.push_back(new_heap.at(i));
	}
	~BinomialHeap(){
		clearHeap();
	}
	void clearHeap(){
		for (int i = 0; i < (int)heap.size(); i++){
			BinomialTree<Type, Comparator> *element = heap.at(i);
			recursionDestructor(element);
		}
		heap.clear();
	}
	void insert(const Type &value){
		BinomialHeap<Type, Comparator> add_element(value);
		mergeHeap(add_element);
	}
	Type getMin(){
		if (heap.size() == 0)
			return Type();
		int index = 0;
		for (int i = 0; i < (int)heap.size(); i++){
			if (comp(heap[index]->value, heap[i]->value))
				index = i;
		}
		return heap[index]->value;
	}
	void extractMin(){
		Type min_element = getMin();
		vector < BinomialTree <Type, Comparator > *> result_heap;
		vector < BinomialTree <Type, Comparator > *> new_heap;
		for (int i = 0; i < (int)heap.size(); i++){
			if (comp(min_element, heap.at(i)->value) || comp(heap.at(i)->value, min_element))
				result_heap.push_back(heap.at(i));
			if (!comp(min_element, heap.at(i)->value) && !comp(heap.at(i)->value, min_element))
				new_heap = heap.at(i)->sons;
		}
		heap = result_heap;
		BinomialHeap<Type, Comparator> add_heap(new_heap);
		mergeHeap(add_heap);
	}
	void mergeHeap(BinomialHeap<Type, Comparator> &add_heap){
		vector < BinomialTree <Type, Comparator > *> new_heap;
		int first_index = 0, second_index = 0;
		while (first_index < heap.size() && second_index < add_heap.heap.size()){
			if (heap.at(first_index)->degree < add_heap.heap.at(second_index)->degree){
				new_heap.push_back(heap.at(first_index));
				first_index++;
			}
			else{
				new_heap.push_back(add_heap.heap.at(second_index));
				second_index++;
			}
		}
		if (first_index == heap.size()){
			while (second_index < add_heap.heap.size()){
				new_heap.push_back(add_heap.heap.at(second_index));
				second_index++;
			}
		}
		else{
			while(first_index < heap.size()){
				new_heap.push_back(heap.at(first_index));
				first_index++;
			}
		}
		add_heap.heap.resize(0);
		vector < BinomialTree <Type, Comparator > *> result_heap;
		for (int i = 0; i < (int)new_heap.size() - 2; i++){
			if (new_heap.at(i)->degree == new_heap.at(i + 1)->degree && new_heap.at(i)->degree == new_heap.at(i + 2)->degree){
				result_heap.push_back(new_heap.at(i));
				new_heap.at(i + 2) = mergeTrees(new_heap.at(i + 1), new_heap.at(i + 2));
				i++;
			}
			else{
				if (new_heap.at(i)->degree == new_heap.at(i + 1)->degree){
					new_heap.at(i + 1) = mergeTrees(new_heap.at(i), new_heap.at(i + 1));
					continue;
				}
				result_heap.push_back(new_heap.at(i));
			}
		}
		if (!new_heap.empty()){
			if (new_heap.size() == 1){
				result_heap.push_back(new_heap.at(0));
			}
			else{
				if (new_heap.at(new_heap.size() - 1)->degree == new_heap.at(new_heap.size() - 2)->degree)
					result_heap.push_back(mergeTrees(new_heap.at(new_heap.size() - 1), new_heap.at(new_heap.size() - 2)));
				else{
					result_heap.push_back(new_heap.at(new_heap.size() - 2));
					result_heap.push_back(new_heap.at(new_heap.size() - 1));
				}
			}
		}
		heap.resize(0);
		for (int i = 0; i < (int)result_heap.size(); i++)
			heap.push_back(result_heap.at(i));
	}
	bool checkHeap(){
		for (int i = 0; i < (int)heap.size() - 1; i++){
			if (heap.at(i)->degree >= heap.at(i + 1)->degree)
				return false;
			int n = heap.at(i)->sons.size();
			for (int j = 0; j < n; j++){
				if (comp(heap.at(i)->value, heap.at(i)->sons.at(j)->value))
					return false;
				if (!checkTree(heap.at(i)->sons.at(j)))
					return false;
			}
		}
		return true;
	}
	bool checkTree(BinomialTree<Type, Comparator> *tr){
		if (tr == NULL)
			return false;
		if (tr->sons.size() == 0)
			return true;
		for (int i = 0; i < (int)tr->sons.size(); i++){
			if (!checkTree(tr->sons.at(i)) && comp(tr->sons.at(i)->value, tr->value))
				return false;
		}
		return true;
	}
private:
	Comparator comp;
	BinomialTree<Type, Comparator>* mergeTrees(BinomialTree<Type, Comparator> *left, BinomialTree<Type, Comparator> *right){
		Comparator comp;
		if (comp(right->value, left->value))
			swap(left, right);
		left->sons.push_back(right);
		left->degree++;
		return left;
	}
	void recursionDestructor(BinomialTree <Type, Comparator> *tr){
		if (tr != NULL){
			for (int i = 0; i < (int)tr->sons.size(); i++){
				recursionDestructor(tr->sons.at(i));
			}
			delete tr;
		}	
	}
	vector < BinomialTree <Type, Comparator> *> heap;
};

#endif
