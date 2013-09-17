#include <iostream>
#include <fstream>
#include <set>

#include "io/inputstreamreader.h"
#include "io/outputstreamwriter.h"
#include "io/binaryfilereader.h"
#include "io/binaryfilewriter.h"

int main()
{
	OutputStreamWriter<int> out;
	std::set<int> v = {1, 193, 10};
	out(v.begin(), v.end(), "[", "]");
	out.setDelimeter(" ");
	out(42), out(572), out(1);
	out.newLine();
	out.setDelimeter(" something ");
	out(47), out(-84), out(39 + 8491);
	out.unbindStream();
	return 0;
}
