#ifndef ISTREAMREADER_H
#define ISTREAMREADER_H

#include <iostream>
#include <fstream>

#include "abstractreader.h"

template <typename T, typename CharT = char, class TraitsT = std::char_traits<CharT> >
class IStreamReader : public AbstractReader<T>
{
public:
    typedef CharT CharType;
    typedef TraitsT TraitsType;
    typedef std::basic_istream<CharType, TraitsType> StreamType;

    explicit IStreamReader(StreamType &stream = std::cin):
        _stream(&stream),
        _ownStream(false)
    {}
    explicit IStreamReader(const char *fileName, std::ios_base::openmode mode = std::ios_base::in):
        _stream(new std::ifstream(fileName, mode)),
        _ownStream(true)
    {}
    ~IStreamReader()
    {
        closeOwnStream();
    }

    StreamType &stream() const { return *_stream; }
    void setStream(StreamType &s)
    {
        closeOwnStream();
        _stream = &s;
        _ownStream = false;
    }

    virtual T next()
    {
        T result;
        *_stream >> result;
        return result;
    }
    virtual bool hasNext()
    {
        return _stream && *_stream;
    }

    void closeOwnStream()
    {
        if (_ownStream)
        {
            delete _stream;
            _stream = 0;
            _ownStream = false;
        }
    }
protected:
    void setOwnStream(StreamType *stream)
    {
        closeOwnStream();
        _stream = stream;
        _ownStream = true;
    }

private:
    StreamType * _stream;
    bool _ownStream;
};

#endif // ISTREAMREADER_H
