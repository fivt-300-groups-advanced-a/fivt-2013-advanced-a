#ifndef SEGMENTADDITIONTREE_H
#define SEGMENTADDITIONTREE_H

#include <functional>

#include "segmenttree.h"

template<typename DataT, class Compare = std::less<DataT> >//, class Plus = std::plus<DataT, DataT> >
class SegmentAdditionTree
{
public:

    SegmentAdditionTree(std::size_t size,
                        const DataT &negInf, const DataT &posInf, const DataT &zero,
                        Compare cmp = std::less<DataT>()):
        tree(size, Result(posInf, negInf, zero), Function(cmp)),
        zero(zero)
    {}

    template<typename FwdIt>
    SegmentAdditionTree(FwdIt begin, FwdIt end,
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

    void add(std::size_t l, std::size_t r, DataT value)
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
        explicit Meta(DataT value = DataT()):
            value(value)
        {}

        DataT value;
    };

    struct Update
    {
        void operator()(Meta &toUpdate, Meta by, std::size_t, std::size_t)
        {
            toUpdate.value = toUpdate.value + by.value;
        }
    };

    struct Apply
    {
        void operator()(Result &res, Meta meta, std::size_t l, std::size_t r)
        {
            res.sum = res.sum + meta.value * (r-l+1);
            res.min = res.min + meta.value;
            res.max = res.max + meta.value;
        }
    };

    SegmentTree<Result, Meta, Function, Update, Apply> tree;
    DataT zero;
};

#endif // SEGMENTADDITIONTREE_H
