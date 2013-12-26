#ifndef BINOMIAL_HEAP

#define BINOMIAL_HEAP

#include <algorithm>
#include <cassert>
#include <iostream>

template<class Type>
class Cindex;

// осталось сделать decrease_key
template<class Type>
class Cvertex
{
	template<class T, class C> friend class Cheap;
	template<class T> friend class Cindex;
	template<class T, class C> friend class Tester;
	public:
		explicit Cvertex(const Type &_value)
		{
			value = _value;
			parent = child = brother = prev = NULL;		
			idx = new Cindex<Type>(this);
			degree = 0;
		}
		Cvertex()
		{
			parent = child = brother = prev = NULL;
			idx = new Cindex<Type>(this);
			degree = 0;
		}
		~Cvertex()
		{
			//delete idx;
		}

		void clear()
		{
		}
		
		
	private:
		Type value;
		Cvertex *parent, *child, *brother, *prev;
		Cindex<Type> *idx;
		int degree;
};

template<class Type>
class Cindex
{
	template<class T, class C> friend class Cheap;
	template<class T> friend class Cvertex;
	template<class T, class C> friend class Tester;
	public:
		Cindex()
		{
			ptr = NULL;
		}
		explicit Cindex(Cvertex<Type> *_ptr)
		{
			ptr = _ptr;
		}
		~Cindex()
		{
		}
		
		Type get_value()
		{
			return ptr -> value;
		}
		
	private:
		Cvertex<Type> *ptr;
};

template<class Type, class Comparator>
class Cheap
{
	template<class T, class C> friend class Tester;
	typedef Cvertex<Type>* iterator;
	public:
		typedef Cindex<Type>* index;
		Cheap(): head(NULL), comp()
		{}

		explicit Cheap(const Comparator &cmp): head(NULL), comp(cmp)
		{}

		explicit Cheap(iterator _head): head(_head), comp()
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
			iterator temp = NULL, i = head, j = second.head, first = NULL;
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
			iterator cur = first, last = NULL;
			while (cur -> brother)
			{
				if (cur -> degree == cur -> brother -> degree && (cur -> brother -> brother == NULL || cur -> brother -> brother -> degree > cur -> degree)){
					if (comp(cur -> value, cur -> brother -> value)) {
						if (last) 
							last -> brother = cur;
						iterator tmp = cur -> brother -> brother;
						cur -> brother -> brother = cur -> child;
						cur -> brother -> parent = cur;
						if (cur -> child)
						{
							cur -> child -> prev = cur -> brother;
						}
						cur -> child = cur -> brother;
						cur -> degree++;
						cur -> brother = tmp;
					} else {
						if (last)
							last -> brother = cur -> brother;
						iterator tmp = cur -> brother;
						cur -> brother =  tmp -> child;
						cur -> parent = tmp;
						if (tmp -> child)
						{
							tmp -> child -> prev = cur;
						}
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
		
		index insert(Type value)
		{
			Cheap<Type, Comparator> new_heap(value);
			iterator tmp = new_heap.head;
			merge(new_heap);
			return tmp -> idx;
		}
		index extract_min()
		{
			assert(head != NULL);
			iterator answer = head, last = head, current_tree = head -> brother, anslast = NULL;
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
			if (answer -> child)
			{	
				Cheap<Type, Comparator> new_heap(inverse(answer -> child, NULL));
				merge(new_heap);
			}
			iterator tmp = answer;
			delete answer;
			return tmp -> idx;
		}
		Type get_min()
		{
			assert(head != NULL);
			iterator answer = head, current_tree = head -> brother;
			while (current_tree)
			{
				if (comp(current_tree -> value, answer -> value))
					answer = current_tree;
				current_tree = current_tree -> brother;
			}
			return answer -> value;
		}
		
		bool decrease_key(index v, Type new_value) 
		{
			if (comp(v -> ptr -> value, new_value)) {
				return false;
			}
			while (v -> ptr -> parent != NULL && comp(new_value, v -> ptr -> parent -> value))
			{
				iterator tmp = v -> ptr;
				v -> ptr = tmp -> parent;
				tmp -> idx = tmp -> parent -> idx;
				tmp -> parent -> idx -> ptr = tmp;
				tmp -> parent -> idx  = v;
				Type c = tmp -> value;
				tmp -> value = tmp -> parent -> value;
				tmp -> parent -> value = c;
			}
			v -> ptr -> value = new_value;
		}
		
		void erase(index v, Type inf) 
		{
			decrease_key(v, inf);
			extract_min();
		}

	private:
		iterator head;
		Comparator comp;
		
		iterator inverse(iterator v, iterator p)
		{
			v -> parent = NULL;
			if (v -> brother == NULL) {
				v -> brother = p;
				return v;
			}
			iterator ans = inverse(v -> brother, v);
			v -> brother = p;
			return ans;
		}
		
		void clear(iterator v)
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
