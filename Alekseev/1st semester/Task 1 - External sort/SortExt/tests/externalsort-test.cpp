#include "gtest/gtest.h"

#include "src/externalsort.h"

#include "complexdata.h"

namespace
{
template<typename DataT>
class VectorReader
{
public:
    VectorReader(const std::vector<DataT> &init):
        buffer(init)
    {
    }
    bool operator() (DataT &some)
    {
        if (buffer.empty())
            return false;
        some = buffer.back();
        buffer.pop_back();
        return true;
    }
protected:
    std::vector<DataT> buffer;
};

template<typename DataT>
class ShuffledVectorReader : public VectorReader<DataT>
{
public:
    ShuffledVectorReader(int seed, const std::vector<DataT> &init):
        VectorReader<DataT>(init)
    {
        srand(seed);
        std::random_shuffle(this->buffer.begin(), this->buffer.end());
    }
};

template<typename DataT>
class VectorWriter
{
public:
    void operator() (const DataT &some)
    {
        buffer.push_back(some);
    }
    const std::vector<DataT> &contents() const
    {
        return buffer;
    }

private:
    std::vector<DataT> buffer;
};
}

TEST(ExternalSort, LocallyGeneratedIntegersPermutation)
{
    ExternalSorter<int> sorter;

    std::vector<int> data;
    for (int i = 0; i < 500000; ++i)
        data.push_back(i);
    ShuffledVectorReader<int> reader(3, data);
    VectorWriter<int> writer;

    ASSERT_TRUE(sorter.sort(reader, writer, 10000));

    std::vector<int> expected = data;
    sort(expected.begin(), expected.end());

    EXPECT_EQ(expected, writer.contents());
}

TEST(ExternalSort, ManuallyGeneratedComplexData)
{
    ExternalSorter<ComplexData, ComplexDataComparator> sorter;

    std::vector<ComplexData> data =
    {
        ComplexData(3.1415926, "pi"),
        ComplexData(3.1415926, "m_pi"),
        ComplexData(3.14, "some"),
        ComplexData(54e200, "huge"),
        ComplexData(0., "zero"),
        ComplexData(3.1415926, "pi"),
        ComplexData(-0., "another zero")
    };

    VectorReader<ComplexData> reader(data);
    VectorWriter<ComplexData> writer;

    ASSERT_TRUE(sorter.sort(reader, writer, 2));

    std::vector<ComplexData> expected = data;
    sort(expected.begin(), expected.end(), ComplexDataComparator());

    ASSERT_EQ(expected.size(), writer.contents().size());
    for (size_t i = 0; i < expected.size(); ++i)
    {
        EXPECT_FLOAT_EQ(expected[i].d, writer.contents()[i].d);
        EXPECT_EQ(expected[i].s, writer.contents()[i].s);
    }
}
