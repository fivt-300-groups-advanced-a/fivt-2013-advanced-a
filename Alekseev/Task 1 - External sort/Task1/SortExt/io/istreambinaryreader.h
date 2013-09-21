#ifndef ISTREAMBINARYREADER_H
#define ISTREAMBINARYREADER_H

#include "istreamreader.h"

template <typename T>
class IStreamBinaryReader : public IStreamReader<T>
{
public:
//  :NOTE: unfortunately IStreamReader<T>::StreamType as constructor parameter doesn't work
//    explicit IStreamBinaryReader(IStreamReader<T>::StreamType &stream = std::cin):
    explicit IStreamBinaryReader(std::basic_istream<char> &stream = std::cin):
        IStreamReader<T>(stream)
    {}
    explicit IStreamBinaryReader(const char *fileName):
        IStreamReader<T>(fileName, std::ios_base::in | std::ios_base::binary)
    {}

    T next()
    {
        this->stream().read(buffer, sizeof(T));
        return *((T*)buffer);
    }

private:
    char buffer[sizeof(T)];
};

#endif // ISTREAMBINARYREADER_H
