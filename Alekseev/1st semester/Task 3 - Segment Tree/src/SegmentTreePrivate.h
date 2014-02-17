#ifndef SEGMENTTREEPRIVATE_H
#define SEGMENTTREEPRIVATE_H

#include <vector>

template <typename ResultT,
          typename MetaInformationT,
          class    MergeFunctionT,
          class    MetaUpdateT,
          class    MetaApplyT>
/**
 * @brief The SegmentTreePrivate class. see
 * http://code.google.com/p/fivt2013-advanced-a/source/browse/Ivaschenko/segmenttree/model/generalsegmenttree.h
 */
class SegmentTreePrivate
{
public:
    /**
     * @brief Result type storing return value of MergeFunction
     */
    typedef ResultT          Result;

    /**
     * @brief MetaInformation type storing meta information for lazy propagation
     */
    typedef MetaInformationT MetaInformation;

    /**
     * @brief MergeFunction functor melding two Result values
     */
    typedef MergeFunctionT   MergeFunction;

    /**
     * @brief MetaUpdate functor updating function value on some segment
     */
    typedef MetaUpdateT      MetaUpdate;

    /**
     * @brief MetaApply functor applying meta information on some
     */
    typedef MetaApplyT       MetaApply;

    /**
     * @brief SizeType derived from std::vector
     */
    typedef typename std::vector<Result>::size_type SizeType;

    /**
     * @brief SegmentTreePrivate constructor using std::vector<ResultType>(size, identity) to initialize
     * @param size
     * @param identity
     * @param func
     * @param metaUpdate
     * @param metaApply
     */
    SegmentTreePrivate(SizeType size, Result identity,
                       MergeFunction func, MetaUpdate metaUpdate, MetaApply metaApply):
        funcIdentity(identity), metaIdentity(MetaInformation()), mergeFunc(func), metaUpdate(metaUpdate), metaApply(metaApply)
    {
        std::vector<Result> dummy(size, identity);
        init(dummy.begin(), dummy.end());
    }

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
    SegmentTreePrivate(FwdIterator begin, FwdIterator end, Result identity,
                MergeFunction func, MetaUpdate metaUpdate, MetaApply metaApply):
        funcIdentity(identity), metaIdentity(MetaInformation()), mergeFunc(func), metaUpdate(metaUpdate), metaApply(metaApply)
    {
        init(begin, end);
    }

    /**
     * @brief treeSizeFor calculates number of vertices required to handle queries on a sequence
     *        containing seqSize elements
     * @param seqSize
     * @return number of vertices
     */
    static SizeType treeSizeFor(SizeType seqSize)
    {
        SizeType ret = 1;
        while (ret < seqSize)
            ret <<= 1;
        ret <<= 1;
        return ret;
    }

    /**
     * @brief update updates metainfo on segment [l;r] with meta
     * @param l
     * @param r
     * @param meta
     */
    void update(SizeType l, SizeType r, const MetaInformation &meta)
    {
        if (r < 0 || l >= seqSize || r < l)
            throw "sequence range exceeded";
        return update(0, 0, halfSize-1, l, r, meta);
    }

    /**
     * @brief get
     * @param l
     * @param r
     * @return result of mergeFunc applied on segment [l;r]
     */
    Result get(SizeType l, SizeType r) const
    {
        if (r < 0 || l >= seqSize || r < l)
            throw "sequence range exceeded";
        return get(0, 0, halfSize-1, l, r);
    }

//private:
//    friend class SegmentTree<Result, MetaInformation, MergeFunction, MetaUpdate, MetaApply>;

    /**
     * @brief middle index of middle element of segment
     * @param left index of left segment bound, inclusive
     * @param right index if right segment bound, exclusive
     * @return index of the middle element of the segment. returns left if (left+1==right)
     */
    static SizeType middle(SizeType left, SizeType right)
    {
        return (left + right) >> 1;
    }
    /**
     * @brief leftSon
     * @param parent
     * @return index of left son of for given parent index
     */
    static SizeType leftSon(SizeType parent)
    {
        return (parent << 1) + 1;
    }
    /**
     * @brief rightSon
     * @param parent
     * @return index of left son of for given parent index
     */
    static SizeType rightSon(SizeType parent)
    {
        return leftSon(parent) + 1;
    }

