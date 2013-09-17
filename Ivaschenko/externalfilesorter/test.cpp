#include "inputstreamreader.h"
#include "binaryfilereader.h"

#include <iostream>
#include <fstream>
#include <set>

#include "inputstreamreader.h"
#include "outputstreamwriter.h"
#include "binaryfilereader.h"
#include "binaryfilewriter.h"

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
