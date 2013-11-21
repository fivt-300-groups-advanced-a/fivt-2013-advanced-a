#include <utility>
#include "gtest/gtest.h"
#include "biheap.h"

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
};
TEST(ValHolder, Constructor)
{
  ValHolder<int> v(7);
  TestAccess<ValHolder<int>, int> v_access;
  v_access._valholder = &v;
  EXPECT_EQ(v_access._val(), 7);
  EXPECT_EQ(v_access._pos(), nullptr);
  v_access._val() = 6;
  EXPECT_EQ(v_access._val(), 6);

  ValHolder<std::pair<int, char> > t(std::make_pair(3, 'a'));
  TestAccess<ValHolder<std::pair<int, char> >, std::pair<int, char> > a_t;
  a_t._valholder = &t;
  EXPECT_EQ(a_t._val().first, 3);
  EXPECT_EQ(a_t._val().second, 'a');
}

TEST(ValPointer, Constructor)
{
  ValPointer<std::pair<int, char> > pu;
  TestAccess<ValPointer<std::pair<int, char> >, std::pair<int, char> > a_pu;
  a_pu._valpointer = &pu;
  EXPECT_EQ(a_pu._ref(), nullptr);

  ValHolder<std::pair<int, char> > v(std::make_pair(3, 'a'));
  ValPointer<std::pair<int, char> > pv(&v);
  TestAccess<ValPointer<std::pair<int, char> >, std::pair<int, char> > a_pv;
  a_pv._valpointer = &pv;
  EXPECT_EQ(a_pv._ref(), &v);
}

TEST(BiTree, trivialConstructor)
{
  BiTree<int> t;
  TestAccess<BiTree<int>,int> a_t;
  a_t._bitree = &t;
  EXPECT_EQ(a_t._child().size(), 0);
  EXPECT_EQ(&(*a_t._pval()), nullptr);
  EXPECT_EQ(a_t._parent(), nullptr);
  EXPECT_EQ(a_t._level(), 0);
}

TEST(BiTree, nontrivialConstructor)
{
  BiTree<int> t(7);
  TestAccess<BiTree<int>,int> a_t;
  a_t._bitree = &t;
  EXPECT_EQ(a_t._child().size(), 0);
  EXPECT_EQ(a_t._parent(), nullptr);
  EXPECT_EQ(a_t._level(), 1);
  ValHolder<int> v = *(a_t._pval());
  TestAccess<ValHolder<int>, int> a_v;
  a_v._valholder = &v;
  EXPECT_EQ(a_v._val(), 7);
  EXPECT_EQ(a_v._pos(), a_t._bitree);
}

TEST(BiTree, rightCallEat)
{
  std::unique_ptr<BiTree<int> > a (new BiTree<int> (8));
  
  TestAccess<BiTree<int>,int> a_a;
  a_a._bitree = &(*a);
  ValHolder<int> k = *(a_a._pval());
  TestAccess<ValHolder<int>, int> a_k;
  a_k._valholder = &k;
  EXPECT_EQ(a_k._val(), 8);
  EXPECT_EQ(a_k._pos(), a_a._bitree);
  
  std::unique_ptr<BiTree<int> > b (new BiTree<int> (7));
  bool flag = a->eat(b);
  EXPECT_EQ(flag, 1);
  EXPECT_EQ(b, nullptr);
  EXPECT_EQ(a_a._level(), 2);
  EXPECT_EQ(a_a._parent(), nullptr);
  EXPECT_EQ(a_a._child().size(), 1);
  TestAccess<ValHolder<int>,int> a_v;
  a_v._valholder = &(*a_a._pval());
  EXPECT_EQ(a_v._val(), 8);
  EXPECT_EQ(a_v._pos(), a_a._bitree);
  TestAccess<BiTree<int>,int> a_b;
  a_b._bitree = &(*(a_a._child()[0]));
  EXPECT_EQ(a_b._level(), 1);
  EXPECT_EQ(a_b._parent(), a_a._bitree);
  EXPECT_EQ(a_b._child().size(), 0);
  TestAccess<ValHolder<int>,int> a_u;
  a_u._valholder = &(*a_b._pval());
  EXPECT_EQ(a_u._val(), 7);
  EXPECT_EQ(a_u._pos(), a_b._bitree);
}

TEST(BiTree, badCallEat)
{
  std::unique_ptr<BiTree<int> > a (new BiTree<int> (8));
  
  std::unique_ptr<BiTree<int> > d (new BiTree<int>);
  BiTree<int>* pd = &(*d);
  EXPECT_EQ(a->eat(d), 0);
  EXPECT_EQ(pd, &(*d));

  std::unique_ptr<BiTree<int> > e (new BiTree<int>);
  EXPECT_EQ(d->eat(e), 0);

  std::unique_ptr<BiTree<int> > b (new BiTree<int> (7));
  EXPECT_EQ(a->eat(b), 1);
  EXPECT_EQ(nullptr, &(*b));

  EXPECT_EQ(b->eat(a), 0);

  EXPECT_EQ(a->eat(b), 0);

  std::unique_ptr<BiTree<int> > c (new BiTree<int> (6));
  BiTree<int>* pc = &(*c);
  EXPECT_EQ(a->eat(c), 0);
  EXPECT_EQ(pc, &(*c));
}

TEST(BiTree, swapPtrs)
{
  std::unique_ptr<BiTree<int> > a (new BiTree<int> (8));
  std::unique_ptr<BiTree<int> > b (new BiTree<int> (7));
  swap(a,b);
  TestAccess<BiTree<int>,int> a_a;
  a_a._bitree = &(*a);
  ValHolder<int> v = *(a_a._pval());
  TestAccess<ValHolder<int>,int> a_v;
  a_v._valholder = &v;
  EXPECT_EQ(a_v._pos(), a_a._bitree);
  EXPECT_EQ(a_v._val(), 7);
  TestAccess<BiTree<int>,int> a_b;
  a_b._bitree = &(*b);
  ValHolder<int> u = *(a_b._pval());
  TestAccess<ValHolder<int>,int> a_u;
  a_u._valholder = &u;
  EXPECT_EQ(a_u._pos(), a_b._bitree);
  EXPECT_EQ(a_u._val(), 8);
  //заметь, что в этом тесте *v совсем не равен a._pval, это другая переменная
}