    /**
     * @brief init initializes tree using given sequence to initialize underlying array
     * @param begin
     * @param end
     */
    template<class FwdIterator>
    void init(FwdIterator begin, FwdIterator end)
    {
        seqSize = std::distance(begin, end);
        treeSize = treeSizeFor(seqSize);
        halfSize = treeSize >> 1;

        tree.reserve(treeSize);
        tree.resize(treeSize, {funcIdentity, MetaInformation()});

        for (auto it = tree.begin() + halfSize-1; begin != end; ++begin, ++it)
            it->value = *begin;
        for (SizeType i = halfSize - 2; i < halfSize; --i)
            tree[i].value = mergeFunc(tree[leftSon(i)].value, tree[rightSon(i)].value);
    }

    /**
     * @brief push updates return value at root vertex and propagates its meta info to its children in case they exist
     * @param root index of the root vertex
     * @param lb index of left bound of the segment root is responsible for
     * @param rb index of right bound of the segment root is responsible for
     */
    void push(SizeType root, SizeType lb, SizeType rb) const
    {
        metaApply(tree[root].value, tree[root].meta, lb, rb);
        if (lb != rb)
        {
            SizeType mid = middle(lb, rb);
            metaUpdate(tree[leftSon(root)].meta, tree[root].meta, lb, mid);
            metaUpdate(tree[rightSon(root)].meta, tree[root].meta, mid+1, rb);
        }
        tree[root].meta = metaIdentity;
    }

    /**
     * @brief update updates metainfo on segment [tl;tr]
     * @param root index of top vertex responsible for segment[l,r]
     * @param l index of the left bound(inclusive) of the segment root is responsible for
     * @param r index of the right bound(exclusive) of the segment root is responsible for
     * @param tl index of the left bound(inclusive) of the query segment
     * @param tr index of the right bound(exclusive) of the query segment
     * @param meta information to pass to MetaUpdate
     */
    void update(SizeType root, SizeType l, SizeType r, SizeType tl, SizeType tr, const MetaInformation &meta)
    {
        push(root, l, r);
        if (l > tr || r < tl)
            return;
        if (l >= tl && r <= tr)
        {
            tree[root].meta = meta;
            push(root, l, r);
            return;
        }
        SizeType mid = middle(l, r);
        update(leftSon(root), l, mid, tl, tr, meta);
        update(rightSon(root), mid+1, r, tl, tr, meta);
        tree[root].value = mergeFunc(tree[leftSon(root)].value, tree[rightSon(root)].value);
    }

    /**
     * @brief get
     * @param root index of top vertex responsible for segment[l,r]
     * @param l index of the left bound(inclusive) of the segment root is responsible for
     * @param r index of the right bound(exclusive) of the segment root is responsible for
     * @param tl index of the left bound(inclusive) of the query segment
     * @param tr index of the right bound(exclusive) of the query segment
     * @return result of mergeFunc applied on segment [tl;dr]
     */
    Result get(SizeType root, SizeType l, SizeType r, SizeType tl, SizeType tr) const
    {
        push(root, l, r);
        if (l > tr || r < tl)
            return funcIdentity;
        if (l >= tl && r <= tr)
            return tree[root].value;
        SizeType mid = middle(l, r);
        return mergeFunc(get(leftSon(root), l, mid, tl, tr), get(rightSon(root), mid+1, r, tl, tr));
    }

    /**
     * @brief The Vertex struct stores together function return value and meta information for some segment
     */
    struct Vertex
    {
        Result value;
        MetaInformation meta;
    };

    SizeType seqSize;
    SizeType treeSize;
    SizeType halfSize;

    mutable std::vector<Vertex> tree;

    const Result funcIdentity;
    const MetaInformation metaIdentity;

    mutable MergeFunction mergeFunc;
    mutable MetaUpdate metaUpdate;
    mutable MetaApply metaApply;
};

#endif // SEGMENTTREEPRIVATE_H
