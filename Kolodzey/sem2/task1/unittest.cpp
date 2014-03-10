#include "hirschberg.h"
#include "gtest/gtest.h"

using std::string;
using std::vector;
using std::pair;

using hirschberg::StringPosition;
using hirschberg::StpIterator;
using hirschberg::StepData;
using hirschberg::GetLastStepOfWagnerFischer;
using hirschberg::EditorialPrescription;
using hirschberg::HoldInstruction;
using hirschberg::ChooseTheBestPartition;
using hirschberg::ElementalInstruction;
using hirschberg::kChange;
using hirschberg::kDelete;
using hirschberg::kInsertAfter;
using hirschberg::Change;
using hirschberg::Delete;
using hirschberg::InsertAfter;
using hirschberg::ApplyPrescription;

TEST(StringPosition, IsEmpty) {
  string str = "abc";

  StringPosition stp1(&str, 0, 3);
  EXPECT_EQ(false, stp1.IsEmpty());

  StringPosition stp2(&str, 2, 1);
  EXPECT_EQ(false, stp2.IsEmpty());

  StringPosition stp3(&str, 1, 1);
  EXPECT_EQ(true, stp3.IsEmpty());
}

TEST(StringPosition, Length) {
  string str = "abc";

  StringPosition stp1(&str, 0, 3);
  EXPECT_EQ(3, stp1.Length());

  StringPosition stp2(&str, 2, 1);
  EXPECT_EQ(1, stp2.Length());

  StringPosition stp3(&str, 1, 1);
  EXPECT_EQ(0, stp3.Length());
}

TEST(StringPosition, Reverse) {
  string str = "abc";
  StringPosition stp(&str, 0, 3);

  EXPECT_EQ(3, (stp.Reversed()).first_);
  EXPECT_EQ(0, (stp.Reversed()).last_);

  EXPECT_EQ(0, ((stp.Reversed()).Reversed()).first_);
  EXPECT_EQ(3, ((stp.Reversed()).Reversed()).last_);
}

TEST(StringPosition, IsReversed) {
  string str = "abc";

  StringPosition stp1(&str, 0, 3);
  EXPECT_EQ(false, stp1.IsReversed());

  StringPosition stp2(&str, 2, 1);
  EXPECT_EQ(true, stp2.IsReversed());

  StringPosition stp3(&str, 1, 1);
  EXPECT_EQ(false, stp3.IsReversed());
}

TEST(StringPosition, Divide) {
  string str = "abcdefg";
  StringPosition stp(&str, 1, 7);
  EXPECT_EQ(StringPosition(&str, 1, 5), stp.Divide(4).first);
  EXPECT_EQ(StringPosition(&str, 5, 7), stp.Divide(4).second);
}

TEST(StringPosition, FirstHalf) {
  string str = "abcdefg";
  StringPosition stp(&str, 2, 4);
  EXPECT_EQ(StringPosition(&str, 2, 3), stp.FirstHalf());
}

TEST(StringPosition, LastHalf) {
  string str = "abcdefg";
  StringPosition stp(&str, 2, 4);
  EXPECT_EQ(StringPosition(&str, 3, 4), stp.LastHalf());
}

TEST(StringPosition, Begin) {
  string str = "abcdefg";
  StringPosition stp1(&str, 2, 4);
  EXPECT_EQ(0, stp1.Begin().val_);
  EXPECT_EQ(stp1, *(stp1.Begin().stp_ref_));
  StringPosition stp2(&str, 5, 0);
  EXPECT_EQ(0, stp2.Begin().val_);
  EXPECT_EQ(stp2, *(stp2.Begin().stp_ref_));
}

TEST(StringPosition, End) {
  string str = "abcdefg";
  StringPosition stp1(&str, 3, 5);
  EXPECT_EQ(2, stp1.End().val_);
  EXPECT_EQ(stp1, *(stp1.End().stp_ref_));
  StringPosition stp2(&str, 5, 0);
  EXPECT_EQ(5, stp2.End().val_);
  EXPECT_EQ(stp2, *(stp2.End().stp_ref_));
}

