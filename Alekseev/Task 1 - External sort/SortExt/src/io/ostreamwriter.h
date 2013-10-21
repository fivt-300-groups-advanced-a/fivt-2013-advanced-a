#ifndef OSTREAMWRITER_H
#define OSTREAMWRITER_H

#include <ios>
#include <string>

// TODO: configurable suffix

template <typename DataT>
class OStreamWriter
{
public:
    typedef DataT DataType;
    typedef std::ostream Stream;

    OStreamWriter(std::ostream &stream):
        stream(&stream)
    {}

    void operator() (const DataType &data)
    {
        *stream << data << suffix();
    }

private:
    std::string suffix() const
    {
        return std::string();
    }

    Stream *stream;
};

template<> inline std::string OStreamWriter<int>::suffix() const { return " "; }
template<> inline std::string OStreamWriter<std::string>::suffix() const { return "\n"; }

#endif // OSTREAMWRITER_H
