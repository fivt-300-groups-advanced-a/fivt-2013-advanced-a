#include <string>
#include <fstream>
#include "gtest/gtest.h"
#include "heapsort.h"

const int MAX_N = 100500;

TEST(heapsort, simple)
{
	int a[4] = {0, 4, 2, 3};
	heapsort(a, a + 4, (std::less<int>()));
	EXPECT_EQ(a[0],0);
	EXPECT_EQ(a[1],2);
	EXPECT_EQ(a[2],3);
	EXPECT_EQ(a[3],4);
}

TEST(heapsort, string)
{
	std::string strs[4] = {"aba", "caba", "daba", "caba"};
	heapsort(strs, strs + 4, (std::less<std::string>()));
	EXPECT_EQ(strs[0],"aba");
	EXPECT_EQ(strs[1],"caba");
	EXPECT_EQ(strs[2],"caba");
	EXPECT_EQ(strs[3],"daba");
}

TEST(heapsort, informatics766test1)
{
	std::ifstream in("sort/01.in");
	std::ifstream ans("sort/01.out");
	int N;
	in >> N;
	int a[MAX_N];
	for (int i = 0; i < N; ++i)
		in >> a[i];
	heapsort(a, a + N, (std::less<int>()));
  for (int j = 0; j < N; ++j)
  {
    int t;
    ans >> t;
    EXPECT_EQ(t, a[j]);
  }
}

TEST(heapsort, informatics766test2)
{
  std::ifstream in("sort/02.in");
  std::ifstream ans("sort/02.out");
  int N;
  in >> N;
  int a[MAX_N];
  for (int i = 0; i < N; ++i)
    in >> a[i];
  heapsort(a, a + N, (std::less<int>()));
  for (int j = 0; j < N; ++j)
  {
    int t;
    ans >> t;
    EXPECT_EQ(t, a[j]);
  }
}

TEST(heapsort, informatics766test3)
{
  std::ifstream in("sort/03.in");
  std::ifstream ans("sort/03.out");
  int N;
  in >> N;
  int a[MAX_N];
  for (int i = 0; i < N; ++i)
    in >> a[i];
  heapsort(a, a + N, (std::less<int>()));
  for (int j = 0; j < N; ++j)
  {
    int t;
    ans >> t;
    EXPECT_EQ(t, a[j]);
  }
}

TEST(heapsort, informatics766test4)
{
  std::ifstream in("sort/04.in");
  std::ifstream ans("sort/04.out");
  int N;
  in >> N;
  int a[MAX_N];
  for (int i = 0; i < N; ++i)
    in >> a[i];
  heapsort(a, a + N, (std::less<int>()));
  for (int j = 0; j < N; ++j)
  {
    int t;
    ans >> t;
    EXPECT_EQ(t, a[j]);
  }
}

TEST(heapsort, informatics766test5)
{
  std::ifstream in("sort/05.in");
  std::ifstream ans("sort/05.out");
  int N;
  in >> N;
  int a[MAX_N];
  for (int i = 0; i < N; ++i)
    in >> a[i];
  heapsort(a, a + N, (std::less<int>()));
  for (int j = 0; j < N; ++j)
  {
    int t;
    ans >> t;
    EXPECT_EQ(t, a[j]);
  }
}

TEST(heapsort, informatics766test6)
{
  std::ifstream in("sort/06.in");
  std::ifstream ans("sort/06.out");
  int N;
  in >> N;
  int a[MAX_N];
  for (int i = 0; i < N; ++i)
    in >> a[i];
  heapsort(a, a + N, (std::less<int>()));
  for (int j = 0; j < N; ++j)
  {
    int t;
    ans >> t;
    EXPECT_EQ(t, a[j]);
  }
}

TEST(heapsort, informatics766test7)
{
  std::ifstream in("sort/07.in");
  std::ifstream ans("sort/07.out");
  int N;
  in >> N;
  int a[MAX_N];
  for (int i = 0; i < N; ++i)
    in >> a[i];
  heapsort(a, a + N, (std::less<int>()));
  for (int j = 0; j < N; ++j)
  {
    int t;
    ans >> t;
    EXPECT_EQ(t, a[j]);
  }
}

TEST(heapsort, informatics766test8)
{
  std::ifstream in("sort/08.in");
  std::ifstream ans("sort/08.out");
  int N;
  in >> N;
  int a[MAX_N];
  for (int i = 0; i < N; ++i)
    in >> a[i];
  heapsort(a, a + N, (std::less<int>()));
  for (int j = 0; j < N; ++j)
  {
    int t;
    ans >> t;
    EXPECT_EQ(t, a[j]);
  }
}

TEST(heapsort, informatics766test9)
{
  std::ifstream in("sort/09.in");
  std::ifstream ans("sort/09.out");
  int N;
  in >> N;
  int a[MAX_N];
  for (int i = 0; i < N; ++i)
    in >> a[i];
  heapsort(a, a + N, (std::less<int>()));
  for (int j = 0; j < N; ++j)
  {
    int t;
    ans >> t;
    EXPECT_EQ(t, a[j]);
  }
}

TEST(heapsort, informatics766test10)
{
  std::ifstream in("sort/10.in");
  std::ifstream ans("sort/10.out");
  int N;
  in >> N;
  int a[MAX_N];
  for (int i = 0; i < N; ++i)
    in >> a[i];
  heapsort(a, a + N, (std::less<int>()));
  for (int j = 0; j < N; ++j)
  {
    int t;
    ans >> t;
    EXPECT_EQ(t, a[j]);
  }
}

