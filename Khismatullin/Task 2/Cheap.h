#ifndef BINOMIAL_HEAP

#define BINOMIAL_HEAP

#include <algorithm>
#include <cassert>
#include <iostream>

template<class Type>
class Cvertex
{
	template<class T, class C> friend class Cheap;
	template<class T, class C> friend class Tester;
	public:
		explicit Cvertex(const Type &_value)
		{
			value = _value;
			parent = child = brother = NULL;
			degree = 0;
		}
		Cvertex()
		{
			parent = child = brother = NULL;
			degree = 0;
		}
		~Cvertex()
		{
		}

		void clear()
		{	}
	private:
		Type value;
		Cvertex *parent, *child, *brother;
		int degree;
};

template<class Type, class Comparator>
class Cheap
{
	template<class T, class C> friend class Tester;
	public:
		Cheap(): head(NULL), comp()
		{}

		explicit Cheap(const Comparator &cmp): head(NULL), comp(cmp)
		{}

		explicit Cheap(Cvertex<Type>* _head): head(_head), comp()
		{}

		explicit Cheap(const Type &value): head(new Cvertex<Type>(value)), comp()
		{}

		~Cheap()
		{
			clear(head);
			head = NULL;
		}
		
		void clear()
		{
			clear(head);
			head = NULL;
		}

		void merge(Cheap<Type, Comparator> &second)
		{
			if (head == NULL)
			{
				head = second.head;
				second.head = NULL;
				return;
			}
			if (second.head == NULL)
				return;
			Cvertex<Type> *temp = NULL, *i = head, *j = second.head, *first = NULL;
			while (i != NULL && j != NULL)
			{
				if (i -> degree <= j -> degree) {
					if (temp)
						temp -> brother = i;
					else
						first = i;
					temp = i;
					i = i -> brother;
				} else {
					if (temp)
						temp -> brother = j;
					else
						first = j;
					temp = j;
					j = j -> brother;
				}
			}
			if (i)
			{
				temp -> brother = i;
			} else {
				temp -> brother = j;
			}
			Cvertex<Type> *cur = first, *last = NULL;
			while (cur -> brother)
			{
				if (cur -> degree == cur -> brother -> degree){
					if (comp(cur -> value, cur -> brother -> value)) {
						Cvertex<Type> *tmp = cur -> brother -> brother;
						cur -> brother -> brother = cur -> child;
						cur -> brother -> parent = cur;
						cur -> child = cur -> brother;
						cur -> degree++;
						cur -> brother = tmp;
					} else {
						if (last)
							last -> brother = cur -> brother;
						Cvertex<Type> *tmp = cur -> brother;
						cur -> brother =  tmp -> child;
						cur -> parent = tmp;
						tmp -> child = cur;
						tmp -> degree++;
						cur = tmp;
					}
					continue;
				}
				if (last == NULL)
					head = cur;
				last = cur;
				cur = cur -> brother;
			}
			if (last == NULL)
				head = cur;
			second.head = NULL;
		}
		
		
		size_t insert(Type value)
		{
			Cheap<Type, Comparator> new_heap(value);
			merge(new_heap);
			return 0;
		}
		Type extract_min()
		{
			assert(head != NULL);
			Cvertex<Type> *answer = head, *last = head, *current_tree = head -> brother, *anslast = NULL;
			while (current_tree)
			{
				if (comp(current_tree -> value, answer -> value))
				{
					answer = current_tree;
					anslast = last;
				}
				last = current_tree;
				current_tree = current_tree -> brother;
			}
			if (anslast)
				anslast -> brother = answer -> brother;
			else
				head = answer -> brother;
			Cvertex<Type> *cur_child = answer -> child;
			while (cur_child)
			{
				cur_child -> parent =  NULL;
				cur_child = cur_child -> brother;
			}
			if (answer -> child)
			{
				Cheap<Type, Comparator> new_heap(answer -> child);
				answer -> child = NULL;
				merge(new_heap);
			}
		}
		Type get_min()
		{
			assert(head != NULL);
			Cvertex<Type> *answer = head, *current_tree = head -> brother;
			while (current_tree)
			{
				if (comp(current_tree -> value, answer -> value))
					answer = current_tree;
				current_tree = current_tree -> brother;
			}
			return answer -> value;
		}

	private:
		Cvertex<Type> *head;
		Comparator comp;
		
		void clear(Cvertex<Type>* v)
		{
			if (v != NULL)
				{
					clear(v -> brother);
					clear(v -> child);
					(*v).clear();
					
					delete v;
				}
		}

};


#endif
