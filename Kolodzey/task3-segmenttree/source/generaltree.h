#include <memory>
#include <iostream>
#include <cstdlib>

template <class T> class TestAccess;

namespace segmentTree {

size_t sizeOfTree(size_t lenght)
{
  if (lenght == 0)
  {
    std::cerr << "counting size of empty tree";
    exit(EXIT_FAILURE);
  }
  return (lenght * 2 - 1);
}

size_t lastMetaVertexIndex(size_t treeSize)
{
  if (treeSize == 0)
  {
    std::cerr << "not possible to find lastMetaVertexIndex an empty tree";
    exit(EXIT_FAILURE);
  }
  if (treeSize % 2 == 0)
  {
    std::cerr << "treeSize mod 2 is 0, smth is wrong";
    exit(EXIT_FAILURE);
  }
  return ((treeSize / 2) - 1);
}

inline size_t leftchild(size_t ind)
{
  return ind * 2 + 1;
}

inline size_t rightchild(size_t ind)
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
                 _data(std::vector<std::pair<ResultType, MetaType>>()) {} 

  GeneralTree(const GeneralTree& x): _ptr_to_resultMerger(nullptr),
                                     _ptr_to_metaMerger(nullptr),
                                     _ptr_to_metaToResultApplier(nullptr),
                                     _data(x._data)
  {
    if ((x._ptr_to_resultMerger.get() != nullptr) &&
        (x._ptr_to_metaMerger.get() != nullptr) &&
        (x._ptr_to_metaToResultApplier.get() != nullptr) &&
        (x._data.size() != 0))
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
    }
    else
    {
      if ((x._ptr_to_resultMerger.get() != nullptr) ||
          (x._ptr_to_metaMerger.get() != nullptr) ||
          (x._ptr_to_metaToResultApplier.get() != nullptr) ||
          (x._data.size() != 0))
      {
        std::cerr << "We are copying partly defined GeneralTree. "
                  << "This situation shouldn't happen." << std::endl;
        exit(EXIT_FAILURE);
      }
    }
  }

  GeneralTree& operator = (const GeneralTree&) = default;
  GeneralTree(GeneralTree&& x) = default;
  GeneralTree& operator = (GeneralTree&&) = default;
  ~GeneralTree() = default;

  void build (size_t length,
      const ResultType&          defaultValue        = ResultType(),
      const MetaType&            defaultMeta         = MetaType(),
      const ResultMerger&        resultMerger        = ResultMerger(),
      const MetaMerger&          metaMerger          = MetaMerger(),
      const MetaToResultApplier& metaToResultApplier = MetaToResultApplier())
  {
    if ((_ptr_to_resultMerger.get() != nullptr) ||
        (_ptr_to_metaMerger.get() != nullptr) ||
        (_ptr_to_metaToResultApplier.get() != nullptr) ||
        (_data.size() != 0))
    {
      std::cerr << "We are building tree but there are already values here. "
                << "Use method reset before redefining." << std::endl;
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

    _data.insert(_data.begin(), sizeOfTree(length),
                 std::pair<ResultType,MetaType>(defaultValue, defaultMeta));
  
    //we need to recalc data for vertexes
    //that are responsible for segments that are longer that 1
    //we skip vertexes that are responsible for segments of lenght of 1,
    //so we start from the last vertex, responsible for segment of size 2
    //we call vertexes responsible for big segments "MetaVertexes"
    for (size_t i = lastMetaVertexIndex(_data.size()); i != (size_t(-1)); --i)
    {
      _data[i].first = (*(_ptr_to_resultMerger))(_data[leftchild(i)].first,
                                                 _data[rightchild(i)].first);
    }
  }

  //Undefined behaviour if iterators are invalid
  template <class InputIterator> void build_from_range (InputIterator begin,
                                                        InputIterator end,
      const MetaType&            defaultMeta         = MetaType(),
      const ResultMerger&        resultMerger        = ResultMerger(),
      const MetaMerger&          metaMerger          = MetaMerger(),
      const MetaToResultApplier& metaToResultApplier = MetaToResultApplier())
  {
    if ((_ptr_to_resultMerger.get() != nullptr) ||
        (_ptr_to_metaMerger.get() != nullptr) ||
        (_ptr_to_metaToResultApplier.get() != nullptr) ||
        (_data.size() != 0))
    {
      std::cerr << "We are building tree but there are already values here. "
                << "Use method reset before redefining." << std::endl;
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

    for (InputIterator it = begin; it != end; ++it)
    {
      _data.push_back(std::pair<ResultType, MetaType>(*it,defaultMeta));
    }
    //need to insert (_data.size() - 1) "Meta" Vertexes;
    //Number (_data.size() - 1) comes from the structure of binarytree
    _data.insert(_data.begin(), _data.size() - 1,
                 std::pair<ResultType,MetaType>(ResultType(),defaultMeta));
    //recalcing data for "Meta" vertexes
    for (size_t i = lastMetaVertexIndex(_data.size()); i != (size_t(-1)); --i)
    {
      _data[i].first = (*(_ptr_to_resultMerger))(_data[leftchild(i)].first,
                                                 _data[rightchild(i)].first);
    }
  }

  void reset()
  {
    _data.clear();
    _ptr_to_resultMerger = nullptr;
    _ptr_to_metaMerger = nullptr;
    _ptr_to_metaToResultApplier = nullptr;
  } 

friend class TestAccess<GeneralTree>;
private:
  std::unique_ptr<ResultMerger> _ptr_to_resultMerger;
  std::unique_ptr<MetaMerger> _ptr_to_metaMerger;
  std::unique_ptr<MetaToResultApplier> _ptr_to_metaToResultApplier;
  std::vector<std::pair<ResultType, MetaType>> _data;
};

}