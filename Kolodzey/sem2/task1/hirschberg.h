#ifndef HIRSCHBERG_H
#define HIRSCHBERG_H

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

namespace hirschberg {

using std::string;
using std::swap;
using std::pair;
using std::vector;
using std::min;
using std::max;
using std::sort;

const int kInfinity = 1000050000;

enum ElementalInstructionType {
  kChange = 0,
  kDelete = 1,
  kInsertBefore = 2, //just for fun, useless
  kInsertAfter = 3,
};

struct StatisticsCounter {
  int wfmaxheight;
  int wftotallaunches;
  int wftotalactions;
  int sctotallaunches;
  StatisticsCounter()
      : wfmaxheight(0),
        wftotallaunches(0),
        wftotalactions(0),
        sctotallaunches(0) {}
};

inline int IntAbs(int x) {
  if (x < 0)
    return -x;
  return x;
}

class StringPosition;

//class to iterate trough string hold in StringPosition
class StpIterator {
 public:
  const StringPosition* stp_ref_;
  int val_;

  StpIterator(const StringPosition* stp_ref, int val)
      : stp_ref_(stp_ref),
        val_(val) {
  }

  bool operator == (StpIterator const it) const {
    return (it.val_ == val_) && (it.stp_ref_ == stp_ref_);
  }

  bool operator != (StpIterator const it) const {
    return !(*this == it);
  }

  const StpIterator& operator ++ () {
    ++val_;
    return *this;
  }

  char operator *();
};

//class StringPosition
//holds indexes in parent string, means string [first, last)
//if last == first, means empty string
//if last < first, means reversed string [last, first)
// str = "abc"
// (0,2) = ab
// (2,0) = ba
class StringPosition {
 public:
  const string* str_ref_;
  int first_;
  int last_;

  StringPosition(const string* str_ref, int first, int last)
      : str_ref_(str_ref),
        first_(first),
        last_(last) {
  }

  bool IsEmpty() const { return first_ == last_; }

  int Length() const { return IntAbs(last_ - first_); }

  StringPosition Reversed() { 
    return StringPosition(str_ref_, last_, first_);
  }

  bool IsReversed() const { return (last_ - first_) < 0; }

  //not working for reversed
  pair<StringPosition,StringPosition> Divide(int firstlen) const {
    return pair<StringPosition,StringPosition> (
               StringPosition(str_ref_, first_, first_ + firstlen),
               StringPosition(str_ref_, first_ + firstlen, last_));
  }
  
  StringPosition FirstHalf() const {
    return StringPosition(str_ref_, first_, (first_ + last_) / 2);
  }

  StringPosition LastHalf() const {
    return StringPosition(str_ref_, (first_ + last_) / 2, last_);
  }

  StpIterator Begin() const { return StpIterator(this, 0); }

  StpIterator End() const { return StpIterator(this, Length()); }

  bool operator == (const StringPosition stp) const {
    return (stp.str_ref_ == str_ref_) &&
           (stp.first_ == first_) &&
           (stp.last_ == last_);
  }
};

char StpIterator::operator * () {
  const StringPosition& base_stringposition = *stp_ref_;
  const string& base_string = *(base_stringposition.str_ref_);
  int pos = -1;
  if (base_stringposition.IsReversed()) {
    pos = base_stringposition.last_ + base_stringposition.Length() - val_ - 1;
  } else {  
    pos = base_stringposition.first_ + val_;
  }
  return base_string.at(pos);
}

//very comfortable class for storing (and counting) data
//for last two steps of hirshberg algorithm
class StepData {
 public:
  vector <int> data_[2];

  StepData(int result_length) {
    data_[0].resize(result_length + 1);
    data_[1].resize(result_length + 1);
    for (size_t i = 0; i < data_[0].size(); ++i)
      data_[0][i] = i;
  }
  
  static int GetSrcIndex(StpIterator src_it) { return (src_it.val_ + 1) % 2; }
  static int GetResIndex(StpIterator res_it) { return res_it.val_ + 1; }
  static int Prev(int index) { return (index - 1 < 0) ? 1 : (index - 1); }

  void Init(StpIterator src_it, int res_index, int value) {
    data_[GetSrcIndex(src_it)][res_index] = value;
  }

  void Init(StpIterator src_it, StpIterator res_it, int value) {
    data_[GetSrcIndex(src_it)][GetResIndex(res_it)] = value;
  }

  void TryToChange(StpIterator src_it, StpIterator res_it) {
    data_[GetSrcIndex(src_it)][GetResIndex(res_it)] = min(
        data_[     GetSrcIndex(src_it) ][     GetResIndex(res_it) ],
        data_[Prev(GetSrcIndex(src_it))][Prev(GetResIndex(res_it))] + 1);
  }
  
  void TryToInsert(StpIterator src_it, StpIterator res_it) {
    data_[GetSrcIndex(src_it)][GetResIndex(res_it)] = min(
        data_[GetSrcIndex(src_it)][     GetResIndex(res_it) ],
        data_[GetSrcIndex(src_it)][Prev(GetResIndex(res_it))] + 1);
  }

