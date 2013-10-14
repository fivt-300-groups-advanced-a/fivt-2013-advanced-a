#include "BinaryReader.h"
#include "Reader.h"
#include <iostream>
#include <string>
#include <cassert>

using namespace std;

int main()
{
	ofstream binwriter("bin_in2.bin", ios::out | ios::binary);
	double y = 123456.78;
	bool f = binwriter.write((char *)&y, sizeof(y));
	assert(f);
	binwriter.close();

	BinaryReader <double> br("bin_in2.bin");
	double x;
	br(x);
	printf("x = %.5lf\n", x);
}