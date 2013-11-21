#include <vector>
#include <memory>

template <class Container, class Value> class TestAccess;

template <class T> class ValHolder;
template <class T> class ValPointer;
template <class T> class BiTree;

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
    void set_pos(BiTree<T>* pos)
    {
      _pos = pos;
    }
  friend class TestAccess<ValHolder<T>,T>;
  friend class BiTree<T>;
  private:
    T _val;
    BiTree<T>* _pos;
};

template <class T>
class BiTree
{
  public:
    BiTree()
    {
      _child.clear();
      _parent = nullptr;
      _level = 0;
      _pval = nullptr;
    }
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
    bool eat(std::unique_ptr<BiTree<T> >& pt)
    {
      if (this == nullptr)
        return 0;
      if (pt == nullptr)
        return 0;
      if (_level != (*pt)._level)
        return 0;
      if (_level == 0)
        return 0;
      ++_level;
      _child.push_back(nullptr);
      _child.back().swap(pt);
      _child.back()->_parent = this;
      return 1;

    }
  friend class TestAccess<BiTree<T>,T>;
  private:
    std::vector <std::unique_ptr <BiTree <T> > > _child;
    BiTree<T>* _parent;
    int _level;
    std::unique_ptr<ValHolder<T> > _pval;
};