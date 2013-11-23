#include "BinomialHeap.h"
#include <functional>
#include <iostream>

int main() {
    BinomialHeap<int, std::less<int> > *heap = new BinomialHeap<int, std::less<int> >(0);
    std::cout << heap->getMin();
    std::cout.flush();
    BinomialHeap<int, std::less<int> > *heap1 = new BinomialHeap<int, std::less<int> >(1);
    std::cout << heap1->getMin();
    std::cout.flush();
    heap->mergeHeap(heap1);
    std::cout << heap->getMin();
    return 0;
}

