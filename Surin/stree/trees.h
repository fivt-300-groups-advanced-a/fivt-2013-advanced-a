#ifndef TREES_H
#include "stree.h"
#include "operations.h"

template<class T, T inf>
using AssMinTree = SegmentTree<T, Compare<T, inf, Less<T> >, SAssignPush<T> >;

template<class T, T inf>
using AssMaxTree = SegmentTree<T, Compare<T, inf, InvComp<T, Less<T> > >, SAssignPush<T> >;

template<class T, T zero>
using AssSumTree = SegmentTree<T, Sum<T, zero>, AssignPush<T> >;

template<class T, T zero, T inf>
using AddMinTree = SegmentTree<T, Compare<T, inf, Less<T> >, SAddPush<T, zero, Plus<T>> >;
 
template<class T, T zero, T minf>
using AddMaxTree = SegmentTree<T, Compare<T, minf, InvComp<T, Less<T> > >, SAddPush<T, zero, Plus<T> > >;

template<class T, T zero>
using AddSumTree = SegmentTree<T, Sum<T, zero>, AddPush<T, zero, Plus<T> > >;

#endif
