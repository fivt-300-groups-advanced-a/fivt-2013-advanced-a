#ifndef SEGMENTTREE_H
#define SEGMENTTREE_H

#include "segmenttreeprivate.h"

/**
 * @brief The SegmentTree class
 *
 * @param Result
 *
 * @param MetaInformation
 *
 * @param MergeFunction should have ReturnType operator()(ReturnType left, ReturnType right)
 *
 * @param MetaUpdate should have operator()(MetaInformation &toUpdate, const MetaInformation &by,
 *                                          SizeType leftBound, SizeType rightBound)
 *
 * @param MetaApply should have operator()(Result &value, MetaInformation metaInfo,
 *                                          SizeType leftBound, SizeType rightBound)
 */
template <typename ResultT,
          typename MetaInformationT,
          class    MergeFunctionT,
          class    MetaUpdateT,
          class    MetaApplyT>
class SegmentTree
{
private:
    typedef SegmentTreePrivate<ResultT, MetaInformationT, MergeFunctionT, MetaUpdateT, MetaApplyT> _Private;

public:
    /**
     * @brief Result type storing return value of MergeFunction
     */
    typedef typename _Private::Result          Result;

    /**
     * @brief MetaInformation type storing meta information for lazy propagation
     */
    typedef typename _Private::MetaInformation MetaInformation;

    /**
     * @brief MergeFunction functor melding two ReturnType values
     */
    typedef typename _Private::MergeFunction   MergeFunction;

    /**
     * @brief MetaUpdate functor updating meta information of some segment
     */
    typedef typename _Private::MetaUpdate      MetaUpdate;

    /**
     * @brief MetaApply functor applying meta information on function result of some segment
     */
    typedef typename _Private::MetaApply       MetaApply;

    /**
     * @brief SizeType derived from std::vector
     */
    typedef typename _Private::SizeType        SizeType;

    /**
     * @brief SegmentTreePrivate constructor using std::vector<ResultType>(size, identity) to initialize
     * @param size
     * @param identity
     * @param func
     * @param metaUpdate
     * @param metaApply
     */
    SegmentTree(SizeType size, Result identity,
                MergeFunction func = MergeFunction(),
                MetaUpdate update = MetaUpdate(),
                MetaApply apply = MetaApply()):
        _private(size, identity, func, update, apply)
    {}

    /**
     * @brief SegmentTreePrivate constructor using [begin, end) sequence
     * @param begin FwdIterator pointing on the first element
     * @param end   FwdIterator pointing on the after-last element
     * @param identity
     * @param func
     * @param metaUpdate
     * @param metaApply
     */
    template<class FwdIterator>
    SegmentTree(FwdIterator begin, FwdIterator end, Result identity,
                MergeFunction func = MergeFunction(),
                MetaUpdate update = MetaUpdate(),
                MetaApply apply = MetaApply()):
        _private(begin, end, identity, func, update, apply)
    {}

    /**
     * @brief treeSizeFor calculates number of vertices required to handle queries on a sequence
     *        containing seqSize elements
     * @param seqSize
     * @return number of vertices
     */
    static SizeType treeSizeFor(SizeType seqSize)
    {
        return _Private::treeSizeFor(seqSize);
    }

    /**
     * @brief update updates metainfo on segment [l;r] with meta
     * @param l
     * @param r
     * @param meta
     */
    void update(SizeType l, SizeType r, MetaInformation meta)
    {
        return _private.update(l, r, meta);
    }

    /**
     * @brief get
     * @param l
     * @param r
     * @return result of mergeFunc applied on segment [l;r]
     */
    Result get(SizeType l, SizeType r) const
    {
        return _private.get(l, r);
    }

    _Private& _privateRef() const
    {
        return _private;
    }
    _Private& _privateRef()
    {
        return _private;
    }

private:
    _Private _private;
};

#endif // SEGMENTTREE_H

// балчуг 7
