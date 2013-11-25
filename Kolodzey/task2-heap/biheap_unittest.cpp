#include <utility>
#include <string>
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
    BiHeap<Value, Compare>* _biheap;
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

/*
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
*/

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
  
  /*
  std::unique_ptr<BiTree<int> > d (new BiTree<int>);
  BiTree<int>* pd = &(*d);
  EXPECT_EQ(a->eat(d), 0);
  EXPECT_EQ(pd, &(*d));
  
  std::unique_ptr<BiTree<int> > e (new BiTree<int>);
  EXPECT_EQ(d->eat(e), 0);
  */

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
  a_b._bitree = &(*b); //жуткая конструкция, заменить на get
  ValHolder<int> u = *(a_b._pval());
  TestAccess<ValHolder<int>,int> a_u;
  a_u._valholder = &u;
  EXPECT_EQ(a_u._pos(), a_b._bitree);
  EXPECT_EQ(a_u._val(), 8);
  //заметь, что в этом тесте *v совсем не равен a._pval, это другая переменная
}

TEST(BiTreeFunc, rehang)
{
  BiTree<char> t('a');
  BiTree<char> s('b');
  ValPointer<char> pa = t.get_pval();
  ValPointer<char> pb = s.get_pval();
  BiTreeFunc<char>::rehang(pa, pb);

  TestAccess<BiTree<char>,char> a_t;
  a_t._bitree = &t;
  ValHolder<char>* b = (a_t._pval()).get();
  TestAccess<ValHolder<char>,char> a_b;
  a_b._valholder = b;
  EXPECT_EQ(a_b._pos(), a_t._bitree);
  EXPECT_EQ(a_b._val(), 'b');
  TestAccess<ValPointer<char>,char> a_pb;
  a_pb._valpointer = &pb;
  EXPECT_EQ(a_pb._ref(), b);

  TestAccess<BiTree<char>,char> a_s;
  a_s._bitree = &s;
  ValHolder<char>* a = (a_s._pval()).get();
  TestAccess<ValHolder<char>,char> a_a;
  a_a._valholder = a;
  EXPECT_EQ(a_a._pos(), a_s._bitree);
  EXPECT_EQ(a_a._val(), 'a');
  TestAccess<ValPointer<char>,char> a_pa;
  a_pa._valpointer = &pa;
  EXPECT_EQ(a_pa._ref(), a);
}

TEST(BiTree, is_son)
{
  std::unique_ptr<BiTree<std::string> > t (new BiTree<std::string>("Sasha"));
  ValPointer<std::string> vpSasha = t->get_pval();
  std::unique_ptr<BiTree<std::string> > s (new BiTree<std::string>("Masha"));
  ValPointer<std::string> vpMasha = s->get_pval();
  EXPECT_EQ(t->is_son(vpSasha), 1);
  EXPECT_EQ(s->is_son(vpMasha), 1);
  EXPECT_EQ(s->is_son(vpSasha), 0);
  EXPECT_EQ(t->is_son(vpMasha), 0);
  BiTreeFunc<std::string>::rehang(vpMasha, vpSasha);
  EXPECT_EQ(t->is_son(vpSasha), 0);
  EXPECT_EQ(s->is_son(vpMasha), 0);
  EXPECT_EQ(s->is_son(vpSasha), 1);
  EXPECT_EQ(t->is_son(vpMasha), 1);
  s->eat(t); //t is nullptr now
  EXPECT_EQ(t->is_son(vpSasha), 0);
  EXPECT_EQ(s->is_son(vpMasha), 1);
  EXPECT_EQ(s->is_son(vpSasha), 1);
  EXPECT_EQ(t->is_son(vpMasha), 0);
  EXPECT_EQ(s->get_val(), "Sasha");
}

TEST(BiTree, lift)
{
  std::unique_ptr<BiTree<std::string> > t (new BiTree<std::string>("Sasha"));
  std::unique_ptr<BiTree<std::string> > s (new BiTree<std::string>("Masha"));
  ValPointer<std::string> vpMasha = s->get_pval();
  t->eat(s);
  EXPECT_EQ(t->get_val(), "Sasha");
  t->lift(vpMasha);
  EXPECT_EQ(t->get_val(), "Masha");
  TestAccess<BiTree<std::string>,std::string> a_t;
  a_t._bitree = t.get();
  EXPECT_EQ((a_t._child()[0])->get_val(), "Sasha");
}

