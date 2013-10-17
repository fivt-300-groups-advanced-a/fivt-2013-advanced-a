#include <functional>
#include <fstream>
#include <string>
#include <iomanip>
#include <memory>
#include "externalsort.h"
#include "rwstreams.h"
#include "sorter.h"
#include <gtest/gtest.h>
//#define BOOST_TEST_DYN_LINK
//#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>
#include <gmock/gmock.h>

using namespace std;

TEST(BinaryRW, INTRW)
{
    const int n = 100;
    vector<int> v(n);
    BinaryWriter<int> bw(unique_ptr<ofstream>(new ofstream("12345", ios :: out | ios :: binary)));
    for (int i = 0; i < n; ++i)
    {
        v[i] = ((int)rand()  << 16) + rand();
        bw.write(v[i]);
    }
    bw.close();

    BinaryReader<int> br(unique_ptr<ifstream>(new ifstream("12345", ios :: in | ios :: binary)));
    for (int i = 0; i < n; ++i)
    {
        int x;
        EXPECT_TRUE(br.read(x));
        EXPECT_EQ(x, v[i]);
    }
    br.close();
}

TEST(BinaryRW, DOUBLERW)
{
    const int n = 100;
    vector<double> v(n);
    BinaryWriter<double> bw(unique_ptr<ofstream>(new ofstream("12345", ios :: out | ios :: binary)));
    for (int i = 0; i < n; ++i)
    {
        v[i] = (double)rand() * (double)rand() * (double)rand() / ((double)rand() + 1.0);
        bw.write(v[i]);
    }
    bw.close();

    BinaryReader<double> br(unique_ptr<ifstream>(new ifstream("12345", ios :: in | ios :: binary)));
    for (int i = 0; i < n; ++i)
    {
        double x;
        EXPECT_TRUE(br.read(x));
        EXPECT_EQ(x, v[i]);
    }
    br.close();
}

TEST(RW, INTRW)
{
    const int n = 100;
    vector<int> v(n);
    Writer<int> bw(unique_ptr<ofstream>(new ofstream("12345")));
    for (int i = 0; i < n; ++i)
    {
        v[i] = ((int)rand()  << 16) + rand();
        bw.write(v[i]);
    }
    bw.close();

    Reader<int> br(unique_ptr<ifstream>(new ifstream("12345")));
    for (int i = 0; i < n; ++i)
    {
        int x;
        EXPECT_TRUE(br.read(x));
        EXPECT_EQ(x, v[i]);
    }
    br.close();
}

#ifdef TEST_BUILD
int main(int argc, char **argv)
{
    testing :: InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#else
int main()
{
    ExternalSorter<int> exs;
    Reader<int> reader(unique_ptr<ifstream>(new ifstream("123")));
    Writer<int> writer(unique_ptr<ofstream>(new ofstream("123.txt")));

    exs.sort(8,
        reader,
        writer,
        StandartSorter<int>(),
        less<int>());
    return 0;
}
#endif
