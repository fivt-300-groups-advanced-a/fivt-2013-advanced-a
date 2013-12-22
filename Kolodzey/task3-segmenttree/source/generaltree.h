#include <memory>
#include <iostream>
#include <cstdlib>

template <class T> class TestAccess;

namespace segmentTree {

template <class ResultType,
          class MetaType,
          class ResultMerger,
          class MetaMerger,
          class MetaToResultApplier>
class GeneralTree
{
public:
  GeneralTree(): _data(std::vector<std::pair<ResultType, MetaType> >()),
                 _ptr_to_resultMerger(nullptr),
                 _ptr_to_metaMerger(nullptr),
                 _ptr_to_metaToResultApplier(nullptr) {}

  GeneralTree(const GeneralTree& x): _data(x._data),
                                     _ptr_to_resultMerger(nullptr),
                                     _ptr_to_metaMerger(nullptr),
                                     _ptr_to_metaToResultApplier(nullptr)
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

friend class TestAccess<GeneralTree>;
private:
  std::vector<std::pair<ResultType, MetaType>> _data;
  std::unique_ptr<ResultMerger> _ptr_to_resultMerger;
  std::unique_ptr<MetaMerger> _ptr_to_metaMerger;
  std::unique_ptr<MetaToResultApplier> _ptr_to_metaToResultApplier;
};

}