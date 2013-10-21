#ifndef INTEGERBITBLOCKEXTRACTOR_H
#define INTEGERBITBLOCKEXTRACTOR_H

#include <type_traits>
#include <numeric>

/**
 * A class implementing BitBlockExtractor interfacem which is able to divide all integer types
 * into part of 2 bits from less significant to the most which is useful for digital sort
 */
template<typename ItegerType, typename = void>
class IntegerBitBlockExtractor {};

/**
 * IntegerBitBlockExtractor for unsigned types (pretty obvious)
 */
template<typename UnsignedInteger> class IntegerBitBlockExtractor
		<UnsignedInteger, typename std::enable_if< !std::numeric_limits<UnsignedInteger>::is_signed >::type>

{
	public:
		std::size_t getBlocksNumber() const
		{
			return sizeof(UnsignedInteger) >> 1;
		}

		std::size_t getBlockRange(std::size_t block) const
		{
			return 1 << 16;
		}

		template<typename ForwardIterator>
		IntegerBitBlockExtractor(ForwardIterator begin, ForwardIterator last) {}

		std::size_t operator() (UnsignedInteger value, std::size_t block)
		{
			return (value >> (block << 4)) & 0xFFFF;
		}
};

/**
 * IntegerBitBlockExtractor for signed types is a bit more complicated
 */
template<typename SignedInteger> class IntegerBitBlockExtractor
		<SignedInteger, typename std::enable_if< std::numeric_limits<SignedInteger>::is_signed >::type>

{
	public:
		std::size_t getBlocksNumber() const
		{
			return sizeof(SignedInteger) >> 1;
		}

		std::size_t getBlockRange(std::size_t block) const
		{
			return 1 << 16;
		}

		template<typename ForwardIterator>
		IntegerBitBlockExtractor(ForwardIterator begin, ForwardIterator end) {}

		std::size_t operator() (SignedInteger value, std::size_t block)
		{
			value ^= ((SignedInteger) 1LL << (sizeof(SignedInteger) * 8 - 1));
			return (value >> (block << 4)) & ((SignedInteger) 0xFFFF);
		}
};

#endif // INTEGERBITBLOCKEXTRACTOR_H
