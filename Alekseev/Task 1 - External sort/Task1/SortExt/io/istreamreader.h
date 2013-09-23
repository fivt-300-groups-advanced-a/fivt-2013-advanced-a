#ifndef ISTREAMREADER_H
#define ISTREAMREADER_H

#include <iostream>
#include <fstream>

#include "abstractreader.h"
#include "streamcommunicator.h"

template <typename T, typename CharT = char, class TraitsT = std::char_traits<CharT> >
class IStreamReader : public AbstractReader<T>, public StreamCommunicator<std::basic_istream<CharT, TraitsT> >
{
public:
    typedef std::basic_istream<CharT, TraitsT> StreamType;
    explicit IStreamReader(StreamType &stream):
        StreamCommunicator< StreamType >(stream)
    {}
    explicit IStreamReader(const char *fileName, std::ios_base::openmode mode = std::ios_base::in):
        StreamCommunicator< StreamType >(fileName, mode)
    {}

    virtual T next()
    {
        T result;
        this->stream() >> result;
        return result;
    }
    virtual bool hasNext()
    {
        return this->_stream && this->stream();
    }
};

#endif // ISTREAMREADER_H
