#ifndef FSCANFREADER_H
#define FSCANFREADER_H

#include <string>

#include "filecommunicator.h"
#include "abstractreader.h"

template <typename T>
class FscanfReader : FileCommunicator, AbstractReader<T>
{
public:
    FscanfReader(FILE *file, const char *fmt):
        FileCommunicator(file),
        _fmt(fmt)
    {}
    FscanfReader(const char *fileName, const char *fmt):
        FileCommunicator(fileName, "r"),
        _fmt(fmt)
    {}

    T next()
    {
        fscanf(file(), _fmt.c_str(), buffer);
        return *((T*)buffer);
    }
    bool hasNext()
    {
        bool ret = file() && !feof(file());
        if (ret)
        {
            char c = getc(file());
            if (c == EOF)
                ret = false;
            else
                ungetc(c, file());
        }
        return ret;
    }

    const std::string &format() const { return _fmt; }
    void setFormat(const char *format)
    {
        _fmt = format;
    }

private:
    std::string _fmt;

    char buffer[sizeof(T)];
};

#endif // FSCANFREADER_H