TEST(BiTreeFunc, merge)
{
  std::unique_ptr<BiTree<int> > a (new BiTree<int> (8));
  std::unique_ptr<BiTree<int> > b (new BiTree<int> (7));
  std::unique_ptr<BiTree<int> > c (nullptr);
  BiTreeFunc<int>::merge(a, b, c, std::less<int>());
  EXPECT_EQ(a, nullptr);
  EXPECT_EQ(b, nullptr);
  EXPECT_EQ(c->get_val(), 7);
  TestAccess<BiTree<int>,int> access_c;
  BiTree<int>* temp = c.get();
  access_c._bitree = temp;
  EXPECT_EQ((access_c._child()[0])->get_val(), 8);
}

TEST(BiTreeFunc, cutroot)
{
  std::unique_ptr<BiTree<int> > a (new BiTree<int> (8));
  std::unique_ptr<BiTree<int> > b (new BiTree<int> (7));
  std::unique_ptr<BiTree<int> > d (new BiTree<int> (9));
  std::unique_ptr<BiTree<int> > e (new BiTree<int> (11));
  a->eat(b);
  d->eat(e);
  a->eat(d);
  std::list<std::unique_ptr<BiTree<int> > > chlist;
  BiTreeFunc<int>::cutroot(a, chlist);
  EXPECT_EQ(a, nullptr);
  EXPECT_EQ((chlist.front())->get_val(), 7);
  EXPECT_EQ((chlist.back())->get_val(), 9);
  TestAccess<BiTree<int>,int> access_ch1;
  access_ch1._bitree = (chlist.back()).get();
  EXPECT_EQ(access_ch1._level(), 2);
  EXPECT_EQ(((access_ch1._child())[0])->get_val(), 11);
}

TEST(BiHeap, Constructor)
{
  BiHeap<int,std::greater<int> > p((std::greater<int>()));
  //указывть тип компаратора обязательно, по переданному значению он его не определить
  TestAccess<BiHeap<int, std::greater<int> >,int> access_p;
  access_p._biheap = &p;
  EXPECT_EQ(access_p._forest().size(), 0);
}

TEST(BiTreeFunc, cmp_levels)
{
  std::unique_ptr<BiTree<int> > a (new BiTree<int> (8));
  std::unique_ptr<BiTree<int> > b (new BiTree<int> (7));
  std::unique_ptr<BiTree<int> > d (new BiTree<int> (9));
  std::unique_ptr<BiTree<int> > e (nullptr);
  EXPECT_EQ(BiTreeFunc<int>::cmp_levels(a, b), 0);
  EXPECT_EQ(BiTreeFunc<int>::cmp_levels(b, a), 0);
  a->eat(b);
  EXPECT_EQ(BiTreeFunc<int>::cmp_levels(a, b), 0);
  EXPECT_EQ(BiTreeFunc<int>::cmp_levels(b, a), 1);
  EXPECT_EQ(BiTreeFunc<int>::cmp_levels(a, d), 0);
  EXPECT_EQ(BiTreeFunc<int>::cmp_levels(d, a), 1);
  EXPECT_EQ(BiTreeFunc<int>::cmp_levels(b, e), 0);
}

TEST(BiHeap, insert_list)
{
  BiHeap<int> heap;
  std::list<std::unique_ptr<BiTree<int> > > elem;
  std::unique_ptr<BiTree<int> > a(new BiTree<int> (8));
  elem.push_back(nullptr);
  swap(a, elem.back());
  EXPECT_EQ(a, nullptr);
  TestAccess<BiHeap<int,std::less<int> >,int>::insert_list(heap, elem);
  EXPECT_EQ(elem.size(), 0);
  TestAccess<BiHeap<int,std::less<int> >,int> access_heap;
  access_heap._biheap = &heap;
  EXPECT_EQ(access_heap._forest().back()->get_val(), 8);
  std::unique_ptr<BiTree<int> > b(new BiTree<int> (9));
  std::unique_ptr<BiTree<int> > c(new BiTree<int> (12));
  c->eat(b);
  elem.push_back(nullptr);
  elem.push_back(nullptr);
  swap(elem.back(), c);
  TestAccess<BiHeap<int,std::less<int> >,int>::insert_list(heap, elem);
  EXPECT_EQ(access_heap._forest().back()->get_val(), 12);
  EXPECT_EQ(access_heap._forest().front(), nullptr);
}

