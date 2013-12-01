#include <vector>
#include <utility>
#include <memory>
#include <list>
#include <iostream>

template <class Container, class Value> class TestAccess;

template <class T> class ValHolder;
template <class T> class ValPointer;
template <class T> class BiTree;

template <class T> class BiTreeFunc;

template <class T>
class ValPointer
{
  public:
    ValPointer()
    {
      _ref = nullptr;
    }
    ValPointer(ValHolder<T>* ref)
    {
      _ref = ref;
    }
    const T& get_val()
    {
      if (_ref == nullptr)
        std::cerr << "Accessing value by empty valptr" << std::endl;
      return _ref->get_val();
    }
    friend class TestAccess<ValPointer<T>,T>;
    friend bool BiTreeFunc<T>::rehang(ValPointer<T> pa, ValPointer<T> pb);
    friend bool BiTree<T>::is_son(ValPointer<T> p);
    friend bool BiTree<T>::lift(ValPointer<T> pval);
  private:
    ValHolder<T>* _ref;
};

template <class T>
class ValHolder
{
  public:
    ValHolder(T val, BiTree<T>* pos = nullptr)
    {
      _val = val;
      _pos = pos;
    }
    const T& get_val()
    {
      return _val;
    }
  friend class TestAccess<ValHolder<T>,T>;
  friend bool BiTreeFunc<T>::rehang(ValPointer<T> pa, ValPointer<T> pb);
  friend bool BiTree<T>::is_son(ValPointer<T> p);
  friend bool BiTree<T>::lift(ValPointer<T> pval);
  private:
    T _val;
    BiTree<T>* _pos;
};

template <class T>
class BiTree
{
  public:
    //Тривиальный конструктор не нужен,
    //всё равно с пустым деревом потом сделать ничего нельзя
    /*
    BiTree()
    {
      _child.clear();
      _parent = nullptr;
      _level = 0;
      _pval = nullptr;
    }
    */
    //Важно, трудноловимая, но возможная к возникновению бага:
    //Если попробовать присвоить по указателю свежесконструированный BiTree
    //То ссылка из соответствующего ValHolder будет вести не туда!
    //Она будет вести туда, где сконструировался новый BiTree
    //А конструируется он явно не в месте, занятым пустым BiTree,
    //Созданным при создании указателя
    BiTree(T val)
    {
      _child.clear();
      _parent = nullptr;
      _level = 1;
      _pval = std::unique_ptr<ValHolder<T> > (new ValHolder<T>(val, this));
    }
    int get_level()
    {
      return _level;
    }
    ValPointer<T> get_pval()
    {
      return ValPointer<T>(_pval.get());
    }
    bool eat(std::unique_ptr<BiTree<T> >& pt)
    {
      if (this == nullptr)
        return 0;
      if (pt == nullptr)
        return 0;
      if (_level != (*pt)._level)
        return 0;
      if (_level == 0) //случай вроде невозможен после убийства пустого конструктора
        return 0;
      ++_level;
      _child.push_back(nullptr);
      _child.back().swap(pt);
      _child.back()->_parent = this;
      return 1;
    }
    bool is_son(ValPointer<T> pval) //хм, почему же падает при дописывании const?
    {
      if (this == nullptr)
        return 0;
      if (pval._ref == nullptr)
        return 0;
      BiTree<T>* pvertex = pval._ref->_pos;
      while (pvertex != nullptr)
      {
        if (pvertex == this)
          return 1;
        pvertex = pvertex->_parent;
      }
      return 0;
    }
    bool lift(ValPointer<T> pval)
    {
      if (is_son(pval))
      {
        BiTree<T>* pvertex = pval._ref->_pos;
        while (pvertex != this)
        {
          BiTreeFunc<T>::rehang(pvertex->get_pval(),
                                pvertex->_parent->get_pval());
          pvertex = pvertex->_parent;
        }
        return 1;
      }
      return 0;
    }
    const T& get_val()
    {
      return _pval->get_val();
    }
  friend class TestAccess<BiTree<T>,T>;
  friend bool BiTreeFunc<T>::rehang(ValPointer<T> pa, ValPointer<T> pb);
  friend bool BiTreeFunc<T>::cutroot(std::unique_ptr<BiTree<T> >& tree,
                            std::list<std::unique_ptr<BiTree<T> > >& children);
  private:
    std::vector <std::unique_ptr <BiTree <T> > > _child;
    BiTree<T>* _parent;
    int _level;
    std::unique_ptr<ValHolder<T> > _pval;
};

