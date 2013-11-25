#include "gtest/gtest.h"
#include "Cheap.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

using namespace std;

template<class Type, class Comparator> class Tester
{
	public:
		Tester(Cheap<Type, Comparator>* h): heap(h) {}
		bool check_heap(int size)
		{
			Cvertex<Type> *cur = heap -> head;
			if (cur == NULL) {
				return (size == 0);
			}
			while (cur)
			{
				if (!check_tree(cur)) { // проверка дерева
					return false;
				}
				size -= (1 << (cur -> degree));
				if ((cur -> brother != NULL) && (cur -> brother -> degree <= cur -> degree)) { // проверка на повторяемость степени дерева, порядок следования
					return false;
				}
				cur = cur -> brother;
			}
			return (size == 0); // проверка на сумму степеней дерева и общее количество элементов
		}
		bool check_tree(Cvertex<Type>* v)
		{
			Cvertex<Type> *cur = v -> child;
			if (v -> degree == 0) {
				return (cur == NULL);
			}
			if ((cur != NULL) && cur -> degree != v -> degree - 1) {
				return false;
			}
			while (cur)
			{
				if ((cur -> brother != NULL) && (cur -> brother -> degree + 1 != cur -> degree)) { // порядок следования сыновей
					return false;
				}
				if (!check_tree(cur)) { 
					return false;
				}
				if (cur -> brother == NULL && cur -> degree != 0) {
					return false;
				}
				cur = cur -> brother;
			}
			return true;
		}
		
	private:
		Cheap<Type, Comparator>* heap;
};

TEST(Invariant, INT)
{
	int testcases = 100;
	for(int test = 0; test < testcases; test++)
	{
		Cheap<int, less<int> > *heap = new Cheap<int, less<int> >;
		Tester<int, less<int> > tester(heap);
		int n = rand() % 10000 + 100;
		for(int i = 0; i < n; i++)
		{
			heap -> insert(1);
		}
		ASSERT_TRUE(tester.check_heap(n));
		delete heap;
	}
}

int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}