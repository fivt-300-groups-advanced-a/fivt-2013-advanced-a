#include <random>
#include "gtest/gtest.h"
#include "putsigns.h"

TEST(Stress, comparedWithDummy) {
  for (int i = 0; i < 100; ++i) {
    std::default_random_engine gen(i);
    std::uniform_int_distribution<int> distr(-4, 4);
    auto numberMaker = std::bind(distr, gen);
    std::vector <int> testcase(20);
    std::generate(testcase.begin(), testcase.end(), numberMaker);
    for (int j = 0; j < 20; ++j)
      std::cerr << testcase[j] << " ";
    std::cerr << std::endl;
    ASSERT_EQ(dummy(testcase), putsigns(testcase)) << "failed with seed" << i;
  }
}