#include "gtest/gtest.h"
#include "binomtree.h"

TEST(BinomTree, set_value)
{
    std::unique_ptr<BinomTree<int> > a (new BinomTree<int>);
    a->set_value(4);
    EXPECT_EQ(a->get_value(), 4);
    a->set_value(3);
    EXPECT_EQ(a->get_value(), 3);
}

TEST(BinomTree, absorb)
{
	std::unique_ptr<BinomTree<int> > a (new BinomTree<int>);
    a->set_value(4);
    std::unique_ptr<BinomTree<int> > b (new BinomTree<int>);
    b->set_value(3);
    a->absorb(b);
    EXPECT_EQ(b, nullptr);
    EXPECT_EQ(a->get_value(), 4);
}