TEST(StpIterator, eq) {
  string str = "abcdefg";
  StringPosition stp1(&str, 1, 3);
  StringPosition stp2(&str, 2, 4);
  StpIterator it11(&stp1, 1);
  StpIterator it12(&stp1, 2);
  StpIterator it21(&stp2, 1);
  StpIterator jt11(&stp1, 1);
  EXPECT_EQ(1, (it11 == jt11));
  EXPECT_EQ(0, (it11 == it12));
  EXPECT_EQ(0, (it11 == it21));
}

TEST(StpIterator, uneq) {
  string str = "abcdefg";
  StringPosition stp1(&str, 1, 3);
  StringPosition stp2(&str, 2, 4);
  StpIterator it11(&stp1, 1);
  StpIterator it12(&stp1, 2);
  StpIterator it21(&stp2, 1);
  StpIterator jt11(&stp1, 1);
  EXPECT_EQ(0, (it11 != jt11));
  EXPECT_EQ(1, (it11 != it12));
  EXPECT_EQ(1, (it11 != it21));
}

TEST(StpIterator, inc) {
  string str = "abcdefg";
  StringPosition stp(&str, 1, 3);
  StpIterator it(&stp, 1);
  ++it;
  EXPECT_EQ(StpIterator(&stp, 2), it);
}

//str = "abcdefg"
//stp =  "bcde"
//stprv = "edcb"
TEST(StpIterator, dereference) {
  string str = "abcdefg";

  StringPosition stp(&str, 1, 5);
  StpIterator it(&stp, 1);
  EXPECT_EQ('c', *it);

  ++it;
  EXPECT_EQ('d', *it);

  StringPosition stprv = stp.Reversed();
  StpIterator jt(&stprv, 0);
  EXPECT_EQ('e', *jt);

  ++jt;
  EXPECT_EQ('d', *jt);
}

TEST(StepData, Constructor) {
  StepData step_data(3);
  vector<int> data0 = {0, 1, 2, 3};
  EXPECT_EQ(data0, step_data.data_[0]);
  EXPECT_EQ(4, step_data.data_[1].size());
}

TEST(StepData, Prev) {
  EXPECT_EQ(0, StepData::Prev(1));
  EXPECT_EQ(1, StepData::Prev(0));
  EXPECT_EQ(5, StepData::Prev(6));
}

TEST(StepData, GetSrcIndex) {
  string str = "abcdefg";
  StringPosition stp(&str, 7, 1);
  StpIterator it(&stp, 3);
  EXPECT_EQ(0, StepData::GetSrcIndex(it));
}

TEST(StepData, GetResIndex) {
  string str = "abcdefg";
  StringPosition stp(&str, 7, 1);
  StpIterator it(&stp, 3);
  EXPECT_EQ(4, StepData::GetResIndex(it));
}

TEST(GetLastStepOfWagnerFischer, Manual) {
  string source = "abd";
  string result = "adc";
  StringPosition stp_res(&result, 0, 3);
  vector <pair<StringPosition, vector<int>>> testcases = {
    {{StringPosition(&source, 0, 1)},{1, 0, 1, 2}},
    {{StringPosition(&source, 0, 2)},{2, 1, 1, 2}},
    {{StringPosition(&source, 0, 3)},{3, 2, 1, 2}},
  };
  for (auto test : testcases)
    EXPECT_EQ(test.second, GetLastStepOfWagnerFischer(test.first, stp_res));
}

TEST(GetLastStepOfWagnerFischer, ManualReversed) {
  string source = "dba";
  string result = "cda";
  StringPosition stp_res(&result, 3, 0);
  vector <pair<StringPosition, vector<int>>> testcases = {
    {{StringPosition(&source, 3, 2)},{1, 0, 1, 2}},
    {{StringPosition(&source, 3, 1)},{2, 1, 1, 2}},
    {{StringPosition(&source, 3, 0)},{3, 2, 1, 2}},
  };
  for (auto test : testcases)
    EXPECT_EQ(test.second, GetLastStepOfWagnerFischer(test.first, stp_res));
}

