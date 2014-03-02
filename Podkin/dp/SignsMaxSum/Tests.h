#ifndef TESTS_H
#define TESTS_H

#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "MaxSum.h"
#include "SlowMaxSum.h"
#include <stdlib.h>
#include <iostream>

TEST(Test, Test) {
    srand(41);
    std::vector <long long> mas(10);
    for (int t = 0; t < 1000; t++) {
        for (int i = 0; i < 10; i++) {
            mas[i] = rand() % 20 - 10;
        }
        ASSERT_EQ(MaxSum(mas), SlowMaxSum(mas));
    }
}

#endif
