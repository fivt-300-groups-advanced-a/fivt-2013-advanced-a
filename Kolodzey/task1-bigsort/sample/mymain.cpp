#include <iostream>
#include "bigsort.h"

int main()
{
    int a[5] = {1, 3, 5, 4, 2};
    
    bubblesort(a, 5);
    for (int i = 0; i < 5; ++i)
    	std::cout << a[i] << " ";
    std::cout << std::endl;

    reversedBubblesort(a, 5);
        for (int i = 0; i < 5; ++i)
    	std::cout << a[i] << " ";
    std::cout << std::endl;

    return 0;
}