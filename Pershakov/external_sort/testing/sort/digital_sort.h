#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "../../sort/digital_sorter.h"

/*
 * DigitalExtractor testing
 */

const unsigned int MOD = (1 << 16);

TEST(TestIntegerDigitExtractor, Int){
    srand(time(0));
    for (int i = 0; i < 1000; i++){
        int a = rand() % 1000000 - 2000000;;
        IntegerDigitExtractor<int> extractor;
        ASSERT_EQ(extractor.getCntDigits(a), 2);
        ASSERT_EQ((extractor.getDigit(a, 1) - MOD) * MOD + 
                extractor.getDigit(a, 0) - MOD, a);
    }
}

/*
 * DigitalSorter testing
 */

TEST(TestDigitalSorter, Int){
    int sz = 1000;
    std::vector<int> a(sz);
    for (int i = 0; i < sz; i++)
        a[i] = rand() % 1000000 - 2000000;
    std::vector<int> b = a;
    std::sort(b.begin(), b.end());
    DigitalSorter<int, IntegerDigitExtractor<int> > sorter;
    sorter(a);
    for (int i = 0; i < sz; i++)
        ASSERT_EQ(a[i], b[i]);
}
