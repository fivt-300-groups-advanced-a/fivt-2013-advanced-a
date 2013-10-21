#ifndef RAWOSTREAMWRITER_H
#define RAWOSTREAMWRITER_H

#include <ios>

template <typename DataT>
class RawOStreamWriter
{
public:
    typedef DataT DataType;
    typedef std::ostream Stream;

    static const std::size_t DataSize = sizeof(DataType);

    RawOStreamWriter(std::ostream &stream):
        stream(&stream)
    {}

    void operator() (const DataType &data)
    {
        stream->write(reinterpret_cast<const char*>(&data), DataSize);
    }

private:
    Stream *stream;
};

#endif // RAWOSTREAMWRITER_H
