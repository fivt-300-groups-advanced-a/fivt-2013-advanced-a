#include "gtest/gtest.h"

#include "utils.h"

#include <string>
#include <vector>

#include "../src/EditDistance.h"

TEST(Manual, Strings)
{
    std::vector< std::pair<std::string, std::string> > cases =
    {
        {"", ""},
        {"", "some"},
        {"some", ""},
        {"a", ""},
        {"", "b"},
        {"a", "a"},
        {"abacaba", "dabacaba"},
        {"123", "456"},
        {"agtcagtcagtc", "agtccgtagtc"}
    };

    for (auto t : cases)
    {
        checkPrescription(t.first.begin(), t.first.end(),
                          t.second.begin(), t.second.end(), std::equal_to<char>());
    }
}
