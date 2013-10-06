#include <fstream>
#include "bigsort.h"

int main()
{
    std::ifstream fin("input.txt");
    std::ofstream fout("output.txt");
    
    int* a;
    void (*fun) (int*, int*) = &std::sort;
    bigsort::bigsort (a, fin, fout, 232546, 
                      std::less<int>(), std::less<int>(), fun,
                      fin, fout);


    return 0;
}