TEST(EditorialPrescrition, Add1toMany) {
  string source = "abc";
  string result = "abca";
  EditorialPrescription cmd(source.length());
  StringPosition src_a (&source, 0, 1);
  StringPosition src_b (&source, 1, 2);
  StringPosition res_a0 (&result, 0, 1);
  StringPosition res_abc (&result, 0, 3);
  StringPosition res_bc (&result, 1, 3);
  StringPosition res_ca (&result, 2, 4);
  cmd.Add1toMany(src_a, res_a0);
  EXPECT_EQ(HoldInstruction(0, 1, 0), cmd.data_[0]);
  cmd.Add1toMany(src_a, res_abc);
  EXPECT_EQ(HoldInstruction(0, 3, 0), cmd.data_[0]);
  cmd.Add1toMany(src_a, res_bc);
  EXPECT_EQ(HoldInstruction(1, 3, 1), cmd.data_[0]);
  cmd.Add1toMany(src_a, res_ca);
  EXPECT_EQ(HoldInstruction(2, 4, 3), cmd.data_[0]);

  cmd.Add1toMany(src_b, res_a0);
  EXPECT_EQ(HoldInstruction(0, 1, 0), cmd.data_[1]);
  cmd.Add1toMany(src_b, res_abc);
  EXPECT_EQ(HoldInstruction(0, 3, 1), cmd.data_[1]);
  cmd.Add1toMany(src_b, res_bc);
  EXPECT_EQ(HoldInstruction(1, 3, 1), cmd.data_[1]);
  cmd.Add1toMany(src_b, res_ca);
  EXPECT_EQ(HoldInstruction(2, 4, 2), cmd.data_[1]);
}

TEST(ChooseTheBestPartition, Manual) {
  vector<pair<int,pair<vector<int>,vector<int>>>> testcases = {
    {0, {{1, 2, 3}, {3, 2, 1}}},
    {1, {{2, 1, 3}, {3, 1, 2}}},
    {2, {{3, 2, 1}, {1, 2, 3}}}
  };
  for (auto test : testcases)
    EXPECT_EQ(test.first,
              ChooseTheBestPartition(test.second.first, test.second.second));
}

TEST(ApplyPrescription, Change) {
  string s = "abcd";
  Change(s, ElementalInstruction(kChange, 0, 'e', 0));
  EXPECT_EQ("ebcd", s);
  Change(s, ElementalInstruction(kChange, 1, 'f', 0));
  EXPECT_EQ("efcd", s);
  Change(s, ElementalInstruction(kChange, 3, 'g', 0));
  EXPECT_EQ("efcg", s);
}

TEST(ApplyPrescription, Delete) {
  string s = "abcd";
  Delete(s, ElementalInstruction(kDelete, 0, '-', 0));
  EXPECT_EQ("bcd", s);
  Delete(s, ElementalInstruction(kDelete, 2, '-', 0));
  EXPECT_EQ("bc", s);
}

TEST(ApplyPrescription, InsertAfter) {
  string s = "abcd";
  InsertAfter(s, ElementalInstruction(kInsertAfter, -1, 'e', 0));
  EXPECT_EQ("eabcd", s);
  InsertAfter(s, ElementalInstruction(kInsertAfter, 4, 'f', 0));
  EXPECT_EQ("eabcdf", s);
  InsertAfter(s, ElementalInstruction(kInsertAfter, 3, 'g', 0));
  EXPECT_EQ("eabcgdf", s);
}

TEST(ApplyPrescription, Manual) {
  string s = "abcd";
  vector <ElementalInstruction> cmd = {
    ElementalInstruction(kInsertAfter, -1, 'e', 0),
    ElementalInstruction(kInsertAfter,  4, 'f', 0),
    ElementalInstruction(kInsertAfter,  3, 'g', 0)
  };
  EXPECT_EQ("eabcgdf", ApplyPrescription(s, cmd));

  s = "abcd";
  cmd = {
    ElementalInstruction(kDelete, 2, '-', 0),
    ElementalInstruction(kDelete, 0, '-', 0),
  };
  EXPECT_EQ("bd", ApplyPrescription(s, cmd));

  s = "abcd";
  cmd = {
    ElementalInstruction(kChange, 0, 'e', 0),
    ElementalInstruction(kChange, 1, 'f', 0),
    ElementalInstruction(kChange, 3, 'g', 0)
  };
  EXPECT_EQ("efcg", ApplyPrescription(s, cmd));
}


