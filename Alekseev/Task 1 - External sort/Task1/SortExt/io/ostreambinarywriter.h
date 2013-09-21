#ifndef OSTREAMBINARYWRITER_H
#define OSTREAMBINARYWRITER_H

#include "ostreamwriter.h"

template <typename T>
class OStreamBinaryWriter : public OStreamWriter<T>
{
public:
    explicit OStreamBinaryWriter(std::basic_ostream<char> &stream = std::cout):
        OStreamWriter<T>(stream)
    {}
    explicit OStreamBinaryWriter(const char *fileName):
        OStreamWriter<T>(fileName, std::ios_base::out | std::ios_base::binary)
    {}

    void put(const T &some)
    {
        stream().write((char*)&some, sizeof(T));
    }
};


#endif // OSTREAMBINARYWRITER_H
