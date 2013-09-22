#ifndef ISTREAMBINARYREADER_H
#define ISTREAMBINARYREADER_H

#include "abstractreader.h"
#include "streamcommunicator.h"

template <typename T>
class IStreamBinaryReader : public AbstractReader<T>, public StreamCommunicator<std::istream>
{
public:
    explicit IStreamBinaryReader(std::istream &stream = std::cin):
        StreamCommunicator<std::istream>(stream)
    {}
    explicit IStreamBinaryReader(const char *fileName):
        StreamCommunicator<std::istream>(fileName, std::ios_base::in | std::ios_base::binary)
    {}

    T next()
    {
        this->stream().read(buffer, sizeof(T));
        return *((T*)buffer);
    }
    bool hasNext()
    {
        return this->stream();
    }

private:
    char buffer[sizeof(T)];
};

#endif // ISTREAMBINARYREADER_H
