#ifndef BINARYDECODER_H
#define BINARYDECODER_H

#include "abstractreader.h"

template <typename T>
class BinaryDecoder : AbstractReader<T>
{
public:
    // :NOTE: if the reader uses istream, it probably should have noskipws manipulator
    explicit BinaryDecoder(AbstractReader<char> &reader):
        reader(reader)
    {}

    T next()
    {
        for (unsigned int i = 0; i < sizeof(T); ++i)
            buffer[i] = reader->next();
        return *((T*)buffer);
    }
    bool hasNext()
    {
        return reader->hasNext();
    }
private:
    AbstractReader *reader;
    char buffer[sizeof(T)];
};

#endif // BINARYDECODER_H
