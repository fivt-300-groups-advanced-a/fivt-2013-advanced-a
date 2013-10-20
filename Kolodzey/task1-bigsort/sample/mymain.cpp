#include <functional>
#include <string>
#include <iostream>
#include <fstream>
#include "bigsort.h"
 

bool mycmp (std::pair <int, int> a, std::pair <int, int> b)
{
    return a.first > b.first;
}

void mysort(std::vector<int>::iterator beg,
            std::vector<int>::iterator end)
{
    sort(beg, end);
}


struct Sorter
{
    void operator () (std::vector<int>::iterator beg,
                      std::vector<int>::iterator end)
    {
        std::sort(beg, end);
    }

};

int main()
{
    std::ifstream fin("input.txt");
    std::ofstream fout("output.txt");
    
    const int k = 3;
    std::fstream b[k];
    for (int i = 0; i < k; ++i)
    {
        char s[9] = {'t', 'm', 'p', '0' + i, '.', 't', 'x', 't'};
        std::cout << s << std::endl;
        std::remove(s);
        b[i].open(s,
                  std::fstream::in | std::fstream::out | std::fstream::app);
     }
    std::cout << "still not fail" << std::endl;
    int a = 0;
    
    bigsort::bigsort (a, fin, fout, 3, 
                      mycmp, mysort,
                      b, "\n", " ");

    

    b[0].close();
    b[1].close();
    b[2].close();

    return 0;
}