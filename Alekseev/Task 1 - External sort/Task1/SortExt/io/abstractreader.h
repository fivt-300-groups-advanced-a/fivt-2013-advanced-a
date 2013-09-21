#ifndef ABSTRACTREADER_H
#define ABSTRACTREADER_H

template <typename T>
class AbstractReader
{
public:
    typedef T DataType;

    virtual T next() = 0;
    virtual bool hasNext() = 0;
};

#endif // ABSTRACTREADER_H
