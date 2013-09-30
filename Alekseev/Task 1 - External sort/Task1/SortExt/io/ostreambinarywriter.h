#ifndef OSTREAMBINARYWRITER_H
#define OSTREAMBINARYWRITER_H

//#include "abstractwriter.h"
#include "streamcommunicator.h"

template <typename T>
class OStreamBinaryWriter : public AbstractWriter<T>, public StreamCommunicator<std::ostream>
{
public:
    explicit OStreamBinaryWriter(std::basic_ostream<char> &stream):
        StreamCommunicator<std::ostream>(stream)
    {}
    explicit OStreamBinaryWriter(const char *fileName):
        StreamCommunicator<std::ostream>(fileName, std::ios_base::out | std::ios_base::binary)
    {}

    void put(const T &some) override
    {
        stream().write(reinterpret_cast<const char*>(&some), sizeof(T));
    }
    void flush() override
    {
        stream().flush();
    }
};


#endif // OSTREAMBINARYWRITER_H
