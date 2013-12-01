#include <fstream>

int main()
{
	std::ofstream out ("fresh_test.in");
	out << "Helloworld" <<std::endl;
	return 0;
}