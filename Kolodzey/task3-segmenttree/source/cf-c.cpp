#include <vector>
#include <memory>
#include <iostream>
#include <cstdlib>
#include <cstdio>

template <class T> class TestAccess;

namespace segmentTree {

int N_INNER_GET = 0;
int N_INNER_MODIFY = 0;

size_t sizeOfTree(size_t length)
{
  if (length == 0)
  {
    std::cerr << "counting size of empty tree";
    exit(EXIT_FAILURE);
  }
  size_t ans = 1;
  while (ans < length)
    ans<<=1;
  ans <<=1;
  return (ans - 1);
}

size_t lengthOfLowerLevel(size_t treeSize)
{
  if (treeSize == 1)
    return 1;
  return (treeSize / 2) + 1;
}

size_t nMetaVertexes(size_t length)
{
  return sizeOfTree(length) / 2;
}

size_t lastMetaVertexIndex(size_t treeSize)
{
  if (treeSize == 0)
  {
    std::cerr << "not possible to find lastMetaVertexIndex in an empty tree";
    exit(EXIT_FAILURE);
  }
  //should we check that treeSize 2^n - 1?

  return ((treeSize / 2) - 1);
}


inline size_t leftchildInd(size_t ind)
{
  return ind * 2 + 1;
}

inline size_t rightchildInd(size_t ind)
{
  return ind * 2 + 2;
}

size_t parent(size_t ind)
{
  if (ind == 0)
  {
    std::cerr << "trying to find parent of root";
    exit(EXIT_FAILURE);
  }
  return (ind - 1) / 2;
}

size_t countLeftSonR(size_t parentl, size_t parentr)
{
  return (parentl + parentr) / 2;
}

size_t countRightSonL(size_t parentl, size_t parentr)
{
  return (parentl + parentr + 1) / 2;
}

template <class ResultType,
          class MetaType,
          class ResultMerger,
          class MetaMerger,
          class MetaToResultApplier>
class GeneralTree
{
public:
  GeneralTree(): _ptr_to_resultMerger(nullptr),
                 _ptr_to_metaMerger(nullptr),
                 _ptr_to_metaToResultApplier(nullptr),
                 _data(std::vector<std::pair<ResultType,
                                       std::unique_ptr<MetaType>>>()),
                 _length(0){} 

  GeneralTree(const GeneralTree& x): _ptr_to_resultMerger(nullptr),
                                        _ptr_to_metaMerger(nullptr),
                                        _ptr_to_metaToResultApplier(nullptr),
                                        _data(),
                                        _length(x._length)
  {
    if ((x._ptr_to_resultMerger.get() != nullptr) &&
        (x._ptr_to_metaMerger.get() != nullptr) &&
        (x._ptr_to_metaToResultApplier.get() != nullptr) &&
        (x._data.size() != 0) &&
        (x._length != 0))
    {
      _ptr_to_resultMerger = std::move(std::unique_ptr
             <ResultMerger> (new ResultMerger
                            (*(x._ptr_to_resultMerger))));

      _ptr_to_metaMerger = std::move(std::unique_ptr
             <MetaMerger> (new MetaMerger
                          (*(x._ptr_to_metaMerger))));

      _ptr_to_metaToResultApplier = std::move(std::unique_ptr
             <MetaToResultApplier> (new MetaToResultApplier
                                   (*(x._ptr_to_metaToResultApplier))));

      typename std::vector<std::pair<ResultType,
                           std::unique_ptr<MetaType>>>::const_iterator it;

      for (it = x._data.cbegin(); it != x._data.cend(); ++it)
      {
        _data.push_back(std::pair<ResultType, std::unique_ptr<MetaType>>
                        ((*it).first, std::unique_ptr<MetaType>
                                      (new MetaType(*((*it).second)))));
      }
    }
    else
    {
      if ((x._ptr_to_resultMerger.get() != nullptr) ||
          (x._ptr_to_metaMerger.get() != nullptr) ||
          (x._ptr_to_metaToResultApplier.get() != nullptr) ||
          (x._data.size() != 0) ||
          (x._length != 0))
      {
        std::cerr << "We are copying partly defined GeneralTree" << std::endl;
        x.print_err();
        exit(EXIT_FAILURE);
      }
    }
  }

