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
#include<sys/resource.h>


int main() {
//  BinaryReader<int> is("temp112");
//  int x; while (is(x)) {
//    std::cout << x << "\n";
//  }
  struct rlimit rlim;
  getrlimit(RLIMIT_NOFILE, &rlim);
  printf("Max number of open files: %d\n", rlim.rlim_cur-1);

  return 0;
}
