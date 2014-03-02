#include <sstream>

#include "gtest/gtest.h"

#include "src/io/ostreamwriter.h"

#include "../complexdata.h"

TEST(OStreamWriter, Integers)
{
    std::ostringstream oss;

    do
    {
        OStreamWriter<int> write(oss);

        for (int x : {54, 23, 12, 0, -3, -213123, 43})
            write(x);
    } while (false);

    EXPECT_EQ("54 23 12 0 -3 -213123 43 ", oss.str());
}

TEST(OStreamWriter, Strings)
{
    std::ostringstream oss;

    do
    {
        OStreamWriter<std::string> write(oss);

        for (std::string s : {"54", "123", "wer", "\t\n", "~\012rew"})
            write(s);
    } while (false);

    EXPECT_EQ("54\n123\nwer\n\t\n\n~\012rew\n", oss.str());
}

TEST(OStreamWriter, ComplexData)
{
    std::ostringstream oss;

    static std::vector<ComplexData> data =
    {
        ComplexData(.41, ".41"),
        ComplexData(0, "3.34e7"),
        ComplexData(-433, "123"),
        ComplexData(.9e-4, "some")
    };

    do
    {
        OStreamWriter<ComplexData> write(oss);

        for (ComplexData d : data)
            write(d);
    } while (false);

    EXPECT_EQ("0.41\n.41\n0\n3.34e7\n-433\n123\n9e-005\nsome\n", oss.str());
}