TEST(BiHeap, improve_list)
{
  BiHeap<int> heap;
  std::list<std::unique_ptr<BiTree<int> > > elem;
  std::unique_ptr<BiTree<int> > a (new BiTree<int> (1));
  std::unique_ptr<BiTree<int> > b (new BiTree<int> (2));
  std::unique_ptr<BiTree<int> > c (new BiTree<int> (3));
  std::unique_ptr<BiTree<int> > d (new BiTree<int> (4));
  std::unique_ptr<BiTree<int> > e (new BiTree<int> (5));
  std::unique_ptr<BiTree<int> > f (new BiTree<int> (6));
  std::unique_ptr<BiTree<int> > g (new BiTree<int> (7));
  std::unique_ptr<BiTree<int> > h (new BiTree<int> (8));
  c->eat(d);
  e->eat(f);
  g->eat(h);
  e->eat(g);
  elem.push_back(nullptr);
  swap(a, elem.back());
  elem.push_back(nullptr);
  swap(b, elem.back());
  elem.push_back(nullptr);
  swap(c, elem.back());
  elem.push_back(nullptr);
  swap(e, elem.back());
  TestAccess<BiHeap<int,std::less<int> >,int>::insert_list(heap, elem);
  TestAccess<BiHeap<int,std::less<int> >,int>::improve_list(heap);
  TestAccess<BiHeap<int,std::less<int> >,int> access_heap;
  access_heap._biheap = &heap;
  EXPECT_EQ(access_heap._forest().size(), 1);
  EXPECT_EQ(access_heap._forest().front()->get_val(), 1);
  EXPECT_EQ(access_heap._forest().front()->get_level(), 4);
  TestAccess<BiTree<int>,int> access_tree;
  access_tree._bitree  = access_heap._forest().front().get();
  EXPECT_EQ(access_tree._child().size(), 3);
  EXPECT_EQ(((access_tree._child())[0])->get_val(), 2);
  EXPECT_EQ(((access_tree._child())[0])->get_level(), 1);
  EXPECT_EQ(((access_tree._child())[1])->get_val(), 3);
  EXPECT_EQ(((access_tree._child())[1])->get_level(), 2);
  TestAccess<BiTree<int>,int> access_ch1;
  access_ch1._bitree  = (access_tree._child())[1].get();
  EXPECT_EQ((access_ch1._child()[0])->get_val(), 4);
  EXPECT_EQ(((access_tree._child())[2])->get_val(), 5);
  EXPECT_EQ(((access_tree._child())[2])->get_level(), 3);
  TestAccess<BiTree<int>,int> access_ch2;
  access_ch2._bitree  = (access_tree._child())[2].get();
  EXPECT_EQ(access_ch2._child()[0]->get_val(), 6);
  EXPECT_EQ(access_ch2._child()[1]->get_val(), 7);
}

TEST(BiHeap, insert)
{
  BiHeap<int> heap;
  ValPointer<int> p1 = heap.insert(1);
  heap.insert(2);
  heap.insert(4);
  heap.insert(3);
  TestAccess<BiHeap<int,std::less<int> >,int> access_heap;
  access_heap._biheap = &heap;
  EXPECT_EQ(access_heap._forest().size(), 1);
  EXPECT_EQ(access_heap._forest().front()->get_val(), 1);
  EXPECT_EQ(access_heap._forest().front()->get_level(), 3);
  TestAccess<BiTree<int>,int> access_tree;
  access_tree._bitree  = access_heap._forest().front().get();
  EXPECT_EQ(access_tree._child().size(), 2);
  EXPECT_EQ(((access_tree._child())[0])->get_val(), 2);
  EXPECT_EQ(((access_tree._child())[0])->get_level(), 1);
  EXPECT_EQ(((access_tree._child())[1])->get_val(), 3);
  EXPECT_EQ(((access_tree._child())[1])->get_level(), 2);
  TestAccess<BiTree<int>,int> access_ch1;
  access_ch1._bitree  = (access_tree._child())[1].get();
  EXPECT_EQ((access_ch1._child()[0])->get_val(), 4);
  EXPECT_EQ(p1.get_val(), 1);
}

