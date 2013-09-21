#ifndef ABSTRACTREADER_H
#define ABSTRACTREADER_H

#include <iterator>

template<typename T>
class AbstractReader
{
public:
    virtual T next() = 0;
    virtual bool hasNext() = 0;
};

#endif // ABSTRACTREADER_H
