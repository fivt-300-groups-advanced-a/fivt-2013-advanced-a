#ifndef OPTIMALSTREAMIO_H
#define OPTIMALSTREAMIO_H

#include <type_traits>

#include "istreamreader.h"
#include "ostreamwriter.h"

#include "rawistreamreader.h"
#include "rawostreamwriter.h"

template<typename DataT> struct IsRawWritable :
        std::integral_constant<bool, std::is_integral<DataT>::value || std::is_floating_point<DataT>::value> {};
//        std::integral_constant<bool, std::is_arithmetic<typename std::remove_all_extents<DataT>::type >::value> {};

template<typename DataT> struct OptimalStreamIO
{
    typedef typename std::conditional<
                                      IsRawWritable<DataT>::value,
                                      RawIStreamReader<DataT>,
                                      IStreamReader<DataT>
                     >::type ReaderType;
    typedef typename std::conditional<
                                      IsRawWritable<DataT>::value,
                                      RawOStreamWriter<DataT>,
                                      OStreamWriter<DataT>
                     >::type WriterType;
};

#endif // OPTIMALSTREAMIO_H
