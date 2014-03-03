#include <random>
#include "gtest/gtest.h"
#include "putsigns.h"

TEST(Stress, comparedWithDummy) {
  for (int i = 0; i < 100; ++i) {
    std::default_random_engine gen(i);
    std::uniform_int_distribution<int> distr(-8, 8);
    auto numberMaker = std::bind(distr, gen);
    std::vector <int> testcase(10);
    std::generate(testcase.begin(), testcase.end(), numberMaker);
    for (int j = 0; j < 10; ++j)
      std::cout << testcase[j] << " ";
    std::cout << std::endl;
    ASSERT_EQ(dummy(testcase), putsigns(testcase)) << "failed with seed" << i;
  }
}