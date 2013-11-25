#include "BinomialHeap.h"
#include <cstdio>
#include <set>
#include <ctime>
#include "gtest/gtest.h"

#define forn(i, n) for (int i = 0; i < (int)n; i++)

struct triple
{
        int x;
        int y;
        int z;
};

struct tripleCmp
{
    bool operator ()(triple A, triple B)
    {
        if (A.x != B.x)
                return A.x < B.x;
        if (A.y != B.y)
                return A.y < B.y;
        return A.z < B.z;
    }
};

TEST(generate_big_heap, generate_big_heap)
{
	tripleCmp cmp;
	Heap <triple, tripleCmp> * H = new Heap <triple, tripleCmp > (cmp);
	HeapChecker <triple, tripleCmp> checker;
	int cur = 0;
	forn(it, 10000)
	{
		triple new_triple;
		new_triple.x = rand();
		new_triple.y = rand();
		new_triple.z = rand();
		H->insert(new_triple);
		ASSERT_TRUE(checker.checkDegreeInvariant(H, 17));
		if (it + 1 == (1 << cur))
		{
			ASSERT_TRUE(checker.checkPowerOfTwo(H, cur));
			cur++;
		}
	}
	triple prevMin;
	ASSERT_EQ(H->getSize(), 10000);
	forn(it, H->getSize())
	{
		triple newMin = H->extractMin();
		if (it != 0)
			ASSERT_FALSE(cmp(newMin, prevMin));
		prevMin = newMin;
		ASSERT_TRUE(checker.checkDegreeInvariant(H, 17));
	}
}


TEST(int_less_test, int_less_test)
{
	std::less <int> cmp;
	Heap <int, std::less <int>> * H = new Heap <int, std::less<int> > (cmp);
	
	HeapChecker <int, std::less<int>> checker;
	multiset <int> myset;
	//int start_time = clock();
	myset.clear();
	forn(j, 10000)
	{
		ASSERT_EQ(H->getSize(), myset.size());
		
		ASSERT_TRUE(checker.checkDegreeInvariant(H, 14));
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
		//typ = 2;
		if (typ == 0)
		{
			int our_ans = H->extractMin();
			int right_ans = *myset.begin();
			//printf("erase our_ans = %d right_ans = %d\n", our_ans, right_ans);
			ASSERT_TRUE(our_ans == right_ans);
			
			myset.erase(myset.begin());
		}
		else if (typ == 1)
		{
			int our_ans = H->getMin();
			int right_ans = *myset.begin();
			//printf("erase our_ans = %d right_ans = %d\n", our_ans, right_ans);
			ASSERT_TRUE(our_ans == right_ans);
			//printf("get our_ans = %d right_ans = %d\n", our_ans, right_ans);
			
		}
		else
		{
			//int r = rand() % 1000;
			//printf("add %d\n", r);
			int r = rand() % 1000;
			
			H->insert(r);
			myset.insert(r);
		}
	}
}

TEST(triple_test, triple_test)
{
	tripleCmp cmp;
	Heap <triple, tripleCmp> * H = new Heap <triple, tripleCmp > (cmp);
	
	HeapChecker <triple, tripleCmp> checker;
	//multiset <triple> myset(tripleCmp);
	multiset <triple, tripleCmp> myset;
	//int start_time = clock();
	myset.clear();
	forn(j, 10000)
	{
		ASSERT_EQ(H->getSize(), myset.size());
		ASSERT_TRUE(checker.checkDegreeInvariant(H, 14));
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
		//typ = 2;
		if (typ == 0)
		{
			triple our_ans = H->extractMin();
			triple right_ans = *myset.begin();
			//printf("erase our_ans = %d right_ans = %d\n", our_ans, right_ans);
			ASSERT_FALSE(cmp(our_ans, right_ans));
			ASSERT_FALSE(cmp(right_ans, our_ans));
			myset.erase(myset.begin());
		}
		else if (typ == 1)
		{
			triple our_ans = H->getMin();
			triple right_ans = *myset.begin();
			//printf("erase our_ans = %d right_ans = %d\n", our_ans, right_ans);
			ASSERT_FALSE(cmp(right_ans, our_ans));
			ASSERT_FALSE(cmp(our_ans, right_ans));
			//printf("get our_ans = %d right_ans = %d\n", our_ans, right_ans);
			
		}
		else
		{
			//int r = rand() % 1000;
			//printf("add %d\n", r);
			triple r;
			r.x = rand() % 1000;
			r.y = rand() % 1000;
			r.z = rand() % 1000;
			H->insert(r);
			myset.insert(r);
		}
	}
}


int main(int argc, char ** argv)
{
	testing::InitGoogleTest(&argc, argv); 
    RUN_ALL_TESTS(); 
	
}