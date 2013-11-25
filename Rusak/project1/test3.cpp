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
  int count_of_files = 1000;
  std::vector<BinaryReader<int>> readers(count_of_files);
  for (int i=0;i<count_of_files;i++) {
    readers[i].assign(temp_file_name(i));
  }

  int elem;
  std::cout << "good: " << readers[277](elem) << "\n";
  std::cout << "ELEM1: " << elem << "\n";
  return 0;
} 