  GeneralTree& operator = (const GeneralTree& x)
  {
    GeneralTree tmp(x);
    _data = std::move(tmp._data);
    _length = std::move(tmp._length);
    _ptr_to_resultMerger = std::move(tmp._ptr_to_resultMerger);
    _ptr_to_metaMerger = std::move(tmp._ptr_to_metaMerger);
    _ptr_to_metaToResultApplier = std::move(tmp._ptr_to_metaToResultApplier);
    return *this;
  }

  GeneralTree(GeneralTree&& x)
  {
    //To add or not to add checking for part-definition?
    _data = std::move(x._data);
    _length = std::move(x._length);
    _ptr_to_resultMerger = std::move(x._ptr_to_resultMerger);
    _ptr_to_metaMerger = std::move(x._ptr_to_metaMerger);
    _ptr_to_metaToResultApplier = std::move(x._ptr_to_metaToResultApplier);
    x.reset();
  }

  GeneralTree& operator = (GeneralTree&& x)
  {
    //To add or not to add checking for part-definition?
    _data = std::move(x._data);
    _length = std::move(x._length);
    _ptr_to_resultMerger = std::move(x._ptr_to_resultMerger);
    _ptr_to_metaMerger = std::move(x._ptr_to_metaMerger);
    _ptr_to_metaToResultApplier = std::move(x._ptr_to_metaToResultApplier);
    x.reset();
    return *this; 
  }

  ~GeneralTree() = default;
  
  void reset()
  {
    _data.clear();
    _ptr_to_resultMerger = nullptr;
    _ptr_to_metaMerger = nullptr;
    _ptr_to_metaToResultApplier = nullptr;
    _length = 0;
  }

  size_t length() const
  {
    return _length;
  }

  
  void build (size_t length,
      const ResultType&          defaultValue        = ResultType(),
      const ResultMerger&        resultMerger        = ResultMerger(),
      const MetaMerger&          metaMerger          = MetaMerger(),
      const MetaToResultApplier& metaToResultApplier = MetaToResultApplier())
  {
    if ((_ptr_to_resultMerger.get() != nullptr) ||
        (_ptr_to_metaMerger.get() != nullptr) ||
        (_ptr_to_metaToResultApplier.get() != nullptr) ||
        (_data.size() != 0) ||
        (_length != 0))
    {
      std::cerr << "We are building tree but there are already values here. "
                << "Use method reset before redefining." << std::endl;
      print_err();
      exit(EXIT_FAILURE);
    }

    if (length == 0)
    {
      std::cerr << "Initializing tree with size = 0" << std::endl;
      exit(EXIT_FAILURE);
    }

    _ptr_to_resultMerger = std::move(std::unique_ptr
           <ResultMerger> (new ResultMerger(resultMerger)));

    _ptr_to_metaMerger = std::move(std::unique_ptr
           <MetaMerger> (new MetaMerger(metaMerger)));

    _ptr_to_metaToResultApplier = std::move(std::unique_ptr
           <MetaToResultApplier> (new MetaToResultApplier
                                     (metaToResultApplier)));
    _length = length;

    _data.resize(sizeOfTree(length));

    typename std::vector<std::pair<ResultType,
                                   std::unique_ptr<MetaType>>>::iterator it;

    for (it = _data.begin(); it != _data.end(); ++it)
    {
      (*it).first = defaultValue;
      (*it).second = nullptr; 
    }
  
    //we need to recalc data for vertexes
    //that are responsible for segments that are longer that 1
    //we skip vertexes that are responsible for segments of lenght of 1,
    //so we start from the last vertex, responsible for segment of size 2
    //we call vertexes responsible for big segments "MetaVertexes"
    for (size_t i = lastMetaVertexIndex(_data.size()); i != (size_t(-1)); --i)
    {
      _data[i].first = (*(_ptr_to_resultMerger))(_data[leftchildInd(i)].first,
                                                 _data[rightchildInd(i)].first);
    }
  }
  

  
  //Undefined behaviour if iterators are invalid
  template <class InputIterator> void build_from_range (InputIterator begin,
                                                        InputIterator end,
      const ResultMerger&        resultMerger        = ResultMerger(),
      const MetaMerger&          metaMerger          = MetaMerger(),
      const MetaToResultApplier& metaToResultApplier = MetaToResultApplier())
  {
    if ((_ptr_to_resultMerger.get() != nullptr) ||
        (_ptr_to_metaMerger.get() != nullptr) ||
        (_ptr_to_metaToResultApplier.get() != nullptr) ||
        (_data.size() != 0) ||
        (_length != 0))
    {
      std::cerr << "We are building tree but there are already values here. "
                << "Use method reset before redefining." << std::endl;
      print_err();
      exit(EXIT_FAILURE);
    }

    if (begin == end)
    {
      std::cerr << "Initializing from empty range" << std::endl;
      exit(EXIT_FAILURE);
    }

    _ptr_to_resultMerger = std::move(std::unique_ptr
           <ResultMerger> (new ResultMerger(resultMerger)));

    _ptr_to_metaMerger = std::move(std::unique_ptr
           <MetaMerger> (new MetaMerger(metaMerger)));

    _ptr_to_metaToResultApplier = std::move(std::unique_ptr
           <MetaToResultApplier> (new MetaToResultApplier
                                     (metaToResultApplier)));

    _length = 0;

    _data.clear();

    for (InputIterator it = begin; it != end; ++it)
    {
      ++_length;
    }

    _data.resize(sizeOfTree(_length));
    int j = lastMetaVertexIndex(_data.size()) + 1;
    for (InputIterator it = begin; it != end; ++it)
    {
      _data[j].first = *(it);
      _data[j].second = nullptr;
      ++j;
    }  
    

    //recalcing data for "Meta" vertexes
    //to tell the truth, it's not a good idea to apply resultMerger to pseudo values
    //which comlete our length to 2^n
    //because it can cause unexpected fall of resultMerger
    //but it's the only problem
    //because in normal case (operations get and modify)
    //we never call ResultMerger for values out of range
    //so this place will be rewrited not to call resultMerger for pseudovalues
    //but later
    for (size_t i = lastMetaVertexIndex(_data.size()); i != (size_t(-1)); --i)
    {
      _data[i].first = (*(_ptr_to_resultMerger))(_data[leftchildInd(i)].first,
                                                 _data[rightchildInd(i)].first);
      _data[i].second = nullptr;
    }
  }
  
  
  ResultType get(size_t l, size_t r)
  {
    if ((_ptr_to_resultMerger.get() == nullptr) ||
        (_ptr_to_metaMerger.get() == nullptr) ||
        (_ptr_to_metaToResultApplier.get() == nullptr) ||
        (_data.size() == 0) ||
        (_length == 0))
    {
      std::cerr << "calling get but the tree isn't well defined" << std::endl;
      print_err();
      exit(EXIT_FAILURE);
    }
    if (l > r)
    {
      std::cerr << "calling get with l > r; " << l << ">" << r << std::endl;
      print_err();
      exit(EXIT_FAILURE);
    }
    if (r >= _length)
    {
      std::cerr << "calling get but right border is out of range" << std::endl
                << "r = " << r << std::endl;
      print_err();
      exit (EXIT_FAILURE);
    }
    //std::cerr << "get (" << l << ", " << r << ")" << std::endl;
    return inner_get(l, r, 0, 0, lengthOfLowerLevel(_data.size()) - 1);
  }
  
