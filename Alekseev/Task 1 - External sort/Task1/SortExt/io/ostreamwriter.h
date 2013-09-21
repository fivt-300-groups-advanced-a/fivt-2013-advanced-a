#ifndef OSTREAMWRITER_H
#define OSTREAMWRITER_H

#include <iostream>
#include <fstream>

#include "abstractwriter.h"

template <typename T, typename CharT = char, class TraitsT = std::char_traits<CharT> >
class OStreamWriter : AbstractWriter<T>
{
public:
    typedef T DataType;
    typedef CharT CharType;
    typedef TraitsT TraitsType;
    typedef std::basic_ostream<CharType, TraitsType> StreamType;
    typedef std::basic_string<CharType, TraitsType> StringType;

    explicit OStreamWriter(StreamType &stream = std::cout, const StringType &suffix = ""):
        _stream(&stream),
        _ownStream(false),
        _suffix(suffix)
    {}
    explicit OStreamWriter(const char *fileName, std::ios_base::openmode mode = std::ios_base::out, const std::string *suffix = ""):
        _stream(new std::ofstream(fileName, mode)),
        _ownStream(true)
    {}
    ~OStreamWriter()
    {
        clearStream();
    }

    StreamType &stream() const { return *_stream; }
    void setStream(StreamType &s)
    {
        clearStream();
        _stream = &s;
        _ownStream = false;
    }

    const StringType &suffix() const { return suffix; }
    void setSuffix(const StringType &suffix)
    {
        _suffix = suffix;
    }

    virtual void put(const T &some)
    {
        *_stream << some << _suffix;
    }

protected:
    void clearStream()
    {
        if (_ownStream)
        {
            delete _stream;
            _stream = 0;
            _ownStream = false;
        }
    }

    void setOwnStream(StreamType *stream)
    {
        clearStream();
        _stream = stream;
        _ownStream = true;
    }

private:
    StreamType * _stream;
    bool _ownStream;
    StringType _suffix;
};

#endif // OSTREAMWRITER_H
