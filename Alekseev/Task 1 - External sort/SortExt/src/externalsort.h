#ifndef EXTERNALSORT_H
#define EXTERNALSORT_H

#include <functional>

#include "istreamreader.h"
#include "ostreamwriter.h"
#include "fstreamqueue.h"
#include "stdsorter.h"

template<typename DataT,      // type of the data to be sorted
         class Comparator   = std::less<DataT>,       // should have a bool operator()(DataT&) method
         class InputReader  = IStreamReader<DataT>,   // should have a bool operator()(DataT&) method
         class OutputWriter = OStreamWriter<DataT>,   // should have an operator()(DataT) method
         class TempQueue    = FStreamQueue<DataT>,    // should have push(DataT) and bool pop(Data&) methods.
                                                      // may be one-go, i.e. it's guaranteed that push
                                                      // operation will not be called after pop operation
         class LocalSorter  = StdSorter<DataT, Comparator> // should have an operator()(Iterator begin, Iterator end) method
         >
void externalSort(InputReader &inputReader, OutputReader &outputReader);

#endif // EXTERNALSORT_H
