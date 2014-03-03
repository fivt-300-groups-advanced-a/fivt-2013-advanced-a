#include "gtest/gtest.h"
#include "putsigns.h"
#include <vector>

TEST(Manual, MainSolution) {
  std::vector<std::pair<std::vector<int>,int>> testcases = {
    {{},0},
    {{1, 1}, 2},
    {{0, 1}, 1},
    {{-1, -1}, 1},
    {{5, -2, 1}, 4}
  };
  for (auto test : testcases)
    EXPECT_EQ(test.second, putsigns(test.first));
}

TEST(Manual, DummySolution) {
  std::vector<std::pair<std::vector<int>,int>> testcases = {
    {{},0},
    {{1, 1}, 2},
    {{0, 1}, 1},
    {{-1, -1}, 1},
    {{5, -2, 1}, 4}
  };
  for (auto test : testcases)
    EXPECT_EQ(test.second, dummy(test.first));
}