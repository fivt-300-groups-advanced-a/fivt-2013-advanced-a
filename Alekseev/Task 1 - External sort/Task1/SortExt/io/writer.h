#ifndef WRITER_H
#define WRITER_H

#include <cassert>

#include "typedwriter.h"

template<typename T>
class Writer : public TypedWriter<T>
{
public:
    Writer():
        TypedWriter<T>()
    {}
    Writer(std::ostream *stream):
        TypedWriter<T>(stream)
    {}

    void put(const T &x)
    {
        assert(this->stream);
        *(this->stream) << x;
    }
};

#endif // WRITER_H
