#include <cstdio>
#include "Writer.h"

int main()
{
	Writer w("output.txt");
	int y = 100500;
	int z = 500100;
	w(y, " ");
	w(z, " ");
	//w.close();
}