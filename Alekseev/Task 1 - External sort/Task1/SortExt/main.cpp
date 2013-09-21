#include <iostream>

#include "istreamreader.h"
#include "istreambinaryreader.h"

int main()
{
    IStreamBinaryReader<char> reader;

    while (reader.hasNext())
        std::cout << (int)reader.next() << std::endl;

    return 0;
}

