#ifndef ABSTRACTWRITER_H
#define ABSTRACTWRITER_H

template <typename T>
class AbstractWriter
{
public:
    virtual void put(const T &some) = 0;
};

#endif // ABSTRACTWRITER_H
