#include "BinaryReader.h"
#include "Reader.h"

int main()
{
	char buf[5];
	BinaryReader bin_r("bin_in");
	bin_r(buf, 5);
	for(int j = 0; j < 5; j++)
		printf("%c", buf[j]);
	printf("\n");

	Reader r1("in1.txt");
	Reader r2("in2.txt");
	double x, y;
	r1(x);
	r2(y);
	printf("x = %.5lf y = %.5lf\n", x, y);
}