  void modify(size_t l, size_t r, MetaType changes)
  {
    if ((_ptr_to_resultMerger.get() == nullptr) ||
        (_ptr_to_metaMerger.get() == nullptr) ||
        (_ptr_to_metaToResultApplier.get() == nullptr) ||
        (_data.size() == 0) ||
        (_length == 0))
    {
      std::cerr << "calling get but the tree isn't well defined" << std::endl;
      print_err();
      exit(EXIT_FAILURE);
    }
    if (l > r)
    {
      std::cerr << "calling get with l > r; " << l << ">" << r << std::endl;
      print_err();
      exit(EXIT_FAILURE);
    }
    if (r >= _length)
    {
      std::cerr << "calling get but right border is out of range" << std::endl
                << "r = " << r << std::endl;
      print_err();
      exit (EXIT_FAILURE);
    }
    //std::cerr << "modify (" << l << ", " << r << ", " << changes << ")" << std::endl;
    inner_modify(l, r, changes, 0, 0, lengthOfLowerLevel(_data.size()) - 1);
  }

friend class TestAccess<GeneralTree>;
private:
  
  void inner_modify(size_t l, size_t r, MetaType changes,
                    size_t ind, size_t vertex_l, size_t vertex_r)
  {
    ++N_INNER_MODIFY;
    //std::cerr << "inner_modify(" << l << ", " << r << ", "
    //          << changes << ", " << ind << ", "
    //          << vertex_l << ", " << vertex_r << ")" << std::endl;
    if ((vertex_l == l) && (vertex_r == r))
    {
      //todo: instant use for leafs
      if (_data[ind].second == nullptr)
      {
        _data[ind].second = std::move(std::unique_ptr<MetaType>
                                                 (new MetaType(changes)));
        return;
      }
      *(_data[ind].second) = (*(_ptr_to_metaMerger))(*(_data[ind].second),
                                                   changes);
      return;
    }

    ResultType leftres;
    size_t vertexLeftSonL = vertex_l;
    size_t vertexLeftSonR = countLeftSonR(vertex_l, vertex_r);

    ResultType rightres;
    size_t vertexRightSonL = countRightSonL(vertex_l, vertex_r);
    size_t vertexRightSonR = vertex_r;

    //copypaste from get without part of comments
    //mb it will be a special function "push_to_child"
    if (_data[ind].second != nullptr)
    {
      inner_modify(vertexLeftSonL, vertexLeftSonR, *(_data[ind].second),
                   leftchildInd(ind), vertexLeftSonL, vertexLeftSonR);
      inner_modify(vertexRightSonL, vertexRightSonR, *(_data[ind].second),
                   rightchildInd(ind), vertexRightSonL, vertexRightSonR);

      //Now meta is applied to children and isn't needed here
      _data[ind].second = nullptr;
    }
    //end copypaste

    if (l <= vertexLeftSonR)
    {
      inner_modify(l, std::min(r, vertexLeftSonR), changes,
                   leftchildInd(ind), vertexLeftSonL, vertexLeftSonR);
    }

    if (r >= vertexRightSonL)
    {
      inner_modify(std::max(l, vertexRightSonL), r, changes,
                   rightchildInd(ind), vertexRightSonL, vertexRightSonR);
    }

    //one more copypaste, I call it "recalc from children"
    //we recalc result ONLY for vertexes, which cover ONLY good points of segment
    //we don't need res in other vertexes
    //really don't need
    if (vertex_r < _length)
    {
      leftres = inner_get(vertexLeftSonL, vertexLeftSonR,
                          leftchildInd(ind),
                          vertexLeftSonL, vertexLeftSonR);
      rightres = inner_get (vertexRightSonL, vertexRightSonR,
                            rightchildInd(ind),
                            vertexRightSonL, vertexRightSonR);
      _data[ind].first = (*(_ptr_to_resultMerger))(leftres, rightres);
    }
    //end copypaste
  }
  
