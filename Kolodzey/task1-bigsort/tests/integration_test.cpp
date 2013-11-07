#include <fstream>
#include <sstream> 
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
	std::stringstream s_in("9 4 3 2 5");
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
    
    bigsort::bigsort (a, s_in, fout, " ", 3, b, " ", mysort, mycmp, std::cout);

    b[0].close();
    b[1].close();
    b[2].close();

    std::stringstream rightAnswer ("2 3 4 5 9");
    fout.seekg(0, fout.beg);
    while (!rightAnswer.eof())
    {
    	int t, ta;
    	fout >> t;
    	rightAnswer >> ta;
    	EXPECT_EQ(t, ta);
    }
}