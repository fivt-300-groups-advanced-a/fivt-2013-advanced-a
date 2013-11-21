#include <vector>
#include <memory>

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
    friend class TestAccess<ValPointer<T>,T>;
    friend bool BiTreeFunc<T>::rehang(ValPointer<T> pa, ValPointer<T> pb);
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
  friend class TestAccess<ValHolder<T>,T>;
  friend bool BiTreeFunc<T>::rehang(ValPointer<T> pa, ValPointer<T> pb);
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
  friend class TestAccess<BiTree<T>,T>;
  friend bool BiTreeFunc<T>::rehang(ValPointer<T> pa, ValPointer<T> pb);
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
      if ((pa._ref == 0) || (pb._ref == 0))
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
};