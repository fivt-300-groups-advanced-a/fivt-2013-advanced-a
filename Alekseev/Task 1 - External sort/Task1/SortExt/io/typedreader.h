#ifndef TYPEDREADER_H
#define TYPEDREADER_H

#include <iostream>

template<typename T>
class TypedReader
{
public:
    TypedReader():
        stream(0)
    {}

    explicit TypedReader(std::istream *stream)
    {
        setIStream(stream);
    }

    virtual T next() = 0;

    virtual bool canRead()
    {
        return stream && *stream;
    }

    virtual void setIStream(std::istream *stream)
    {
        this->stream = stream;
    }

    std::istream * getIStream()
    {
        return stream;
    }

protected:
    std::istream *stream;
};

#endif // TYPEDREADER_H
