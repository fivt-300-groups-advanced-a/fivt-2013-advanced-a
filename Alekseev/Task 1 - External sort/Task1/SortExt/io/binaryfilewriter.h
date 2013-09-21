#ifndef BINARYFILEWRITER_H
#define BINARYFILEWRITER_H

#include "filecommunicator.h"
#include "abstractwriter.h"

template <typename T>
class BinaryFileWriter: FileCommunicator, AbstractWriter<T>
{
public:
    explicit BinaryFileWriter(FILE *file):
        FileCommunicator(file)
    {}
    explicit BinaryFileWriter(const char *fileName):
        FileCommunicator(fileName, "wb")
    {}

    void put(const T &some)
    {
        fwrite(&some, sizeof(some), 1, file());
    }
    void flush()
    {
        fflush(file());
    }
};

#endif // BINARYFILEWRITER_H
