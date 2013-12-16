#include <gtest/gtest.h>

struct ReturnType
{
	public:
		int sum, maxPrefix, maxSuffix, maxSubseg;
		std::size_t prefPos, suffPos, segLeft, segRight, size;
		// all bounds are exclusive!
};

struct MetaInformation
{
	public:
		int value;
};

struct Function
{
	public:
		void operator () (ReturnType &a, const ReturnType &b)
		{
			if (b.maxSubseg > a.maxSubseg)
			{
				a.maxSubseg = b.maxSubseg;
				a.segLeft = b.segLeft;
				a.segRight = b.segRight;
			}
			if (a.maxSuffix + b.maxPrefix > a.maxSubseg)
			{
				a.maxSubseg = a.maxSuffix + b.maxSuffix;
				a.segLeft = a.suffPos;
				a.segRight = b.prefPos;
			}

			if (b.maxPrefix + a.sum > a.maxPrefix)
			{
				a.maxPrefix = b.maxPrefix + a.sum;
				a.prefPos = b.prefPos;
			}

			if (a.maxSuffix + b.sum > a.maxSuffix)
			{
				a.maxSuffix = a.maxSuffix + b.sum;
				a.suffPos = b.suffPos;
			}

			a.sum += b.sum;
			a.size += b.size;
		}
};

struct MetaUpdater
{
	public:
		void operator () (ReturnType &value, const MetaInformation &info,
						  std::size_t left, std::size_t right)
		{
			value.size = 1;
			value.sum = value.size * info.value;

			if (info.value < 0)
			{
				value.maxPrefix = value.maxSuffix = value.maxSubseg = 0;
				value.prefPos = left - 1, value.suffPos = right;
			}
			else
			{
				value.maxPrefix = value.maxSuffix = value.maxSubseg = value.sum;
				value.prefPos = right + 1, value.suffPos = left - 1;
			}
		}
};

struct MetaMerger
{
	public:
		void operator() (MetaInformation &a, const MetaInformation &b)
		{
			a.value = b.value;
		}
};

TEST(MaximalSumSubSegment, Integration)
{
	// TODO: find subsegment with maximal sum

}
