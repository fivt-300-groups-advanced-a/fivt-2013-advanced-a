#include <string>
#include <iostream>

#include "lib/generalreader.h"
#include "lib/regularreader.h"
#include "lib/binaryreader.h"

int main() {
  //std::string z = "input1.txt";
  //RegularReader<int> is(z);
  //FAIL
 
  RegularReader<int> is("input1.txt");
  int n; is(n);
  std::cout << n << "\n";
  is(n);
  std::cout << n << "\n";

  RegularReader<double> is2("input2.txt");
  double q; is2(q);
  std::cout << q << "\n";

  BinaryReader<char> is3("binary_input");
  for (int i=0;i<5;i++) {
    char c; is3(c);
    std::cout << c;
  }
  std::cout << "\n";
}
