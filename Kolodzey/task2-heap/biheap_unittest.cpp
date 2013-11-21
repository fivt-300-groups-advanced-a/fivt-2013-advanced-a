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
  ValHolder<int> v = *a_t._pval();
  TestAccess<ValHolder<int>, int> a_v;
  a_v._valholder = &v;
  EXPECT_EQ(a_v._val(), 7);
  EXPECT_EQ(a_v._pos(), &t);
}