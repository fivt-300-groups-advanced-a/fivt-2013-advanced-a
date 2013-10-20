#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include "../../sort/digital_sorter.h"

/*
 * DigitalExtractor testing
 */

const unsigned int MOD = (1 << 16);

TEST(TestIntegerDigitExtractor, Int){
    unsigned int a = 241241431;
    IntegerDigitExtractor<int> extractor;
    EXPECT_EQ(extractor.getCntDigits(a), 2);
    EXPECT_EQ((extractor.getDigit(a, 1) - MOD) * MOD + 
            extractor.getDigit(a, 0) - MOD, a);

    a = 0;
    EXPECT_EQ(extractor.getCntDigits(a), 1);
    EXPECT_EQ(extractor.getDigit(a, 0) - MOD, a);
    
    a = 3213;
    EXPECT_EQ(extractor.getCntDigits(a), 1);
    EXPECT_EQ((extractor.getDigit(a, 1) - MOD) * MOD + 
            extractor.getDigit(a, 0) - MOD, a);

    a = -42343224;
    EXPECT_EQ(extractor.getCntDigits(a), 2);
    EXPECT_EQ((extractor.getDigit(a, 1) - MOD) * MOD + 
            extractor.getDigit(a, 0) - MOD, a);
}

/*
 * DigitalSorter testing
 */

TEST(TestDigitalSorter, Int){
    std::vector<int> a(6);
    a[0] = -12, a[1] = 213, a[2] = 31343432, a[3] = 0;
    a[4] = -2365352, a[5] = 432;
    std::vector<int> b = a;
    std::sort(b.begin(), b.end());
    DigitalSorter<int, IntegerDigitExtractor<int> > sorter;
    sorter(a);
    for (int i = 0; i < 6; i++)
        ASSERT_EQ(a[i], b[i]);
}
