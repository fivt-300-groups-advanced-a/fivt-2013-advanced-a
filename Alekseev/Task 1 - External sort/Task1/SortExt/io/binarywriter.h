#ifndef BINARYWRITER_H
#define BINARYWRITER_H

#include <cassert>

#include "typedwriter.h"

template<typename T>
class BinaryWriter : public TypedWriter<T>
{
public:
    BinaryWriter():
        TypedWriter<T>()
    {}
    BinaryWriter(std::ostream *stream):
        TypedWriter<T>(stream)
    {}

    void put(const T &x)
    {
        assert(this->stream);
        this->stream->write((char*)&x, sizeof(T));
    }
};

#endif // BINARYWRITER_H
