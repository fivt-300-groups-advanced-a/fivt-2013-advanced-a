#ifndef RADIX_SORT
#define RADIX_SORT

#include <vector>
#include <algorithm>
#include <memory>

namespace RadixSort
{
    /**
    * This function provide RadixSort
    * Arguments:
    *      begin - iterator on beginning of data
    *      end - iterator on ending of data
    *      extr - Extractor
    * Extractor must have:
    *           maxvalue - maximum value of Extract function
    *           iterationCount - count of iterations of sorting
    *           Extract - method to Extract digit from value
    */
    template<typename RandomAccessIterator, typename Extractor>
    inline void sort(RandomAccessIterator begin, RandomAccessIterator end, Extractor extr)
    {
        int *count = (int*)malloc(extr.maxValue * sizeof(int));
        int *id = (int*)malloc((end - begin) * sizeof(int));
        //std :: vector<int> count(extr.maxValue);
        //std :: vector<int> id(end - begin);
        for (int ibit = 0; ibit < extr.iterationCount; ++ibit)
        {
            memset(count, 0, extr.maxValue * sizeof(int));
            //count.assign(extr.maxValue, 0);
            for (RandomAccessIterator it = begin; it < end; ++it)
            {
                int bit = extr(*it, ibit);
                id[it - begin] = count[bit];
                ++count[bit];
            }

            for (int bit = 1; bit < extr.maxValue; ++bit)
                count[bit] += count[bit - 1];

            for (int bit = extr.maxValue - 1; bit > 0 ; --bit)
                count[bit] = count[bit - 1];
            count[0] = 0;

            for (RandomAccessIterator it = begin; it < end; ++it)
            {
                int num = count[(int)extr(*it, ibit)] + id[it - begin];
                while (it - begin != num)
                {
                    std :: swap(*it, *(begin + num));
                    std :: swap(id[it - begin], id[num]);
                    num = count[(int)extr(*it, ibit)] + id[it - begin];
                }
            }

        }
        delete[] count;
        delete[] id;
    }

    /**
    * This class is Simple int Extractor
    * It split int to bits
    */
    class SimpleIntExtractor
    {
    public:
        const unsigned int maxValue = 2;
        const unsigned int iterationCount = 32;

        inline unsigned int operator()(const int &i, const int id) const
        {
            if (id == 31)
                return (1 - ((i >> id) & 1));
            return ((i >> id) & 1);
        }
    };

    /**
    * This class is Simple int Extractor
    * It split int to bits
    */
    class SimpleUnsignedIntExtractor
    {
    public:
        const unsigned int maxValue = 2;
        const unsigned int iterationCount = 32;

        inline unsigned int operator()(const int &i, const int id) const
        {
            return ((i >> id) & 1);
        }
    };

    /**
    * This class is Fast int Extractor
    * It split to 2 parts for 16 bits
    */
    class FastIntExtractor
    {
    public:
        const unsigned int maxValue = (1 << 16);
        const unsigned int iterationCount = 2;

        inline unsigned int operator()(const int &i, const int id) const
        {
            if (id == 0)
                return i & ((1 << 16) - 1);
            return (((i >> 16) & ((1 << 16) - 1)) ^ (1 << 15));
        }
    };

    /**
    * This class is Fast unsigned int Extractor
    * It split to 2 parts for 16 bits
    */
    class FastUnsignedIntExtractor
    {
    public:
        const unsigned int maxValue = (1 << 16);
        const unsigned int iterationCount = 2;

        inline unsigned int operator()(const int &i, const int id) const
        {
            if (id == 0)
                return i & ((1 << 16) - 1);
            return ((i >> 16) & ((1 << 16) - 1));
        }
    };
}

#endif
