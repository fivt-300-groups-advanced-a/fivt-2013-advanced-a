#ifndef ISTREAMREADER_H
#define ISTREAMREADER_H

#include <iostream>
#include <fstream>

#include "streamcommunicator.h"

template <typename T, class StreamT = std::istream >
class IStreamReader : public StreamCommunicator< StreamT >
{
public:
    typedef StreamT StreamType;

    explicit IStreamReader(StreamType &stream):
        StreamCommunicator< StreamType >(stream)
    {
    }
    explicit IStreamReader(const char *fileName, std::ios_base::openmode mode = std::ios_base::in):
        StreamCommunicator< StreamType >(fileName, mode)
    {
    }

    bool operator() (T &result)
    {
        if (!this->stream())
            return false;
        this->stream() >> result;
        return this->stream();
    }
};

#endif // ISTREAMREADER_H