  void TryToDelete(StpIterator src_it, StpIterator res_it) {
    data_[GetSrcIndex(src_it)][GetResIndex(res_it)] = min(
        data_[     GetSrcIndex(src_it) ][GetResIndex(res_it)],
        data_[Prev(GetSrcIndex(src_it))][GetResIndex(res_it)] + 1);
  }

  void TryToKeep(StpIterator src_it, StpIterator res_it) {
    data_[GetSrcIndex(src_it)][GetResIndex(res_it)] = min(
        data_[     GetSrcIndex(src_it) ][     GetResIndex(res_it)],
        data_[Prev(GetSrcIndex(src_it))][Prev(GetResIndex(res_it))]);
  }

  vector <int> GetLastStep(int source_length) {
    return data_[source_length % 2];
  }
};

vector<int> GetLastStepOfWagnerFischer (StringPosition src_stp,
                                        StringPosition res_stp,
            /* param for testing --> */ StatisticsCounter* cnt = NULL) {
  //==code below is for testing only==
  if (cnt != NULL) {
    ++(cnt->wftotallaunches);
    cnt->wftotalactions += src_stp.Length() * res_stp.Length();
    cnt->wfmaxheight = max(cnt->wfmaxheight, res_stp.Length());
  }
  //==code above is for testing only==

  StepData step_data(res_stp.Length());
  StpIterator src_it = src_stp.Begin();
  for (int i = 1; src_it != src_stp.End(); ++src_it, ++i) {
    step_data.Init(src_it, 0, i);
    StpIterator res_it = res_stp.Begin();
    for (; res_it != res_stp.End(); ++res_it) {
      step_data.Init(src_it, res_it, kInfinity);
      step_data.TryToChange(src_it, res_it);
      step_data.TryToInsert(src_it, res_it);
      step_data.TryToDelete(src_it, res_it);
      if (*src_it == *res_it)
        step_data.TryToKeep(src_it, res_it);
    }
  }
  return step_data.GetLastStep(src_stp.Length());
}

//count using Wagner-Fischer
int CountLevenshteinDistance (const string& source, const string& result) {
  StringPosition stp_source(&source, 0, source.length());
  StringPosition stp_result(&result, 0, result.length());
  return GetLastStepOfWagnerFischer(stp_source, stp_result).back();
}

//structures for answer reconstruction
//values that are count
struct HoldInstruction {
  int l;
  int r;
  int me;
  HoldInstruction(): l(-2), r(-2), me(-2) {}
  HoldInstruction(int newl, int newr, int newme) : l(newl),
                                                   r(newr),
                                                   me(newme) {}
  bool operator == (const HoldInstruction cmd) const {
    return ((me == cmd.me) && (l == cmd.l) && (r == cmd.r));
  }
};

//values that are returned
struct ElementalInstruction {
  ElementalInstructionType type;
  int source_pos;
  char ch;
  int nice_number;
  
  ElementalInstruction(ElementalInstructionType newtype, int newsource_pos,
                       char newch, int newnice_number)
    : type(newtype),
      source_pos(newsource_pos),
      ch(newch),
      nice_number(newnice_number) {}
  
  bool operator < (const ElementalInstruction cmd) const {
    return nice_number > cmd.nice_number;
  }
};

//class to cout the way we restore the answer
class EditorialPrescription {
 public:
  vector<HoldInstruction> data_; //holds [l,r) or -1, -1, -1

  EditorialPrescription(int source_length)
      : data_(source_length, HoldInstruction(-2, -2, -2)) {}

  void AddDeletions(StringPosition stp_src) {
    //well, if input stp is reversed, loop wil do nothing
    //but we don't need to launch it for reversed stp
    for (int i = stp_src.first_; i < stp_src.last_; ++i) {
      data_[i] = HoldInstruction(-1, -1, -1);
    }
  }
  //don't launch it for reversed stp!!!
  void Add1toMany(StringPosition stp_src, StringPosition stp_res) {
    const string& res = *(stp_res.str_ref_);
    size_t eqpos = res.find(*stp_src.Begin(), stp_res.first_);
    if ((eqpos == std::string::npos) || ((int)(eqpos) >= stp_res.last_)) {
      eqpos = stp_res.first_;
    }
    int src_pos = stp_src.first_;
    data_[src_pos].me = (int)(eqpos);
    data_[src_pos].l = stp_res.first_;
    data_[src_pos].r = stp_res.last_;
  }

