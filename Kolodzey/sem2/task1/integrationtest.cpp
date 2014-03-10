#include <random>

#include "hirschberg.h"
#include "gtest/gtest.h"

using std::string;
using std::pair;
using std::vector;

using hirschberg::StatisticsCounter;
using hirschberg::ElementalInstruction;
using hirschberg::GetListOfPrescriptions;
using hirschberg::ApplyPrescription;
using hirschberg::CountLevenshteinDistance;

enum DebugMode {
 kFull = 0,
 kFast = 1,
};

void Check(pair<string, string> test, DebugMode mode) {
  StatisticsCounter cnt;
  vector<ElementalInstruction> ListOfPrescriptions = GetListOfPrescriptions(
                                                         test.first,
                                                         test.second,
                                                         &cnt);
  std::cerr << ListOfPrescriptions.size();
  
  //correctness
  EXPECT_EQ(CountLevenshteinDistance(test.first, test.second),
            ListOfPrescriptions.size());
  
  if (mode == kFull) {
    EXPECT_EQ(test.second,
              ApplyPrescription(test.first, ListOfPrescriptions));
  }
  
  //number of actions
  EXPECT_EQ(cnt.wfmaxheight, test.second.length());
  EXPECT_LT(cnt.wftotallaunches, 3 * test.first.length() + 5);
  EXPECT_LT(cnt.wftotalactions,
            3 * test.first.length() * test.second.length() + 25);
  EXPECT_LT(cnt.sctotallaunches, 3 * test.first.length() + 5);
}

TEST(Integration, Manual) {
  pair<string, string> test;
  test.first = "ACGTACGTACGT";
  test.second = "ACGTACGTACGT";
  Check(test, kFull);
}

string GenRandomString(size_t length, size_t alphabet_size, int seed) {
  std::default_random_engine generator(seed);
  std::uniform_int_distribution<char> distribution('a',
                                                   'a' + alphabet_size - 1);
  string ans;
  for (int i = 0; i < length; ++i)
    ans = ans.append(1, distribution(generator));
  return ans;
}

TEST(Integration, TwoRandomStringsFullCheck) {
  pair<string, string> test;
  test.first = GenRandomString(10000, 2, 0);
  test.second = GenRandomString(10000, 2, 10050);
  Check(test, kFast);
}

/*
TEST(Integration, TwoRandomStringsFastCheck) {
  pair<string, string> test;
  //test.first = GenRandomString(1000, 4, 0);
  //test.second = GenRandomString(1000, 4, 10050);
  Check(test, kFast);
}
*/