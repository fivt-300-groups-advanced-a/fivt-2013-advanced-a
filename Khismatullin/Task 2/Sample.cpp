#include <iostream>
#include "Cheap.h"

using namespace std;

int main()
{
	Cheap<int, std::less<int> > h1, h2;
	for(int i = 0; i < 3; i++)
	{
		h2.insert(2 * i);
	}
	for(int i = 0; i < 3; i++)
	{
		h1.insert(2 * i + 1);
	}
	return 0;
}
	