  //returns vector of instructions sorted in order of decrease of nice_number
  //so, in right order
  vector<ElementalInstruction> GetElementalInstructions(const string& source,
                                                        const string& result) {
    vector<ElementalInstruction> ans;
    int current_nice_number = 0;
    for (int source_pos = 0; source_pos < (int)(data_.size()); ++source_pos) {
      int me = data_[source_pos].me;
      int l = data_[source_pos].l;
      int r = data_[source_pos].r;
      if (me == -2) {
        std::cerr << "LOL, -2 at " << source_pos << std::endl;
      } else if (me == -1) {
        ans.push_back(ElementalInstruction(kDelete, source_pos, '-',
                                           current_nice_number));
        ++current_nice_number;
      } else {
        for (int result_pos = l; result_pos != me; ++result_pos) {
          ans.push_back(ElementalInstruction(kInsertAfter, source_pos - 1,
                                             result[result_pos],
                                             current_nice_number));
          ++current_nice_number;
        }
        if (source[source_pos] != result[me]) {
          ans.push_back(ElementalInstruction(kChange, source_pos,
                                             result[me],
                                             current_nice_number));
          ++current_nice_number;
        }
        for (int result_pos = me + 1; result_pos != r; ++result_pos) {
          ans.push_back(ElementalInstruction(kInsertAfter, source_pos,
                                             result[result_pos],
                                             current_nice_number));
          ++current_nice_number;
        }
      }
    }
    sort(ans.begin(), ans.end());
    return ans;
  }
};

//small function useful in hirschberg
int ChooseTheBestPartition(vector<int> prefvalue, vector<int> sufvalue) {
  int len = prefvalue.size() - 1;
  int ans = 0;
  for (int preflen = 0; preflen <= len; ++preflen)
    if ((prefvalue[preflen] + sufvalue[len - preflen]) <
        (prefvalue[    ans] + sufvalue[len -     ans])) {
      ans = preflen;
    }
  return ans;
}

//so, main function of hirchberg algorithm
void SetUpCorrespondence(StringPosition stp_source,
                         StringPosition stp_result,
                         EditorialPrescription* ans,            
   /* for testing --> */ StatisticsCounter* cnt = NULL) {
  //==code below is for testing only==
  if (cnt != NULL) {
    ++(cnt->sctotallaunches);
  }
  //==code above is for testing only==
  
  if (stp_result.IsEmpty()) {
    ans->AddDeletions(stp_source);
    return;
  }

  if (stp_source.Length() == 1) {
    ans->Add1toMany(stp_source, stp_result);
    return;
  }

  StringPosition stp_firstsource = stp_source.FirstHalf();
  StringPosition stp_lastsource = stp_source.LastHalf();
  vector<int> prefvalue = GetLastStepOfWagnerFischer(stp_firstsource,
                                                     stp_result, cnt);
  vector<int> sufvalue = GetLastStepOfWagnerFischer(stp_lastsource.Reversed(),
                                                    stp_result.Reversed(),
                                                    cnt);
  int firstlen = ChooseTheBestPartition(prefvalue, sufvalue);
  StringPosition stp_firstresult = stp_result.Divide(firstlen).first;
  StringPosition stp_lastresult = stp_result.Divide(firstlen).second;
  SetUpCorrespondence(stp_firstsource, stp_firstresult, ans, cnt);
  SetUpCorrespondence( stp_lastsource,  stp_lastresult, ans, cnt);
}

//dummy laucher, for informatics
int CountLevenshteinDistance2 (const string& source, const string& result) {
  StringPosition stp_source(&source, 0, source.length());
  StringPosition stp_result(&result, 0, result.length());
  EditorialPrescription ans(source.length());
  SetUpCorrespondence(stp_source, stp_result, &ans);
  vector<ElementalInstruction> cmdList = ans.GetElementalInstructions(source,
                                                                      result);
  return cmdList.size();
}

//functions for checker = applying prescription
void Change(string& str, ElementalInstruction cmd) {
  str[cmd.source_pos] = cmd.ch;
}

void Delete(string& str, ElementalInstruction cmd) {
  str.erase(cmd.source_pos, 1);
}

void InsertAfter(string& str, ElementalInstruction cmd) {
  if (cmd.source_pos + 1 == (int)(str.length()))
    str.append(1, cmd.ch);
  else
    str.insert(cmd.source_pos + 1, 1, cmd.ch);
}

string ApplyPrescription(const string& str, vector<ElementalInstruction> cmd) {
  string ansstr = str;
  vector<ElementalInstruction>::iterator it;
  for (it = cmd.begin(); it != cmd.end(); ++it) {
    switch((*it).type) {
      case kDelete:
        Delete(ansstr, *it);
        break;
      case kChange:
        Change(ansstr, *it);
        break;
      case kInsertAfter:
        InsertAfter(ansstr, *it);
        break;
      case kInsertBefore:
        std::cerr << "OMG, kInsertBefore" << std::endl;
        break;
    }
  }
  return ansstr;
}

//so, function, which will be launched by user
//ta-dam!
vector<ElementalInstruction> GetListOfPrescriptions(
                                 const string& source, 
                                 const string& result,
           /* for testing --> */ StatisticsCounter* cnt = NULL) {
  StringPosition stp_source(&source, 0, source.length());
  StringPosition stp_result(&result, 0, result.length());
  EditorialPrescription ans(source.length());
  SetUpCorrespondence(stp_source, stp_result, &ans, cnt);
  vector<ElementalInstruction> cmdList = ans.GetElementalInstructions(source,
                                                                      result);
  return cmdList;
}

} //namespace hirschberg
#endif //HIRSCHBERG_H