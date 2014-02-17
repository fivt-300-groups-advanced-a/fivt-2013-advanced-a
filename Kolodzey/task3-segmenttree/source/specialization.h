namespace segmentTree{

struct ResultType_llsum
{
  ResultType_llsum():_l(-1), _r(-1), _sum(0) {}
  ResultType_llsum(size_t l, size_t r, long long sum):_l(l), _r(r), _sum(sum) {}

  size_t _l;
  size_t _r;
  long long _sum;
};

struct ResultMerger_llsum
{
  ResultType_llsum operator()(ResultType_llsum left, ResultType_llsum right)
  {
  return ResultType_llsum(left._l, right._r, left._sum + right._sum);
  }
};

struct MetaToResultApplier_llsum_add
{
  ResultType_llsum operator()(long long meta, ResultType_llsum res)
  {
    return ResultType_llsum(res._l, res._r, res._sum + meta * (res._r - res._l + 1));
  }
};

struct MetaToResultApplier_llsum_assign
{
  ResultType_llsum operator()(long long meta, ResultType_llsum res)
  {
    return ResultType_llsum(res._l, res._r, meta * (res._r - res._l + 1));
  }
};

struct MetaMerger_llsum_add
{
  long long operator()(long long oldmeta, long long newmeta)
  {
    return oldmeta + newmeta;
  }
};

struct MetaMerger_llsum_assign
{
  long long operator()(long long oldmeta, long long newmeta)
  {
    return newmeta;
  }
};

//todo: define user-friendly llsum with user-friendly build without l r
typedef segmentTree::GeneralTree <ResultType_llsum, long long,
                                  ResultMerger_llsum,
                                  MetaMerger_llsum_add,
                                  MetaToResultApplier_llsum_add> llsum_add;

typedef segmentTree::GeneralTree <ResultType_llsum, long long,
                                  ResultMerger_llsum,
                                  MetaMerger_llsum_assign,
                                  MetaToResultApplier_llsum_assign> llsum_assign;                               
}