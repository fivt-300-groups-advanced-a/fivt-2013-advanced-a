#ifndef STREAMCOMMUNICATOR_H
#define STREAMCOMMUNICATOR_H

#include <iostream>

template <class StreamType>
class StreamCommunicator
{
public:
    explicit StreamCommunicator(StreamType &stream = std::cin):
        _stream(&stream),
        _ownStream(false)
    {}
    explicit StreamCommunicator(const char *fileName, std::ios_base::openmode mode = std::ios_base::in):
        _stream(new std::ifstream(fileName, mode)),
        _ownStream(true)
    {}
    ~StreamCommunicator()
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

#endif // STREAMCOMMUNICATOR_H
