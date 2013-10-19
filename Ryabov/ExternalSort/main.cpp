#include <functional>
#include <fstream>
#include <string>
#include <iomanip>
#include <memory>
#include <time.h>

#include "externalsort.h"
#include "rwstreams.h"
#include "sorter.h"
#include "radixsort.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>


using namespace std;

TEST (RadixSort, RadixIntSortN_1000000_FastEx)
{
    const int n = 1000000;
    vector<int> v1(n);
    for (int i = 0; i < n; ++i)
        v1[i] = ((rand() << 16) + rand());
    vector<int> v2 = v1;
    auto c1 = clock();
    sort(v1.begin(), v1.end());
    auto c2 = clock();
    RadixSort :: sort(v2.begin(), v2.end(), RadixSort :: FastIntExtractor());
    auto c3 = clock();
    cout << "STD: " << c2 - c1 << " ms -- RADIX: " << c3 - c2 << "ms -- ratio: " << (double)(c3 - c2) / (c2 - c1) << endl;
    for (int i = 0; i < n; ++i)
        EXPECT_EQ(v1[i], v2[i]);
}

TEST (RadixSort, RadixIntSortN_100000_SimpleEx)
{
    const int n = 100000;
    vector<int> v1(n);
    for (int i = 0; i < n; ++i)
        v1[i] = ((rand() << 16) + rand());
    vector<int> v2 = v1;
    int c1 = clock();
    sort(v1.begin(), v1.end());
    int c2 = clock();
    RadixSort :: sort(v2.begin(), v2.end(), RadixSort :: SimpleIntExtractor());
    int c3 = clock();
    cout << "STD: " << c2 - c1 << " ms -- RADIX: " << c3 - c2 << "ms -- ratio: " << (double)(c3 - c2) / (c2 - c1) << endl;
    for (int i = 0; i < n; ++i)
        EXPECT_EQ(v1[i], v2[i]);
}

TEST (BinaryRW, INTRW_N_100)
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

TEST (BinaryRW, DOUBLERW_N_100)
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

TEST (RW, INTRW_N_100)
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

TEST (IntegrateTest, Int_N_10000_MEM_100)
{
    const int n = 10000;
    vector<int> v(n);
    for (int i = 0; i < n; ++i)
        v[i] = rand() + 100;
    ofstream cout1("12345");
    for (int i=  0; i < n; ++i)
        cout1 << v[i] << " ";
    cout1.close();
    ExternalSorter<int> exs;
    Reader<int> reader(unique_ptr<ifstream>(new ifstream("12345")));
    Writer<int> writer(unique_ptr<ofstream>(new ofstream("12345.out")));
    exs.sort(100,
        reader,
        writer,
        StandartSorter<less<int> >(),
        less<int>());
    sort(v.begin(), v.end());
    ifstream cin1("12345.out");
    for (int i = 0; i < n; ++i)
    {
        int x;
        cin1 >> x;
        EXPECT_EQ(x, v[i]);
    }
}

TEST (IntegrateTest, Int_N_100000_MEM_1000)
{
    const int n = 100000;
    vector<int> v(n);
    for (int i = 0; i < n; ++i)
        v[i] = ((int)rand() << 16) << rand();
    ofstream cout1("12345");
    for (int i=  0; i < n; ++i)
        cout1 << v[i] << " ";
    cout1.close();
    ExternalSorter<int> exs;
    Reader<int> reader(unique_ptr<ifstream>(new ifstream("12345")));
    Writer<int> writer(unique_ptr<ofstream>(new ofstream("12345.out")));
    exs.sort(1000,
        reader,
        writer,
        StandartSorter<less<int> >(),
        less<int>());
    sort(v.begin(), v.end());
    ifstream cin1("12345.out");
    for (int i = 0; i < n; ++i)
    {
        int x;
        cin1 >> x;
        EXPECT_EQ(x, v[i]);
    }
}

TEST (IntegrateTest, Int_N_100000_MEM_1000_Radix)
{
    const int n = 100000;
    vector<int> v(n);
    for (int i = 0; i < n; ++i)
        v[i] = ((int)rand() << 16) << rand();
    ofstream cout1("12345");
    for (int i=  0; i < n; ++i)
        cout1 << v[i] << " ";
    cout1.close();
    ExternalSorter<int> exs;
    Reader<int> reader(unique_ptr<ifstream>(new ifstream("12345")));
    Writer<int> writer(unique_ptr<ofstream>(new ofstream("12345.out")));
    exs.sort(1000,
        reader,
        writer,
        RadixSorter<RadixSort :: FastIntExtractor>(),
        less<int>());
    sort(v.begin(), v.end());
    ifstream cin1("12345.out");
    for (int i = 0; i < n; ++i)
    {
        int x;
        cin1 >> x;
        EXPECT_EQ(x, v[i]);
    }
}

TEST (IntegrateTest, Double_N_100000_MEM_10000)
{
    const int n = 100000;
    vector<double> v(n);
    for (int i = 0; i < n; ++i)
        v[i] = (double)rand() * (double)rand() * (double)rand() / (double)rand();
    ofstream cout1("12345");
    for (int i=  0; i < n; ++i)
        cout1.write(reinterpret_cast<char*>(&v[i]), sizeof v[i]);
    cout1.close();
    ExternalSorter<double> exs;
    BinaryReader<double> reader(unique_ptr<ifstream>(new ifstream("12345")));
    BinaryWriter<double> writer(unique_ptr<ofstream>(new ofstream("12345.out")));
    exs.sort(10000,
        reader,
        writer,
        StandartSorter<less<double> >(),
        less<double>());
    sort(v.begin(), v.end());
    ifstream cin1("12345.out");
    for (int i = 0; i < n; ++i)
    {
        double x;
        cin1.read(reinterpret_cast<char*>(&x), sizeof (x));
        EXPECT_EQ(x, v[i]);
    }
}

#ifdef TEST_BUILD
int main(int argc, char **argv)
{
    srand(time(NULL));
    testing :: InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#else
int main(int argc, char **argv)
{
    ExternalSorter<int> exs;
    Reader<int> reader(unique_ptr<ifstream>(new ifstream("123")));
    Writer<int> writer(unique_ptr<ofstream>(new ofstream("123.txt")));

    exs.sort(8,
        reader,
        writer,
        StandartSorter<less<int> >(),
        less<int>());
    return 0;
}

#endif
