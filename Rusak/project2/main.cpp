#include <iostream>

#include "heap.h"
//#include "heap_test.h"

int main() {
  auto heap = MakeHeap<int>();
 // auto test = MakeHeapTest<int>();
  for (;;) {
    std::string s; std::cin >> s;
    int co = 0;
    if (s=="push") {
      int v; std::cin >> v;
      heap.insert(v);
    }
    else if (s=="min"){
      std::cout << heap.find_min() << "\n";
      heap.extract_min();
    } 
  //  test.check_invariants(heap);
  }
  return 0;
}
