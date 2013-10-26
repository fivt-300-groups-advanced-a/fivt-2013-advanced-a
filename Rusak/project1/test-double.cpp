#include <string>
#include <iostream>

#include "lib/generalreader.h"
#include "lib/regularreader.h"
#include "lib/binaryreader.h"

#include "lib/generalwriter.h"
#include "lib/regularwriter.h"
#include "lib/binarywriter.h"

#include "lib/sorters.h"
#include "lib/externalsorter.h"

int main() {
  RegularReader<double> is("input2.txt");
  RegularWriter<double> os("output2.txt");
  ExternalSorter<double> ex;
  ex(16, is, os);
}
