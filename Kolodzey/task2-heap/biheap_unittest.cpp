#include <utility>
#include "gtest/gtest.h"
#include "biheap.h"

template <class Value>
class TestAccess <ValHolder<Value>, Value>{
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
class TestAccess <ValPointer<Value>, Value>{
  public:
    ValHolder<Value>*& _ref()
    {
      return _valpointer->_ref;
    }
    ValPointer<Value>*_valpointer;
};


TEST(ValHolder, Constructor)
{
  ValHolder<int> v(7);
  TestAccess<ValHolder<int>, int> v_access;
  v_access._valholder = &v;
  EXPECT_EQ(v_access._val(), 7);
  BiTree<int>* p = NULL;
  EXPECT_EQ(v_access._pos(), p);
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
  ValHolder<std::pair<int, char> >* pnull = NULL;
  EXPECT_EQ(a_pu._ref(), pnull);

  ValHolder<std::pair<int, char> > v(std::make_pair(3, 'a'));
  ValPointer<std::pair<int, char> > pv(&v);
  TestAccess<ValPointer<std::pair<int, char> >, std::pair<int, char> > a_pv;
  a_pv._valpointer = &pv;
  EXPECT_EQ(a_pv._ref(), &v);
}