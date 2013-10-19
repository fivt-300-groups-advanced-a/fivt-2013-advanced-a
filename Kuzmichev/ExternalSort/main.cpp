#include "BinaryReader.h"
#include "BinaryWriter.h"
#include "ExternalSort.h"
#include "Reader.h"
#include "Writer.h"

#include <iostream>

#include "gtest/gtest.h"

struct triple
{
	int x;
	int y;
	int z;
};

istream & operator >> (istream & in, triple & p)
{
	return in >> p.x >> p.y >> p.z;
}

ostream & operator << (ostream & out, triple & p)
{
	return out << p.x << " " << p.y << " " << p.z;
}

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

TEST(int_sort, int_sort)
{
	int n = 1e5;
	ofstream out("input_1_test.txt");
	for (int j = 1; j <= n; j++)
		out << rand() << " ";
	out.close();
	Reader r("input_1_test.txt");
	Writer w("output_1_test.txt");
	std::less <int> cmp;
	ExternalSorter <int, Reader, Writer, std::less <int> > es(r, w, cmp);
	es.externalSort();
	freopen("output_1_test.txt", "r", stdin);
	int first, second;
	ASSERT_EQ(scanf("%d", &first), 1) << "can't read first number" << endl;
	for (int j = 2; j <= n; j++)
	{
		ASSERT_EQ(scanf("%d", &second), 1) << "can't read " << j<< " number" << endl;
		ASSERT_LE(first, second) << "ASSERT_LE FAILED first =  " << first << " second = " << second << " j = " << j << endl;
		first = second;
	}
}

TEST(triple_sort, triple_sort)
{
	int n = 1e4;
	ofstream out("input_2_test.txt");
	for (int j = 1; j <= n; j++)
		out << rand() << " " << rand() << " " << rand() << " ";
	out.close();
	Reader r("input_2_test.txt");
	Writer w("output_2_test.txt");
	tripleCmp cmp;
	ExternalSorter <triple, Reader, Writer, tripleCmp > es(r, w, cmp);
	es.externalSort();
	freopen("output_2_test.txt", "r", stdin);
	triple A, B;
	ASSERT_EQ(scanf("%d%d%d", &A.x, &A.y, &A.z), 3) << "can't read first element" << endl;
	for (int j = 2; j <= n; j++)
	{
		ASSERT_EQ(scanf("%d%d%d", &B.x, &B.y, &B.z), 3) << "can't read " << j<< " element" << endl;
		ASSERT_TRUE(cmp(A, B)) << "ASSERT_LE FAILED " << " j = " << j << endl;
		A = B;
	}
}

int main(int argc, char ** argv)
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	std::getchar();
	
	//Writer w1 = w;
	//mycmp ttt;
	//ExternalSorter <int, Reader, Writer, mycmp> es(r, w, ttt);
	/*mycmp tt;
	printf("before creating es\n");
	ExternalSorter <int, Reader, Writer, mycmp > es(r, w, tt);
	printf("after creating es\n");
	es.externalSort();
	printf("after sort\n");*/
}