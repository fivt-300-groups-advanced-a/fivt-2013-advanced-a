#ifndef RAWISTREAMREADER_H
#define RAWISTREAMREADER_H

#include <ios>

template <typename DataT>
class RawIStreamReader
{
public:
    typedef DataT DataType;
    typedef std::istream Stream;

    static const std::size_t DataSize = sizeof(DataType);

    RawIStreamReader(Stream &stream):
        stream(&stream)
    {
    }

    bool operator() (DataType &d)
    {
        if (!*stream)
            return false;
        stream->read(reinterpret_cast<char*>(&d), DataSize);
        return *stream;
    }

private:
    Stream *stream;
};

#endif // RAWISTREAMREADER_H