template <class T>
class BiTreeFunc
{
  public:
    static bool rehang(ValPointer<T> pa, ValPointer<T> pb)
    {
      if ((pa._ref == nullptr) || (pb._ref == nullptr))
        return 0;
      BiTree<T>* oldPosA = pa._ref->_pos;
      BiTree<T>* oldPosB = pb._ref->_pos;
      BiTree<T>* newPosA = oldPosB;
      BiTree<T>* newPosB = oldPosA;
      ValHolder<T>* AddressA = pa._ref;
      ValHolder<T>* AddressB = pb._ref;
      AddressA->_pos = newPosA;
      AddressB->_pos = newPosB;
      swap(newPosA->_pval, newPosB->_pval);
      return 1;
    }
    template <class Compare>
    static bool merge(std::unique_ptr<BiTree<T> >& a,
                      std::unique_ptr<BiTree<T> >& b,
                      std::unique_ptr<BiTree<T> >& res,
                      Compare Cmp)
    {
      if ((a == nullptr) || (b == nullptr))
        return 0;
      if (Cmp(a->get_val(), b->get_val())) //в дереве обязательно что-то лежить, пустого конструктора нет, поэтому этот случай не проверяем
      {
        bool flag = a->eat(b);
        if (flag == 0)
          return 0;

        res.reset();
        swap(a, res);
        return 1;
      }
      bool flag = b->eat(a);
      if (flag == 0)
        return 0;
      res.reset();
      swap(b, res);
      return 1;
      //по идее при некорректности операции возвращается 0 и всё остаётся, как было
    }
    static bool cutroot(std::unique_ptr<BiTree<T> >& tree,
                        std::list<std::unique_ptr<BiTree<T> > >& children)//список упорядочен по возрастанию уровня
    {
      if (tree == nullptr)
        return 0;
      children.clear();
      for (size_t i = 0; i < tree->_child.size(); ++i)
      {
        children.push_back(nullptr);
        swap(children.back(), (tree->_child[i])); //по прямому присвоить не получится
        children.back()->_parent = nullptr;
      }
      tree.reset();
      return 1;
    }
    static bool cmp_levels(const std::unique_ptr<BiTree<T> >& a,
                    const std::unique_ptr<BiTree<T> >& b)
    {
      if ((a == nullptr) && (b == nullptr))
        return 0;
      if (a == nullptr) //нулевой указатель считается самым маленьким деревом
        return 1;
      if (b == nullptr)
        return 0;
      return (a->get_level() < b->get_level());
    }
};

//и на 200+ строке наконец-то заканчиваются вспомогательные классы...
template <class T, class Compare = std::less<int> > 
class BiHeap
{
  public:
    typedef typename std::list<std::unique_ptr<BiTree<T> > >::iterator Itlist;
    BiHeap(Compare cmp = std::less<T>())
    {
      _forest.clear();
      _cmp = cmp;
    }
    ValPointer<T> insert(T val)
    {
      std::unique_ptr<BiTree<T> > tree (new BiTree<T> (val));
      ValPointer<T> ans = tree->get_pval();
      std::list<std::unique_ptr<BiTree<T> > > listForTree;
      listForTree.push_back(nullptr);
      swap(listForTree.back(), tree);
      insert_list(listForTree);
      improve_list();
      return ans;
    }
    
    void eat(BiHeap<T,Compare>& heap) //попытка сожрать кучу с другим компаратором(точнее с компаратором в другом состоянии) вызывает undefined behaviour
    {
      std::list<std::unique_ptr<BiTree<T> > > elem;
      heap.export_to_list(elem); //может это и костыль, но как ещё обходить <std::list> does not provide a call operator?
      insert_list(elem);
      improve_list();
    }
    
    bool is_empty()
    {
      return (_forest.size() == 0);
    }

