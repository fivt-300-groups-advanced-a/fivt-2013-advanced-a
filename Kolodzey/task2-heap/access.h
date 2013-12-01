template <class Value>
class TestAccess <ValHolder<Value>, Value>
{
  public:
    Value& _val()
    {
      return _valholder->_val;
    }
    BiTree<Value>*& _pos()
    {
      return _valholder->_pos;
    }
    ValHolder<Value>* _valholder;
};

template <class Value>
class TestAccess <ValPointer<Value>, Value>
{
  public:
    ValHolder<Value>*& _ref()
    {
      return _valpointer->_ref;
    }
    ValPointer<Value>*_valpointer;
};

template <class Value>
class TestAccess <BiTree<Value>, Value>
{
  public:
    std::vector <std::unique_ptr <BiTree <Value> > > & _child()
    {
      return _bitree->_child;
    }
    BiTree<Value>*& _parent()
    {
      return _bitree->_parent;
    }
    int& _level()
    {
      return _bitree->_level;
    }
    const std::unique_ptr<ValHolder<Value> > & _pval()
    {
      return _bitree->_pval;
    }
    BiTree<Value>* _bitree;
    static void dfs(const std::unique_ptr<BiTree<Value> >& p, std::vector<Value>& vals)
    {
      if (p == nullptr)
        return;
      Value val = p->get_val();
      vals.push_back(val);
      for (size_t i = 0; i < p->_child.size(); ++i)
        dfs(p->_child[i], vals);
    }
};

template<class Value, class Compare>
class TestAccess <BiHeap<Value, Compare>, Value>
{
  public:
    Compare& _cmp()
    {
      return _biheap->_cmp;
    }
    std::list<std::unique_ptr<BiTree<Value> > >& _forest()
    {
      return _biheap->_forest;
    }
    static void insert_list(BiHeap<Value, Compare>& biheap,
                       std::list<std::unique_ptr<BiTree<Value> > >& elem)
    {
      biheap.insert_list(elem);
    }
    static bool improve_list(BiHeap<Value, Compare>& biheap)
    {
      bool flag = 0;
      flag = biheap.improve_list();
      return flag;
    }
    static void get_all_val(BiHeap<Value, Compare>& heap, std::vector<Value>& vals)
    {
      typename std::list<std::unique_ptr<BiTree<Value> > >::iterator it;
      for (it = heap._forest.begin(); it != heap._forest.end(); ++it)
        TestAccess<BiTree<Value>,Value>::dfs(*(it), vals);
    }
    BiHeap<Value, Compare>* _biheap;
};