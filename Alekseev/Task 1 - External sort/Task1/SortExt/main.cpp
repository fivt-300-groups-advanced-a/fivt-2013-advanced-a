#include <iostream>

#include "istreamreader.h"
#include "ostreamwriter.h"

#include "fscanfreader.h"
#include "fprintfwriter.h"

#include "istreambinaryreader.h"
#include "ostreambinarywriter.h"

int main()
{
//    IStreamReader<char> reader;
//    FscanfReader<char> reader(stdin, "%c");
    IStreamBinaryReader<int> reader(std::cin);
//    FprintfWriter<int> writer(stdout, "%d\n");
    OStreamBinaryWriter<int> writer(std::cout);

    while (reader.hasNext())
        writer.put(reader.next());

    return 0;
}