TEST(BiHeap, eat)
{
  BiHeap<int> heap1;
  heap1.insert(1);
  heap1.insert(2);
  heap1.insert(4);
  heap1.insert(3);
  BiHeap<int> heap2;
  heap2.insert(5);
  heap2.insert(6);
  heap2.insert(7);
  heap2.insert(8);
   heap1.eat(heap2);
  EXPECT_EQ(heap2.is_empty(), 1);
  EXPECT_EQ(heap1.is_empty(), 0);
  TestAccess<BiHeap<int,std::less<int> >,int> access_heap;
  access_heap._biheap = &heap1;
  EXPECT_EQ(access_heap._forest().size(), 1);
  EXPECT_EQ(access_heap._forest().front()->get_val(), 1);
  EXPECT_EQ(access_heap._forest().front()->get_level(), 4);
  TestAccess<BiTree<int>,int> access_tree;
  access_tree._bitree  = access_heap._forest().front().get();
  EXPECT_EQ(access_tree._child().size(), 3);
  EXPECT_EQ(((access_tree._child())[0])->get_val(), 2);
  EXPECT_EQ(((access_tree._child())[0])->get_level(), 1);
  EXPECT_EQ(((access_tree._child())[1])->get_val(), 3);
  EXPECT_EQ(((access_tree._child())[1])->get_level(), 2);
  TestAccess<BiTree<int>,int> access_ch1;
  access_ch1._bitree  = (access_tree._child())[1].get();
  EXPECT_EQ((access_ch1._child()[0])->get_val(), 4);
  EXPECT_EQ(((access_tree._child())[2])->get_val(), 5);
  EXPECT_EQ(((access_tree._child())[2])->get_level(), 3);
  TestAccess<BiTree<int>,int> access_ch2;
  access_ch2._bitree  = (access_tree._child())[2].get();
  EXPECT_EQ(access_ch2._child()[0]->get_val(), 6);
  EXPECT_EQ(access_ch2._child()[1]->get_val(), 7);
}

TEST(BiHeap, get_top_ref)
{
  BiHeap<int> heap;
  ValPointer<int> ref0 = heap.get_top_ref();
  TestAccess<ValPointer<int>,int> access_ref0;
  access_ref0._valpointer = &ref0;
  EXPECT_EQ(access_ref0._ref(), nullptr);
  heap.insert(5);
  EXPECT_EQ(heap.get_top_ref().get_val(), 5);
  heap.insert(6);
  EXPECT_EQ(heap.get_top_ref().get_val(), 5);
  heap.insert(2);
  EXPECT_EQ(heap.get_top_ref().get_val(), 2);
}

TEST(BiHeap, top)
{
  BiHeap<int> heap;
  heap.insert(5);
  EXPECT_EQ(heap.top(), 5);
  heap.insert(6);
  EXPECT_EQ(heap.top(), 5);
  heap.insert(2);
  EXPECT_EQ(heap.top(), 2);
}

TEST(BiHeap, erase)
{
  BiHeap<int> heap;
  heap.insert(1);
  heap.insert(2);
  heap.insert(3);
  heap.insert(4);
  heap.insert(5);
  ValPointer<int> p6 = heap.insert(6);
  heap.insert(7);
  heap.insert(8);
  heap.erase(p6);
  TestAccess<BiHeap<int,std::less<int> >,int> access_heap;
  access_heap._biheap = &heap;
  EXPECT_EQ(access_heap._forest().size(), 3);
  BiHeap<int>::Itlist it = access_heap._forest().begin();
  EXPECT_EQ((*it)->get_val(),2);
  ++it;
  EXPECT_EQ((*it)->get_val(),3);
  ++it;
  EXPECT_EQ((*it)->get_val(),1);
}

TEST(BiHeap, top_unusualcmp)
{
  BiHeap<int,std::greater<int> > heap((std::greater<int>()));
  heap.insert(5);
  EXPECT_EQ(heap.top(), 5);
  heap.insert(6);
  EXPECT_EQ(heap.top(), 6);
  heap.insert(2);
  EXPECT_EQ(heap.top(), 6);
}