  ResultType inner_get(size_t l, size_t r,
                       size_t ind, size_t vertex_l, size_t vertex_r)
  {
    ++N_INNER_GET;
    //std::cerr << "inner_get(" << l << ", " << r << ", " << ind << ", "
    //          << vertex_l << ", " << vertex_r << ")" << std::endl;

    if ((vertex_l == l) && (vertex_r == r))
    {
      if (_data[ind].second != nullptr)
        return (*(_ptr_to_metaToResultApplier))(*(_data[ind].second),
                                                  _data[ind].first);
    //  std::cerr << "return myself" << std::endl;
      return _data[ind].first;
    }

    ResultType leftres;
    size_t vertexLeftSonL = vertex_l;
    size_t vertexLeftSonR = countLeftSonR(vertex_l, vertex_r);

    ResultType rightres;
    size_t vertexRightSonL = countRightSonL(vertex_l, vertex_r);
    size_t vertexRightSonR = vertex_r;

    if (_data[ind].second != nullptr)
    {
      //metaMerger can't cause any problems if applied to the whole vertex
      //even if the vertex is invalid (covers pseudo vertex)

      //Children exist because every call of get is
      //supported to be correct and in correct call 
      //((vertex_l == l) && (vertex_r == r)) is true for leaf
      //so we don't reach this part of code
      inner_modify(vertexLeftSonL, vertexLeftSonR, *(_data[ind].second),
                   leftchildInd(ind), vertexLeftSonL, vertexLeftSonR);
      inner_modify(vertexRightSonL, vertexRightSonR, *(_data[ind].second),
                   rightchildInd(ind), vertexRightSonL, vertexRightSonR);

      //Now meta is applied to children and isn't needed here
      _data[ind].second = nullptr;

      //we recalc result ONLY for vertexes, which cover ONLY good points of segment
      //we don't need res in other vertexes
      //really don't need
      if (vertex_r < _length)
      {
        leftres = inner_get(vertexLeftSonL, vertexLeftSonR,
                            leftchildInd(ind),
                            vertexLeftSonL, vertexLeftSonR);
        rightres = inner_get (vertexRightSonL, vertexRightSonR,
                              rightchildInd(ind),
                              vertexRightSonL, vertexRightSonR);
        _data[ind].first = (*(_ptr_to_resultMerger))(leftres, rightres);
      }
    }

    if (l <= vertexLeftSonR)
    {
      leftres = inner_get(l, std::min(r, vertexLeftSonR),
                          leftchildInd(ind),
                          vertexLeftSonL, vertexLeftSonR);
    }

    if (r >= vertexRightSonL)
    {
      rightres = inner_get(std::max(l, vertexRightSonL), r,
                           rightchildInd(ind),
                           vertexRightSonL, vertexRightSonR);
    }

    ResultType res;
    if ((l <= vertexLeftSonR) && (r >= vertexRightSonL))
    {
      //std::cerr << "joined res" << std::endl;
      res = (*(_ptr_to_resultMerger))(leftres, rightres);
    }
    else
    {
      if (l <= vertexLeftSonR)
      {
        //std::cerr << "just left res" << std::endl;
        res = leftres;
      }
      else
      {
        //std::cerr << "just right res" << std::endl;
        res = rightres;
      }
    }
    return res;
  }
  

