#include <gtest/gtest.h>
#include "ExternalSorter.h"
#include "StandartSorter.h"
#include "FileReader.h"
#include "FileWriter.h"
#include <time.h>
#include <stdlib.h>
#include "functional"
#include <iostream>

int Numbers[100000000], NumberCounter[100000000];
int n = 100000000;

TEST(TestExternalSorter, IntegrationTest){
    srand(time(NULL));
    FileWriter<int> out("input.txt");
    for (int i = 0; i < n; i++)
        NumberCounter[i] = 0;
    for (int i = 0; i < n; i++){
        int Number = rand() % n;
        NumberCounter[Number]++;
        EXPECT_TRUE(out(Number));
    }
    out.SetStream("output.txt");
    FileReader<int> in("input.txt");
    StandartSorter<int> sort;
    ExternalSorter<int> ext_sort;
    std::less<int> comp;
    ext_sort(1000000, in, out, sort, comp);
    for (int i = 0; i < n; i++)
        Numbers[i] = 0;
    in.SetStream("output.txt");
    int Number;
    EXPECT_TRUE(in(Number));
    Numbers[Number]++;
    for (int i = 1; i < n; i++){
        int SecondNumber;
        EXPECT_TRUE(in(SecondNumber));
        EXPECT_TRUE(Number <= SecondNumber);
        Number = SecondNumber;
        Numbers[Number]++;
    }
    for (int i = 0; i < n; i++){
        EXPECT_EQ(NumberCounter[i], Numbers[i]);
    }
}
