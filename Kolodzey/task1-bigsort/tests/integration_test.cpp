#include <fstream>
#include "bigsort.h"
#include "gtest/gtest.h"

bool mycmp (std::pair <int, int> a, std::pair <int, int> b)
{
    return a.first > b.first;
}

void mysort(std::vector<int>::iterator beg,
            std::vector<int>::iterator end)
{
    sort(beg, end);
}

TEST (IntegrationTest, SmallInteger)
{
	std::ifstream fin("input.txt");
    std::fstream fout;
    std::remove("output.txt");
    fout.open("output.txt",
    		  std::fstream::in | std::fstream::out | std::fstream::app);
    
    const int k = 3;
    std::fstream b[k];

    for (int i = 0; i < k; ++i)
    {
        char s[9] = {'t', 'm', 'p', '0' + i, '.', 't', 'x', 't'};
        std::remove(s);
        b[i].open(s,
                  std::fstream::in | std::fstream::out | std::fstream::app);
     }
    int a = 0;
    
    bigsort::bigsort (a, fin, fout, " ", 3, b, " ", mysort, mycmp, std::cout);

    b[0].close();
    b[1].close();
    b[2].close();

    std::ifstream rightAnswer ("output.a");
    fout.seekg(0, fout.beg);
    while (!rightAnswer.eof())
    {
    	int t, ta;
    	fout >> t;
    	rightAnswer >> ta;
    	EXPECT_EQ(t, ta);
    }
}