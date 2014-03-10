#include <iostream>
#include <functional>
#include <sstream>

#include "gtest/gtest.h"

#include "../src/EditDistance.h"

template<class RAIter, class Equal>
std::vector<std::string> printablePrescription(RAIter srcBegin, RAIter srcEnd,
                                               RAIter destBegin, RAIter destEnd,
                                               Equal equal)
{
    std::vector<EditInstruction> pre = editPrescription(srcBegin, srcEnd, destBegin, destEnd, equal);

    std::vector<std::string> ret;

    for (EditInstruction i : pre)
    {
        std::ostringstream ss;
        switch (i.type) {
        case EditInstruction::INSERT:
            ss << "Insert " << *(destBegin + i.newCharPosition)
               << " at position " << i.oldCharPosition;
            break;
        case EditInstruction::ERASE:
            ss << "Erase symbol at position " << i.oldCharPosition;
            break;
        case EditInstruction::REPLACE:
            ss << "Replace symbol at position " << i.oldCharPosition
               << " with '" << *(destBegin + i.newCharPosition) << "'";
            break;
        }
        ret.push_back(ss.str());
    }

    return ret;
}

TEST(Fancy, Manual)
{
    std::string a = "agtcagtcagtc", b = "agtccgtagtc";

    for (std::string s : printablePrescription(a.begin(), a.end(), b.begin(), b.end(), std::equal_to<char>()))
        std::cerr << s << "\n";
}