  void print_err() const
  {
    std::cerr << "resultMerger "
              << _ptr_to_resultMerger.get() << std::endl
              << "metaMerger " 
              << _ptr_to_metaMerger.get() << std::endl
              << "metaToResultApplier "
              << _ptr_to_metaToResultApplier.get() << std::endl
              << "_data.size() "
              << _data.size() << std::endl
              << "length " << _length << std::endl;
  }

  std::unique_ptr<ResultMerger> _ptr_to_resultMerger;
  std::unique_ptr<MetaMerger> _ptr_to_metaMerger;
  std::unique_ptr<MetaToResultApplier> _ptr_to_metaToResultApplier;
  std::vector<std::pair<ResultType, std::unique_ptr<MetaType>>> _data;
  size_t _length;
};
}

struct ResSumTree
{
  ResSumTree():_l(-1), _r(-1), _sum(0) {}
  ResSumTree(size_t l, size_t r, long long sum):_l(l), _r(r), _sum(sum) {}

  size_t _l;
  size_t _r;
  long long _sum;
};

struct ResultMergerSumTree
{
  ResSumTree operator()(ResSumTree left, ResSumTree right)
  {
  return ResSumTree(left._l, right._r, left._sum + right._sum);
  }
};


struct MetaToResSumAddTree
{
  ResSumTree operator()(long long meta, ResSumTree res)
  {
    return ResSumTree(res._l, res._r, res._sum + meta * (res._r - res._l + 1));
  }
};

struct MetaToResSumAssignTree
{
  ResSumTree operator()(long long meta, ResSumTree res)
  {
    return ResSumTree(res._l, res._r, meta * (res._r - res._l + 1));
  }
};

struct MetaMergerSumAddTree
{
  long long operator()(long long oldmeta, long long newmeta)
  {
    return oldmeta + newmeta;
  }
};

struct MetaMergerSumAssignTree
{
  long long operator()(long long oldmeta, long long newmeta)
  {
    return newmeta;
  }
};

namespace segmentTree{
typedef segmentTree::GeneralTree <ResSumTree, long long,
                                  ResultMergerSumTree,
                                  MetaMergerSumAddTree,
                                  MetaToResSumAddTree> sum_add;

typedef segmentTree::GeneralTree <ResSumTree, long long,
                                  ResultMergerSumTree,
                                  MetaMergerSumAssignTree,
                                  MetaToResSumAssignTree> sum_assign;                                
}

int main()
{
  freopen("sum.in", "r", stdin);
  freopen("sum.out", "w", stdout);
  int N;
  std::cin >> N;
  segmentTree::sum_assign rsq;
  std::vector<ResSumTree> v;
  for (int i = 0; i <= N; ++i)
    v.push_back(ResSumTree(i, i, 0));
  rsq.build_from_range(v.begin(),v.end());

  int K;
  std::cin >> K;
  for (int i = 0; i < K; ++i)
  {
    char cmd;
    size_t l, r;
    long long x;
    std::cin >> cmd;
    if (cmd == 'A')
    {
      std::cin >> l >> r >> x;
      rsq.modify(l, r, x);
    }
    if (cmd == 'Q')
    {
      std::cin >> l >> r;
      std::cout << rsq.get(l, r)._sum << std::endl;
    }
  }
  return 0;
}
