#ifndef BINARYENCODER_H
#define BINARYENCODER_H

#include "abstractwriter.h"

template <typename T>
class BinaryEncoder : public AbstractWriter<T>
{
public:
    explicit BinaryEncoder(AbstractWriter<char> &writer):
        writer(writer)
    {}

    void put(const T &some)
    {
        for (unsigned int i = 0; i < sizeof(T); ++i)
            writer->put((char*)(&some)[i]);
    }

private:
    AbstractWriter<char> *writer;
};

#endif // BINARYENCODER_H
