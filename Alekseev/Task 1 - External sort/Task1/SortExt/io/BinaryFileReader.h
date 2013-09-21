#ifndef BINARYFILEREADER_H
#define BINARYFILEREADER_H

#include "filecommunicator.h"
#include "abstractreader.h"

template <typename T>
class BinaryFileReader: public FileCommunicator, public AbstractReader<T>
{
public:
    explicit BinaryFileReader(FILE *file):
        FileCommunicator(file)
    {}
    explicit BinaryFileReader(const char *fileName):
        FileCommunicator(fileName, "rb")
    {}

    T next()
    {
        fread(buffer, sizeof(T), 1, this->file());
        return *((T*)buffer);
    }
    void hasNext()
    {
        return file() && !feof(file());
    }

private:
    char buffer[sizeof(T)];
};

#endif // BINARYFILEREADER_H
