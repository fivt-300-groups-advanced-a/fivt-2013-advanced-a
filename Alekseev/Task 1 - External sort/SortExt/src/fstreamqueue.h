#ifndef FSTREAMQUEUE_H
#define FSTREAMQUEUE_H

#include <cstdio>
#include <cassert>

#include <fstream>
#include <string>

#include "optimalstreamio.h"

template <typename DataT,
          class ReaderT = typename OptimalStreamIO<DataT>::ReaderType,
          class WriterT = typename OptimalStreamIO<DataT>::WriterType >
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
            stream.open(fileName, std::ios_base::app | std::ios_base::out | std::ios_base::binary);
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
