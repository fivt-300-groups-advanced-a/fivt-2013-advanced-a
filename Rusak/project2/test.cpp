#include <iostream>

#include "heap.h"

int main() {
  auto heap = MakeHeap<int>();
  for (;;) {
    std::string s; std::cin >> s;
    if (s=="push") {
      int v; std::cin >> v;
      heap.insert(v);
    }
    else {
      std::cout << heap.find_min() << "\n";
      heap.extract_min();
    }
  }
  return 0;
}
