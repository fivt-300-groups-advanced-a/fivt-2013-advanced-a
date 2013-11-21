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
      _ref = NULL;
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
    ValHolder(T val, BiTree<T>* pos = NULL)
    {
      _val = val;
      _pos = pos;
    }
    void set_pos(BiTree<T>* pos)
    {
      _pos = pos;
    }
  friend class TestAccess<ValHolder<T>,T>;
  private:
    T _val;
    BiTree<T>* _pos;
};

template <class T>
class BiTree
{
  friend class TestAccess<BiTree<T>,T>;
  private:
    std::vector <std::unique_ptr <BiTree <T> > > _child;
    BiTree<T>* _parent;
    int _level;
    std::unique_ptr<ValHolder<T> > _val;
};