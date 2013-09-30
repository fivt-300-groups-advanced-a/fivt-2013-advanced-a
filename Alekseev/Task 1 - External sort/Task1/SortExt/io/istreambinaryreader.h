#ifndef ISTREAMBINARYREADER_H
#define ISTREAMBINARYREADER_H

//#include "abstractreader.h"
#include "streamcommunicator.h"

template <typename T>
class IStreamBinaryReader : public StreamCommunicator<std::istream>
{
public:
    explicit IStreamBinaryReader(std::istream &stream = std::cin):
        StreamCommunicator<std::istream>(stream)
    {}
    explicit IStreamBinaryReader(const char *fileName):
        StreamCommunicator<std::istream>(fileName, std::ios_base::in | std::ios_base::binary)
    {}

    bool operator() (T &result)
    {
        if (!stream())
            return false;
        stream().read(buffer, sizeof(T));
        if (!stream())
            return false;
        result = *reinterpret_cast<T*>(buffer);
        return true;
    }

private:
    char buffer[sizeof(T)];
};

#endif // ISTREAMBINARYREADER_H
