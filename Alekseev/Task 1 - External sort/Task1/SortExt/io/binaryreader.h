#ifndef BINARYREADER_H
#define BINARYREADER_H

#include <cassert>

#include "typedreader.h"

template<typename T>
class BinaryReader : public TypedReader<T>
{
public:
    BinaryReader():
        TypedReader<T>()
    {}
    BinaryReader(std::istream *stream):
        TypedReader<T>(stream)
    {}

    T next()
    {
        assert(this->stream);
        this->stream->read(buffer, sizeof(T));
        return *((T*)buffer);
    }

private:
    char buffer[sizeof (T)];
};

#endif // BINARYREADER_H
