#ifndef TYPEDWRITER_H
#define TYPEDWRITER_H

#include <iostream>

template<typename T>
class TypedWriter
{
public:
    TypedWriter():
        stream(0)
    {}

    explicit TypedWriter(std::ostream *stream)
    {
        setOStream(stream);
    }

    virtual void put(const T &x) = 0;

    virtual void setOStream(std::ostream *stream)
    {
        this->stream = stream;
    }

    std::ostream * getOStream()
    {
        return stream;
    }

protected:
    std::ostream *stream;
};

#endif // TYPEDWRITER_H
