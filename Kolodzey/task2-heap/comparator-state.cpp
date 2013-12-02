#include "gtest/gtest.h"
#include "biheap.h"
#include "access.h"

struct Ptr
{
  Ptr()
  {
    _x = 0;
    _y = 0;
  }
  Ptr(long long x, long long y)
  {
    _x = x;
    _y = y;
  }
	long long _x;
	long long _y;
};

class DistCompare
{
public:
  DistCompare(){
    _p = Ptr(0,0);
  }
	DistCompare(Ptr p){
    _p = p;
  }

	bool operator ()(const Ptr& a, const Ptr& b) const
	{
		return ((a._x - _p._x) * (a._x - _p._x) + (a._y - _p._y) * (a._y - _p._y)) <
			     ((b._x - _p._x) * (b._x - _p._x) + (b._y - _p._y) * (b._y - _p._y));
	}
	Ptr _p;
};

TEST(BiHeap, statecompare)
{
	BiHeap<Ptr, DistCompare> from0_0 (DistCompare(Ptr(0, 0)));
  BiHeap<Ptr, DistCompare> from7_3 (DistCompare(Ptr(7, 3)));
  from0_0.insert(Ptr(0,3));
  from7_3.insert(Ptr(0,3));
  from0_0.insert(Ptr(6,-2));
  from7_3.insert(Ptr(6,-2));
  EXPECT_EQ(from0_0.top()._x, 0);
  EXPECT_EQ(from0_0.top()._y, 3);
  EXPECT_EQ(from7_3.top()._x, 6);
  EXPECT_EQ(from7_3.top()._y, -2);
}