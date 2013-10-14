#include <string>
#include <iostream>
#include <fstream>
#include "bigsort.h"

bool mycmp (std::pair <int, int> a, std::pair <int, int> b)
{
    return a.first > b.first;
}

int main()
{
    std::ifstream fin("input.txt");
    std::ofstream fout("output.txt");
    
    std::fstream fs;
    fs.open ("test.txt", std::fstream::in | std::fstream::out);

    fs << "100400 more lorem ipsum";
    fs.seekg(0);

    int a = 100500;
    fs >> a;
    std::cout << a << std::endl;

    fs.close();

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
    void (*fun) (int*, int*) = &std::sort;
    bigsort::bigsort (a, fin, fout, 3, 
                      mycmp, std::less<int>(), fun,
                      b);

    b[0].close();
    b[1].close();
    b[2].close();


    return 0;
}