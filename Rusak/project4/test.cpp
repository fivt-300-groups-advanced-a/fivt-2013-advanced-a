
#include "gtest/gtest.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "lists_of_incidents.h"
//#include "factories.h"
//#include "graph.h"

typedef std::unique_ptr<ListOfIncidents> list_ptr;
typedef std::unique_ptr<ListIterator> iterator_ptr;

TEST(Prepare, GenerateRandom) {
  srand(time(NULL));
}

TEST(ListOfIncedents, CheckCorrectness) {
  const int tests = 20;
  const int size = 100;
  for (int te=0;te<tests;te++) {
    std::vector<unsigned int> q;
    int col = size+rand()%size;
    int ma = 0;
    for (int i=0;i<col;i++) {
      ma = ma+rand()%3+1;
      q.push_back(ma);
    }

    list_ptr bit(new BitsetList(q));
    list_ptr vect(new VectorList(q));
    iterator_ptr it_bit = bit->get_iterator();
    iterator_ptr it_vect = vect->get_iterator();
    int pos = 0;
    std::vector<bool> was_bit, was_vect;
    was_bit.resize(400);
    was_vect.resize(400);
    while (it_bit->valid()) {
      EXPECT_TRUE(it_vect->valid());
      was_bit[it_bit->current()] = true;
      was_vect[it_vect->current()] = true;
      it_vect->next();
      it_bit->next();
      pos++;
    }
    for (int i=0;i<400;i++) {
      EXPECT_EQ(was_bit[i], was_vect[i]);
      EXPECT_EQ(was_bit[i], find(q.begin(), q.end(), i)!=q.end());
    }
    EXPECT_FALSE(it_bit->valid());
    EXPECT_FALSE(it_vect->valid());
    EXPECT_EQ(pos, (int)q.size());
  }
}