TEST(heapsort, informatics766test11)
{
  std::ifstream in("sort/11.in");
  std::ifstream ans("sort/11.out");
  int N;
  in >> N;
  int a[MAX_N];
  for (int i = 0; i < N; ++i)
    in >> a[i];
  heapsort(a, a + N, (std::less<int>()));
  for (int j = 0; j < N; ++j)
  {
    int t;
    ans >> t;
    EXPECT_EQ(t, a[j]);
  }
}

TEST(heapsort, informatics766test12)
{
  std::ifstream in("sort/12.in");
  std::ifstream ans("sort/12.out");
  int N;
  in >> N;
  int a[MAX_N];
  for (int i = 0; i < N; ++i)
    in >> a[i];
  heapsort(a, a + N, (std::less<int>()));
  for (int j = 0; j < N; ++j)
  {
    int t;
    ans >> t;
    EXPECT_EQ(t, a[j]);
  }
}

TEST(heapsort, informatics766test13)
{
  std::ifstream in("sort/13.in");
  std::ifstream ans("sort/13.out");
  int N;
  in >> N;
  int a[MAX_N];
  for (int i = 0; i < N; ++i)
    in >> a[i];
  heapsort(a, a + N, (std::less<int>()));
  for (int j = 0; j < N; ++j)
  {
    int t;
    ans >> t;
    EXPECT_EQ(t, a[j]);
  }
}

TEST(heapsort, informatics766test14)
{
  std::ifstream in("sort/14.in");
  std::ifstream ans("sort/14.out");
  int N;
  in >> N;
  int a[MAX_N];
  for (int i = 0; i < N; ++i)
    in >> a[i];
  heapsort(a, a + N, (std::less<int>()));
  for (int j = 0; j < N; ++j)
  {
    int t;
    ans >> t;
    EXPECT_EQ(t, a[j]);
  }
}

TEST(heapsort, informatics766test15)
{
  std::ifstream in("sort/15.in");
  std::ifstream ans("sort/15.out");
  int N;
  in >> N;
  int a[MAX_N];
  for (int i = 0; i < N; ++i)
    in >> a[i];
  heapsort(a, a + N, (std::less<int>()));
  for (int j = 0; j < N; ++j)
  {
    int t;
    ans >> t;
    EXPECT_EQ(t, a[j]);
  }
}

TEST(heapsort, informatics766test16)
{
  std::ifstream in("sort/16.in");
  std::ifstream ans("sort/16.out");
  int N;
  in >> N;
  int a[MAX_N];
  for (int i = 0; i < N; ++i)
    in >> a[i];
  heapsort(a, a + N, (std::less<int>()));
  for (int j = 0; j < N; ++j)
  {
    int t;
    ans >> t;
    EXPECT_EQ(t, a[j]);
  }
}

TEST(heapsort, informatics766test17)
{
  std::ifstream in("sort/17.in");
  std::ifstream ans("sort/17.out");
  int N;
  in >> N;
  int a[MAX_N];
  for (int i = 0; i < N; ++i)
    in >> a[i];
  heapsort(a, a + N, (std::less<int>()));
  for (int j = 0; j < N; ++j)
  {
    int t;
    ans >> t;
    EXPECT_EQ(t, a[j]);
  }
}

TEST(heapsort, informatics766test18)
{
  std::ifstream in("sort/18.in");
  std::ifstream ans("sort/18.out");
  int N;
  in >> N;
  int a[MAX_N];
  for (int i = 0; i < N; ++i)
    in >> a[i];
  heapsort(a, a + N, (std::less<int>()));
  for (int j = 0; j < N; ++j)
  {
    int t;
    ans >> t;
    EXPECT_EQ(t, a[j]);
  }
}

TEST(heapsort, informatics766test19)
{
  std::ifstream in("sort/19.in");
  std::ifstream ans("sort/19.out");
  int N;
  in >> N;
  int a[MAX_N];
  for (int i = 0; i < N; ++i)
    in >> a[i];
  heapsort(a, a + N, (std::less<int>()));
  for (int j = 0; j < N; ++j)
  {
    int t;
    ans >> t;
    EXPECT_EQ(t, a[j]);
  }
}

TEST(heapsort, informatics766test20)
{
  std::ifstream in("sort/20.in");
  std::ifstream ans("sort/20.out");
  int N;
  in >> N;
  int a[MAX_N];
  for (int i = 0; i < N; ++i)
    in >> a[i];
  heapsort(a, a + N, (std::less<int>()));
  for (int j = 0; j < N; ++j)
  {
    int t;
    ans >> t;
    EXPECT_EQ(t, a[j]);
  }
}

TEST(heapsort, informatics766test21)
{
  std::ifstream in("sort/21.in");
  std::ifstream ans("sort/21.out");
  int N;
  in >> N;
  int a[MAX_N];
  for (int i = 0; i < N; ++i)
    in >> a[i];
  heapsort(a, a + N, (std::less<int>()));
  for (int j = 0; j < N; ++j)
  {
    int t;
    ans >> t;
    EXPECT_EQ(t, a[j]);
  }
}

TEST(heapsort, informatics766test22)
{
  std::ifstream in("sort/22.in");
  std::ifstream ans("sort/22.out");
  int N;
  in >> N;
  int a[MAX_N];
  for (int i = 0; i < N; ++i)
    in >> a[i];
  heapsort(a, a + N, (std::less<int>()));
  for (int j = 0; j < N; ++j)
  {
    int t;
    ans >> t;
    EXPECT_EQ(t, a[j]);
  }
}

TEST(heapsort, informatics766test23)
{
  std::ifstream in("sort/23.in");
  std::ifstream ans("sort/23.out");
  int N;
  in >> N;
  int a[MAX_N];
  for (int i = 0; i < N; ++i)
    in >> a[i];
  heapsort(a, a + N, (std::less<int>()));
  for (int j = 0; j < N; ++j)
  {
    int t;
    ans >> t;
    EXPECT_EQ(t, a[j]);
  }
}