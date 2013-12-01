#include <string>
#include <iostream>
#include <cstdio>

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

int main() {
  int t = 200;
  for (int i=0;i<t;i++) {
    BinaryWriter<int> os(temp_file_name(i));
    for (int j=5;j<100000;j++) os(j);
    os.close();
  }
  std::vector< BinaryReader<int> > readers(t);
  for (int i=0;i<t;i++) {
    readers[i].assign(temp_file_name(i));
  }
  int good = 0;
  for (int i=0;i<t;i++) {
    int x; if (readers[i](x)) {
      good++;
      std::cout << x << "\n";
    }
  }
  std::cout << "Good files: " << good << "\n";
  return 0;
}
