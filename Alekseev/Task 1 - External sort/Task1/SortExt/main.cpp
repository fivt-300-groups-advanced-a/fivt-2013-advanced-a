#include <iostream>

#include "istreamreader.h"
#include "ostreamwriter.h"

int main()
{
    IStreamReader<char> reader;
    OStreamWriter<int> writer(std::cout, "\n");

    while (reader.hasNext())
        writer.put(reader.next());

    return 0;
}

