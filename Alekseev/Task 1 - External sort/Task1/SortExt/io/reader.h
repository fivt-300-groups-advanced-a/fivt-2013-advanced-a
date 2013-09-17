#ifndef READER_H
#define READER_H

#include <cassert>

#include "typedreader.h"

template<typename T>
class Reader : public TypedReader<T>
{
public:
    Reader():
        TypedReader<T>()
    {}
    Reader(std::istream *stream):
        TypedReader<T>(stream)
    {}

    T next()
    {
        assert(this->stream);

        T result;
        (*this->stream) >> result;
        return result;
    }
};

#endif // READER_H
