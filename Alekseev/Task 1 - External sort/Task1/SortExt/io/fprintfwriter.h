#ifndef FPRINTFWRITER_H
#define FPRINTFWRITER_H

#include <string>

#include "abstractwriter.h"
#include "filecommunicator.h"

template <typename T>
class FprintfWriter : FileCommunicator, AbstractWriter<T>
{
public:
    FprintfWriter(FILE *file, const char *fmt):
        FileCommunicator(file),
        _fmt(fmt)
    {}
    FprintfWriter(const char *fileName, const char *fmt):
        FileCommunicator(fileName, "w"),
        _fmt(fmt)
    {}

    void put(const T &some)
    {
        fprintf(file(), _fmt.c_str(), some);
    }
    void flush()
    {
        fflush(file());
    }

    const std::string &format() const { return _fmt; }
    void setFormat(const char *format)
    {
        _fmt = format;
    }

private:
    std::string _fmt;
};

#endif // FPRINTFWRITER_H
