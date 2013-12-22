namespace segmentTree
{
template <class ResultType,
          class MetaType,
          class ResultMerger,
          class MetaMerger,
          class MetaToResultApplier> class GeneralTree;
}

template <class T> class TestAccess;

template <class ResultType,
          class MetaType,
          class ResultMerger,
          class MetaMerger,
          class MetaToResultApplier> class TestAccess
                                           <segmentTree::GeneralTree
                                                       <ResultType,
                                                        MetaType,
                                                        ResultMerger, 
                                                        MetaMerger,
                                                        MetaToResultApplier> >
{
public:
  typedef segmentTree::GeneralTree <ResultType,
                                    MetaType,
                                    ResultMerger, 
                                    MetaMerger,
                                    MetaToResultApplier> GeneralTree;

  TestAccess(): _ptr_to_GTree(nullptr) {}
  explicit TestAccess(GeneralTree* ptrToGTree): _ptr_to_GTree(ptrToGTree) {}

  void set_tree(GeneralTree* ptrToGTree)
  {
    _ptr_to_GTree = ptrToGTree;
  }

  std::unique_ptr<ResultMerger>& get_ptr_to_resultMerger()
  {
    return _ptr_to_GTree->_ptr_to_resultMerger;
  }

  std::unique_ptr<MetaMerger>& get_ptr_to_metaMerger()
  {
    return _ptr_to_GTree->_ptr_to_metaMerger;
  }

  std::unique_ptr<MetaToResultApplier>& get_ptr_to_metaToResultApplier()
  {
    return _ptr_to_GTree->_ptr_to_metaToResultApplier;
  }

  std::vector<std::pair<ResultType, MetaType> >& get_data()
  {
    return _ptr_to_GTree->_data;
  }

private:
  GeneralTree* _ptr_to_GTree;
};