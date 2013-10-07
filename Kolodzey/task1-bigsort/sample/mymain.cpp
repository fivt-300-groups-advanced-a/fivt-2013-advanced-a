#include <string>
#include <iostream>
#include <fstream>
#include "bigsort.h"

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

    const int k = 2;
    std::fstream b[k];
    for (int i = 0; i < k; ++i)
    {
        char s[9];
        s[0] = 't';
        s[1] = 'm';
        s[2] = 'p';
        s[3] = '0' + i;
        s[4] = '.';
        s[5] = 't';
        s[6] = 'x';
        s[7] = 't';
        s[8] = 0;
        std::cout << s << std::endl;
        std::remove(s);
        b[i].open(s,
                  std::fstream::in | std::fstream::out | std::fstream::app);
     }
    std::cout << "still not fail" << std::endl;
    void (*fun) (int*, int*) = &std::sort;
    bigsort::bigsort (a, fin, fout, 2, 
                      std::less<int>(), std::less<int>(), fun,
                      b);

    b[0].close();
    b[1].close();


    return 0;
}