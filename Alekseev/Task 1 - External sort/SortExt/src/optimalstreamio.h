#ifndef OPTIMALSTREAMIO_H
#define OPTIMALSTREAMIO_H

#include "istreamreader.h"
#include "ostreamwriter.h"

template<typename DataT>
struct OptimalStreamIO
{
    typedef IStreamReader<DataT> ReaderType;
    typedef OStreamWriter<DataT> WriterType;
};

#endif // OPTIMALSTREAMIO_H
