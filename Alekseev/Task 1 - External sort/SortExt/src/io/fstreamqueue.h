#ifndef FSTREAMQUEUE_H
#define FSTREAMQUEUE_H

#include <cstdio>
#include <cassert>

#include <fstream>
#include <string>

#include "optimalstreamio.h"

template <typename DataT, class ReaderT, class WriterT>
class FStreamQueue
{
public:
    FStreamQueue(): reader(stream), writer(stream)
    {
        tmpnam(fileName);
        setState(Pushing);
    }
    ~FStreamQueue()
    {
        stream.close();
        remove(fileName);
    }

    void push(const DataT& data)
    {
        assert(state == Pushing);
        writer(data);
    }
    bool pop(DataT& data)
    {
        if (state != Popping)
            setState(Popping);
        return reader(data);
    }

protected:
    enum State
    {
        Pushing = 0,
        Popping = 1
    };

    void setState(State newState)
    {
        if (newState == Pushing)
        {
            stream.open(fileName, std::ios_base::out | std::ios_base::binary);
        }
        else if (newState == Popping)
        {
            stream.close();
            stream.open(fileName, std::ios_base::in | std::ios_base::binary);
        }
        state = newState;
    }

private:
    char fileName[L_tmpnam];
    State state;

    std::fstream stream;

    ReaderT reader;
    WriterT writer;
};

#endif // FSTREAMQUEUE_H
