#ifndef EXTERNALSORT_H
#define EXTERNALSORT_H

#include "istreamreader.h"
#include "ostreamwriter.h"
#include "fstreamqueue.h"
#include <functional>

template<typename DataT,      // type of the data to be sorted
         class InputReader  = IStreamReader<DataT>,   // should have a bool operator()(DataT&) method
         class OutputWriter = OStreamWriter<DataT>,   // should have an operator()(DataT) method
         class TempQueue    = FStreamQueue<DataT>,    // should have push(DataT) and bool pop(Data&) methods.
                                                      // it's guaranteed that push will not be called after pop
         class Comparator   = std::less<DataT>,       // should have a bool operator()(DataT&) method
         class LocalSorter    // should have an operator()(Iterator begin, Iterator end) method
         >
void externalSort(InputReader &inputReader, OutputReader &outputReader);

#endif // EXTERNALSORT_H
