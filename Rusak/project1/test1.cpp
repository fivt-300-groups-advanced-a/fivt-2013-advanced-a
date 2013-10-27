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

int main() {
  RegularReader<int> is("input1.txt");
  RegularWriter<int> os("output1.txt");
  auto ex = NewExternalSorter<int>(8, is, os, StdSort<int>(), std::greater<int>());
  ex.run();
}
