#include <gtest/gtest.h>
#include "sort.h"
#include "binary_file_io.h"
#include "istream_io.h"
#include <cstdio>
#include <utility>
#include <list>
#include <ctime>
#include <cstdlib>

int intlistsize(std::list<int> * t) {
    return t->size();
}

template<class T>
class DEBUG_RW: public Reader<T>, public Writer<T> {
private:
    std::list<T> q;
public:
    virtual bool eos() {
        return q.empty();
    }
    virtual Reader<T> & operator >>(T & x) {
        if (eos()) return *this;
        x = q.front();
        q.pop_front();
        return *this;
    }
    virtual Writer<T> & operator <<(const T & x) {
        q.push_back(x);
        return *this;
    }
    int size() {
        return q.size();
    }
};

template<class T, class Cmp>
bool sortedstream(Reader<T> & ccin, int expsize, Cmp * cmp) {
    bool fs = true;
    T x;
    while (!ccin.eos()) {
        T y; ccin >> y; expsize--;
        if (!fs) {
            if (!(*cmp)(x, y)) return false;
        }
    }
    return expsize == 0;
}

TEST(Binary_File_io, Open_Close) {
    BFReader<int> inp;
    for (int i = 0; i < 60; i++) {
        ASSERT_NO_THROW(inp.open("tempfile"));
        ASSERT_NO_THROW(inp.close());
    }
    BFWriter<int> outp;
    for (int i = 0; i < 60; i++) {
        ASSERT_NO_THROW(outp.open("tempfile"));
        ASSERT_NO_THROW(outp.close());
    }
    remove("tempfile");
}

TEST(Binary_File_io, Int) {
    BFWriter<int> outp("tempfile");
    outp << 1 << 2 << 3;
    outp.close();
    ASSERT_NO_THROW(outp.close());
    BFReader<int> inp("tempfile");
    int zz[3];
    sort_utils::readBlock(&inp, zz, 3 * sizeof(int));
    EXPECT_FALSE(inp.eos());
    EXPECT_EQ(zz[0], 1);
    EXPECT_EQ(zz[1], 2);
    EXPECT_EQ(zz[2], 3);
    inp >> zz[0];
    EXPECT_TRUE(inp.eos());
    ASSERT_NO_THROW(inp.close());
    ASSERT_NO_THROW(inp.close());
    remove("tempfile");
}

TEST(Binary_File_io, pair_int_int) {
    BFWriter<std::pair<int, int> > outp("tempfile");
    outp << std::make_pair(1, 3);
    ASSERT_NO_THROW(outp.close());
    ASSERT_NO_THROW(outp.close());
    BFReader<std::pair<int, int> > inp("tempfile");
    std::pair<int, int> p; inp >> p;
    EXPECT_EQ(p, std::make_pair(1, 3));
    remove("tempfile");
}

TEST(Sorting, one_block) {
   DEBUG_RW<int> ii, io;
   srand(time(0));
   for (int i = 0; i < 6; i++)
        ii << rand();
   ASSERT_NO_THROW(bigSort(&ii, &io, new CInvOp<int, CLess<int> >()));
   EXPECT_TRUE((sortedstream<int, CInvOp<int, CLess<int> > >(io, 6, new CInvOp<int, CLess<int> >())));
}

TEST(Sorting, many_blocks) {
   DEBUG_RW<int> ii, io;
   srand(time(0));
   for (int i = 0; i < 8; i++)
        ii << rand();
   ASSERT_NO_THROW(bigSort(&ii, &io, new CInvOp<int, CLess<int> >(), 8));
   EXPECT_EQ(io.size(), 8);
   EXPECT_TRUE((sortedstream<int, CInvOp<int, CLess<int> > >(io, 8, new CInvOp<int, CLess<int> >())));
}

//slow performance test
/*
#define NINT 100000000

TEST(Sorting, int_test) {
    BFWriter<int> outp("tempfile");
    for (int i = 0; i < NINT; i++)
        outp << rand();
    outp.close();
    outp.open("new-outp");
    BFReader<int> inp("tempfile");
    ASSERT_NO_THROW(bigSort(&inp, &outp, new CInvOp<int, CLess<int> >()));
    outp.close();
    inp.close();
    inp.open("new-outp");
    ASSERT_TRUE(sortedstream(inp, NINT, new CInvOp<int, CLess<int> >()));
    remove("tempfile");
    remove("new-outp");
}
*/

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
