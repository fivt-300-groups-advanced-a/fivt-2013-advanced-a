#include <gtest/gtest.h>
#include "ExternalSorter.h"
#include "StandartSorter.h"
#include "FileReader.h"
#include "FileWriter.h"
#include <time.h>
#include <stdlib.h>
#include "functional"
#include <iostream>

const int sz = 10000;
const long long n = 400000LL;
const int memory = 400;

int Numbers[sz], NumberCounter[sz];

TEST(TestExternalSorter, IntegrationTest){
    srand(time(NULL));
    FileWriter<int> out("input.txt");
    for (int i = 0; i < sz; i++)
        NumberCounter[i] = 0;
    for (long long i = 0LL; i < n; i++){
        int Number = rand() % sz;
        NumberCounter[Number]++;
        EXPECT_TRUE(out(Number));
    }
    out.SetStream("output.txt");
    FileReader<int> in("input.txt");
    StandartSorter<int> sort;
    ExternalSorter<int> ext_sort;
    std::less<int> comp;
    ext_sort(memory, in, out, sort, comp);
    for (int i = 0; i < sz; i++)
        Numbers[i] = 0;
    in.SetStream("output.txt");
    int Number;
    EXPECT_TRUE(in(Number));
    Numbers[Number]++;
    for (long long i = 1LL; i < n; i++){
        int SecondNumber;
        EXPECT_TRUE(in(SecondNumber));
        EXPECT_TRUE(Number <= SecondNumber);
        Number = SecondNumber;
        Numbers[Number]++;
    }
    for (int i = 0; i < sz; i++){
        EXPECT_EQ(NumberCounter[i], Numbers[i]);
    }
}
