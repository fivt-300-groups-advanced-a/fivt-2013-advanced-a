#include <string>
#include <iostream>

#include "lib1/generalreader.h"
#include "lib1/regularreader.h"
#include "lib1/binaryreader.h"

#include "lib1/generalwriter.h"
#include "lib1/regularwriter.h"
#include "lib1/binarywriter.h"

#include "lib1/sorters.h"
#include "lib1/externalsorter.h"
#include <ctime>
#include <cstdlib>

void read_write_regular() {
  RegularWriter<int> os("temp.txt");
  for (int i=0;i<10000;i++) os(i);
  os.close();
  RegularReader<int> is("temp.txt");
  int it; 
  std::vector<int> read_ints;
  while (is(it)) {
    read_ints.push_back(it);
  }
  bool good = true;
  if (read_ints.size()!=10000) good = false;
  if (good) {
    for (size_t i=0;i<read_ints.size();i++) if (read_ints[i]!=i) good=false;
  }
  if (good) {
    std::cout << "Read Write Regular correct\n";
  }
  else {
    std::cout << "Read Write Regular ERROR\n";
  }
  is.close();
  remove("temp.txt");
}

void read_write_binary() {
  BinaryWriter<int> os("temp.txt");
  for (int i=0;i<10000;i++) os(i);
  os.close();
  BinaryReader<int> is("temp.txt");
  int it; 
  std::vector<int> read_ints;
  while (is(it)) {
    read_ints.push_back(it);
  }
  bool good = true;
  if (read_ints.size()!=10000) good = false;
  if (good) {
    for (size_t i=0;i<read_ints.size();i++) if (read_ints[i]!=i) good=false;
  }
  if (good) {
    std::cout << "Read Write Binary correct\n";
  }
  else {
    std::cout << "Read Write Binary ERROR\n";
  }
  is.close();
  remove("temp.txt");
}

void sort_rand_ints_asc() {
  //generate
  BinaryWriter<int> os("temp.txt");
  for (int i=0;i<10000;i++) os(rand());
  os.close();
  //sort
  BinaryReader<int> is1("temp.txt");
  BinaryWriter<int> os1("result.txt");
  auto ex = NewExternalSorter<int>(4000, is1, os1);
  ex.run();
  os1.close();
  is1.close();
  //check
  BinaryReader<int> is2("result.txt");
  bool good = true;
  std::vector<int> read_ints;
  int it; while (is2(it)) {
    read_ints.push_back(it);
  }
  for (size_t i=1;i<read_ints.size();i++) if (read_ints[i-1]>read_ints[i]) good = false;

  if (good) {
    std::cout << "Sort Rand ints ASC correct\n";
  }
  else {
    std::cout << "Sort Rand ints ASC ERROR\n";
  }
  is2.close();
  remove("temp.txt");
  remove("result.txt");
}

void sort_rand_ints_desc() {
  //generate
  BinaryWriter<int> os("temp.txt");
  for (int i=0;i<10000;i++) os(rand());
  os.close();
  //sort
  BinaryReader<int> is1("temp.txt");
  BinaryWriter<int> os1("result.txt");
  auto ex = NewExternalSorter<int>(4000, is1, os1, StdSort<int>(), std::greater<int>());
  ex.run();
  os1.close();
  is1.close();
  //check
  BinaryReader<int> is2("result.txt");
  bool good = true;
  std::vector<int> read_ints;
  int it; while (is2(it)) {
    read_ints.push_back(it);
  }
  for (size_t i=1;i<read_ints.size();i++) if (read_ints[i-1]<read_ints[i]) good = false;

  if (good) {
    std::cout << "Sort Rand ints DESC correct\n";
  }
  else {
    std::cout << "Sort Rand ints DESC ERROR\n";
  }
  is2.close();
  remove("temp.txt");
  remove("result.txt");
}

void sort_rand_doubles_asc() {
  //generate
  BinaryWriter<double> os("temp.txt");
  for (int i=0;i<10000;i++) os((double)rand()/RAND_MAX*1000);
  os.close();
  //sort
  BinaryReader<double> is1("temp.txt");
  BinaryWriter<double> os1("result.txt");
  auto ex = NewExternalSorter<double>(4000, is1, os1);
  ex.run();
  os1.close();
  is1.close();
  //check
  BinaryReader<double> is2("result.txt");
  bool good = true;
  std::vector<int> read_doubles;
  double it; while (is2(it)) {
    read_doubles.push_back(it);
  }
  for (size_t i=1;i<read_doubles.size();i++) if (read_doubles[i-1]>read_doubles[i]) good = false;

  if (good) {
    std::cout << "Sort Rand doubles ASC correct\n";
  }
  else {
    std::cout << "Sort Rand doubles ASC ERROR\n";
  }
  is2.close();
  remove("temp.txt");
  remove("result.txt");
}

void sort_speed_ints(int col, int RAM) {
  clock_t t1 = clock();
  //generate
  BinaryWriter<int> os("temp.txt");
  for (int i=0;i<col;i++) os(rand());
  os.close();
  clock_t t2 = clock();
  //sort
  BinaryReader<int> is1("temp.txt");
  BinaryWriter<int> os1("result.txt");
  auto ex = NewExternalSorter<int>(RAM, is1, os1);
  ex.run();
  os1.close();
  is1.close();
  clock_t t3 = clock();
  std::cout << "Sort " << col << " ints, with " << RAM << " of RAM\n";
  std::cout << "Check speed:\n";
  std::cout << "Generate: " << (double)(t2-t1)/CLOCKS_PER_SEC << " sec.\n";
  std::cout << "Sort: " << (double)(t3-t2)/CLOCKS_PER_SEC << " sec.\n";
  remove("temp.txt");
  remove("result.txt");
}

int main() {
  //Example:
  //RegularReader<int> is("input1.txt");
  //RegularWriter<int> os("output1.txt");
  //auto ex = NewExternalSorter<int>(8, is, os, StdSort<int>(), std::greater<int>());
  //ex.run();
  srand(time(NULL));
  read_write_regular();
  read_write_binary();
  sort_rand_ints_asc();
  sort_rand_ints_desc();
  sort_rand_doubles_asc();
  //sort_speed_ints(100000000, 4000000);
  sort_speed_ints(100000, 400);
}
