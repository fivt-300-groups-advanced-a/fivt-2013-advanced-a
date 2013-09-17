#include <iostream>
#include <fstream>

#include "io/binaryreader.h"

#include "io/binarywriter.h"
#include "io/writer.h"

#include <cstdlib>

int main()
{
//    std::ifstream bin("inp.txt", std::ios::binary | std::ios::in);
    BinaryReader<char> breader(&std::cin);
    int ok = breader.next();

    BinaryWriter<char> briter(&std::cout);
    briter.put(ok);

    Writer<int> writer(&std::cout);
    briter.put(' ');
    writer.put(ok);

    return 0;
}

