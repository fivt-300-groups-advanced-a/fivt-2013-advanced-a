#include <iostream>

#include "istreamreader.h"
#include "ostreamwriter.h"

#include "fscanfreader.h"
#include "fprintfwriter.h"

int main()
{
//    IStreamReader<char> reader;
    FscanfReader<char> reader(stdin, "%c");
    FprintfWriter<int> writer(stdout, "%d\n");

    while (reader.hasNext())
        writer.put(reader.next());

    return 0;
}