    ValPointer<T> get_top_ref()
    {
      if (is_empty())
        return ValPointer<T>(nullptr);
      ValPointer<T> ans = (_forest.front())->get_pval();
      typename std::list<std::unique_ptr<BiTree<T> > >::iterator it;
      for (it = _forest.begin(); it != _forest.end(); ++it)
      {
        if (_cmp((*it)->get_val(), ans.get_val()))
          ans = (*it)->get_pval();
      }
      return ans;
    }

    const T& top()
    {
      return get_top_ref().get_val(); //падает при запросе к пустой куче
    }

    bool erase(ValPointer<T> pval)
    {
      //std::cerr << "entered erase" << std::endl;
      Itlist it = _forest.begin();
      while (it != _forest.end())
      {
        //std::cerr << "looking at tree of level " << (*it)->get_level() 
        //          << " with top value" << (*it)->get_val() << std::endl;
        if ((*it)->is_son(pval))
        {
          //std::cerr << "found tree which holds value to delete" << std::endl;
          std::unique_ptr<BiTree<T> > tr(nullptr);
          swap(*it, tr);
          _forest.erase(it);
          //std::cerr << "extracted tree from list" << std::endl;
          tr->lift(pval);
          //std::cerr << "lifted value to the top of tree" << std::endl;
          std::list<std::unique_ptr<BiTree<T> > > ch;
          BiTreeFunc<T>::cutroot(tr, ch);
          //std::cerr << "created list of children of the tree (root deleted)" << std::endl;
          insert_list(ch);
          //std::cerr << "inserted list of children into heap" << std::endl;
          improve_list();
          //std::cerr << "improved list of children (merged trees of the same level)" << std::endl;
          return 1;
        }
        ++it;
      }
      return 0;
    }
    bool pop()
    {
      ValPointer<T> ptop = get_top_ref();
      //std::cerr << "pop find ref to top, top value is " << ptop.get_val() << std::endl;
      return erase(ptop);
    }
  friend class TestAccess <BiHeap<T, Compare>,T>;
  private:
    void export_to_list(std::list<std::unique_ptr<BiTree<T> > >& elem)
    {
      elem.clear();
      typename std::list<std::unique_ptr<BiTree<T> > >::iterator it;
      for (it = _forest.begin(); it != _forest.end(); ++it)
      {
        elem.push_back(nullptr);
        swap(elem.back(), *it);
      }
      _forest.clear();
    }
    void insert_list(std::list<std::unique_ptr<BiTree<T> > >& elem)
    {
      _forest.merge(elem, BiTreeFunc<T>::cmp_levels);
    }
    bool improve_list()
    {
      //std::cerr << "entered method improve_list" << std::endl;
      Itlist it = _forest.end();
      --it;
      while (it != _forest.begin())
      {
        typename std::list<std::unique_ptr<BiTree<T> > >::iterator next = it;
        --next;
        if ((*it == nullptr) || (*next == nullptr)) //не, ну мёржить нулевые деревья… такое нам хранить не надо, лучше падать.
        { 
          std::cerr << "zero trees in heap" << std::endl;
          return 0;
        }
        //std::cerr << "it lvl " << (*it)->get_level()
        //          << " val " << (*it)->get_val() << std::endl;
        //std::cerr << "next lvl " << (*next)->get_level()
        //          << " val " << (*next)->get_val() << std::endl;
        if ((*next)->get_level() < (*it)->get_level())
        {
          --it;
        }
        else if ((*next)->get_level() == (*it)->get_level())
        {
          std::unique_ptr<BiTree<T> > c (nullptr);
          BiTreeFunc<T>::merge(*it, *next, c, _cmp);
          //std::cerr << "successful merge" << std::endl;
          _forest.erase(next);
          //std::cerr << "erased" << std::endl;
          swap(c, *it);
          if (it != (--_forest.end()))
            ++it; 
        }
        else if ((*next)->get_level() > (*it)->get_level()) //список деревьев не упорядочен по возрастанию размера, вернуть 0, значения испорчены :(
        {
          std::cerr << "trees are not ordered by size in heap" << std::endl;
          return 0;
        }
      }
      return 1;
    }
    Compare _cmp;
    std::list<std::unique_ptr<BiTree<T> > > _forest;
};