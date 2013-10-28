#include <string>
#include <iostream>

#include "lib0/generalreader.h"
#include "lib0/regularreader.h"
#include "lib0/binaryreader.h"

#include "lib0/generalwriter.h"
#include "lib0/regularwriter.h"
#include "lib0/binarywriter.h"

#include "lib0/sorters.h"
#include "lib0/externalsorter.h"

int main() {
  RegularReader<int> is("input1.txt");
  RegularWriter<int> os("output1.txt");
  ExternalSorter<int> ex;
  ex(8, is, os);
}
