#ifndef SEGMENTASSIGNMENTTREE_H
#define SEGMENTASSIGNMENTTREE_H

#include <functional>

#include "segmenttree.h"

template<typename DataT, class Compare = std::less<DataT> >//, class Plus = std::plus<DataT, DataT> >
class SegmentAssignmentTree
{
public:

    SegmentAssignmentTree(std::size_t size,
                        const DataT &negInf, const DataT &posInf, const DataT &zero,
                        Compare cmp = std::less<DataT>()):
        tree(size, Result(posInf, negInf, zero), Function(cmp)),
        zero(zero)
    {}

    template<typename FwdIt>
    SegmentAssignmentTree(FwdIt begin, FwdIt end,
                        const DataT &negInf, const DataT &posInf, const DataT &zero,
                        Compare cmp = std::less<DataT>()):
        tree(begin, end, Result(posInf, negInf, zero), Function(cmp)),
        zero(zero)
    {}

    struct Result
    {
        /*implicit*/ Result(DataT value):
            min(value), max(value), sum(value)
        {}
        Result(DataT min, DataT max, DataT sum):
            min(min), max(max), sum(sum)
        {}
        DataT min, max, sum;
    };

    Result get(std::size_t l, std::size_t r)
    {
        return tree.get(l, r);
    }

    void assign(std::size_t l, std::size_t r, DataT value)
    {
        tree.update(l, r, Meta(value));
    }

private:
    struct Function
    {
        explicit Function(Compare less):
            less(less)
        {}
        Result operator() (const Result &a, const Result &b)
        {
            return Result(std::min(a.min, b.min, less), std::max(a.max, b.max, less), a.sum + b.sum);
        }
    private:
        Compare less;
    };

    struct Meta
    {
        Meta():
            value(), hasAssignment(false)
        {}

        explicit Meta(DataT value):
            value(value), hasAssignment(true)
        {}

        DataT value;
        bool hasAssignment;
    };

    struct Update
    {
        void operator()(Meta &toUpdate, Meta by, std::size_t, std::size_t)
        {
            if (by.hasAssignment)
            {
                toUpdate.value = by.value;
                toUpdate.hasAssignment = true;
            }
        }
    };

    struct Apply
    {
        void operator()(Result &res, Meta meta, std::size_t l, std::size_t r)
        {
            if (meta.hasAssignment)
            {
                res.max = meta.value;
                res.min = meta.value;
                res.sum = meta.value * (r - l + 1);
            }
        }
    };

    SegmentTree<Result, Meta, Function, Update, Apply> tree;
    DataT zero;
};

#endif // SEGMENTASSIGNMENTTREE_H
