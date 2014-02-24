#ifndef ISTREAMREADER_H
#define ISTREAMREADER_H

#include <ios>

template <typename DataT>
class IStreamReader
{
public:
    typedef DataT DataType;
    typedef std::istream Stream;

    /*implicit*/ IStreamReader(std::istream &stream):
        stream(&stream)
    {}

    bool operator() (DataType &data)
    {
        if (!*stream)
            return false;
        *stream >> data;
        return *stream;
    }

private:
    Stream *stream;
};

#endif // ISTREAMREADER_H
