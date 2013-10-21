#include <string>
#include <iostream>

#include "lib/generalreader.h"
#include "lib/regularreader.h"
#include "lib/binaryreader.h"

#include "lib/generalwriter.h"
#include "lib/regularwriter.h"
#include "lib/binarywriter.h"

#include "lib/externalsorter.h"

int main() {
 
  RegularReader<int> is("input1.txt");
  RegularWriter<int> os("output1.txt");
  ExternalSorter<int> ex;
  ex(24, is, os, std::less<int>());
}
