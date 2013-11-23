#include "BinomialHeap.h"
#include <cstdio>
#include <cassert>
#include <set>
#include <ctime>

typedef Vertex <int> * pointer;
#define forn(i, n) for (int i = 0; i < (int)n; i++)

void add(Heap <int> * H, int x)
{
	H->insert(x);
	printf("%d\n", H->getMin());
	fflush(stdout);
}

void minn(Heap <int> * H)
{
	printf("%d\n", H->getMin());
}

void init(Heap <int> * H)
{
	int minn = 100500;
	forn(j, 10)
	{
		int new_e = rand() % 10 + 10 - j;
		
		H->insert(new_e);
		minn = min(minn, new_e);
		int h_min = H->getMin();
		assert(h_min == minn);
		printf("new_e = %d minn = %d h_min = %d\n", new_e, minn, h_min);
	}
}

multiset <int> myset;

void test(Heap <int> * H)
{
	int start_time = clock();
	myset.clear();
	forn(j, 1000000)
	{
		//printf("iteration %d\n", j);
		int typ;
		while(true)
		{
			typ = rand() % 3;
			if (typ == 0 || typ == 1)
			{
				if (myset.empty())
					continue;
			}
			break;
		}
		if (typ == 0)
		{
			int our_ans = H->extractMin();
			int right_ans = *myset.begin();
			//printf("erase our_ans = %d right_ans = %d\n", our_ans, right_ans);
			assert(our_ans == right_ans);
			myset.erase(myset.begin());
		}
		else if (typ == 1)
		{
			int our_ans = H->getMin();
			int right_ans = *myset.begin();
			//printf("get our_ans = %d right_ans = %d\n", our_ans, right_ans);
			assert(our_ans == right_ans);
		}
		else
		{
			int r = rand() % 1000;
			//printf("add %d\n", r);
			H->insert(r);
			myset.insert(r);
		}
	}
	printf("time %d\n", clock() - start_time);
}

int main()
{
	//freopen("output.txt", "w", stdout);
	/*Heap <int> * H1 = new Heap<int>();
	minn(H1);
	Heap <int> * H2 = new Heap<int>();
	//init(H1);
	//init(H2);
	/*add(H1, 5);
	add(H1, 2);
	add(H1, 3);
	add(H2, 10);
	add(H2, 15);
	add(H2, 100);*/

	//Heap <int> * H3 = H1->merge(H2);
	//minn(H3);
	//Heap <int> * H4 = new Heap<int> ();
	//test(H4);
	Heap <int> * H5 = new Heap<int>();
	pointer p1 = H5->insert(10);
	pointer p2 = H5->insert(10);
	pointer p3 = H5->insert(10);
	minn(H5);
	pointer p4 = H5->insert(5);
	minn(H5);
	decreaseKey(p1, 4);
	minn(H5);
	H5->deleteElem(p1, -100500);
	minn(H5);
}