#ifndef OSTREAMWRITER_H
#define OSTREAMWRITER_H

#include <iostream>
#include <fstream>

#include "abstractwriter.h"
#include "streamcommunicator.h"

template <typename T, typename CharT = char, class TraitsT = std::char_traits<CharT> >
class OStreamWriter : public AbstractWriter<T>, public StreamCommunicator<std::basic_ostream<CharT, TraitsT> >
{
public:
    typedef std::basic_ostream<CharT, TraitsT> StreamType;
    typedef std::basic_string<CharT, TraitsT> StringType;

    explicit OStreamWriter(StreamType &stream, const StringType &suffix = "\n"):
        StreamCommunicator< StreamType >(stream),
        _suffix(suffix)
    {}
    explicit OStreamWriter(const char *fileName, std::ios_base::openmode mode = std::ios_base::out, const std::string *suffix = "\n"):
        StreamCommunicator< StreamType >(fileName, mode),
        _suffix(suffix)
    {}

    const StringType &suffix() const { return _suffix; }
    void setSuffix(const StringType &suffix)
    {
        _suffix = suffix;
    }

    virtual void put(const T &some)
    {
        this->stream() << some << _suffix;
    }
    void flush()
    {
        this->stream().flush();
    }

private:
    StringType _suffix;
};

#endif // OSTREAMWRITER_H
