#ifndef TESTING_FUNCTIONS_H

#define TESTING_FUNCTIONS_H

#include <cstdlib>
#include <ctime>

#include "../binomial_heap.h"

template <class T, class Comparator> 
void eraseFromVector(std::vector<T> &vect, T val){
    Comparator cmp;
    for (int i = 0; i < (int)vect.size(); i++)
        if (!cmp(val, vect[i]) && !cmp(vect[i], val)){
            vect.erase(vect.begin() + i);
            return;
        }
}

#endif
