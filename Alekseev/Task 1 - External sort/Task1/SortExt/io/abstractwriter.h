#ifndef ABSTRACTWRITER_H
#define ABSTRACTWRITER_H

template <typename T>
class AbstractWriter
{
public:
    typedef T DataType;

    virtual void put(const T &some) = 0;

    virtual void flush() {}
};

#endif // ABSTRACTWRITER_H
