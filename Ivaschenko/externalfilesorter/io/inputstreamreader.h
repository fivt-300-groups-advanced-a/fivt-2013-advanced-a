#ifndef INPUTSTREAMREADER_H
#define INPUTSTREAMREADER_H

#include <set>
#include <type_traits>

#include "abstractreader.h"

/**
 * Class implementing AbstractReader interface
 * Used for reading fixed-type data from input stream
 *
 * TODO: specialising for floating point data types and std::strings
 */
template<typename DataType, typename = void> class InputStreamReader : public AbstractReader<DataType>
{
	public:
		/**
		 * Initialising from any input stream (stdin by default)
		 */
		explicit InputStreamReader(std::istream &in = std::cin)
		{
			this->stream = &in;
		}

		/**
		 * Reads an element
		 * Returns true in case of success, false otherwise
		 */
		bool operator() (DataType &element)
		{
			return this->ready() && (*this->stream >> element);
		}
};

/**
 * Specification of InputStreamReader for all integer types
 * Features:
 *		- Reading sequences separated by adjustable delimeters
 *		- Reading non-decimal integers
 * TODO: reading hex in 0x%d format and oct in 0%d format
 */
template<typename IntegerType> class InputStreamReader
		<IntegerType, typename std::enable_if< std::is_integral<IntegerType>::value >::type>
			: public AbstractReader<IntegerType>
{
	public:
		/**
		 * Initialising from any input stream, default radix is 10.
		 * Defualt delimeters are space and eoln
		 */
		explicit InputStreamReader(std::istream &in = std::cin)
		{
			this->stream = &in;
			radix = 10u;
			resetDelimeters();
			addDelimeter(' ');
			addDelimeter('\n');
		}


		/**
		 * Read an element. Reads input char by char searching of group of consecutive digits
		 * surrounded by delimeters. Returns true in case of success, false otherwise
		 */
		bool operator() (IntegerType &number)
		{
			if (!this->ready()) return false;
			char c = 0;
			while (true)
			{
				while (c == 0 || isDelimeter(c))
					if (!this->stream->read(&c, 1)) return false;

				bool minus = c == '-' && std::is_signed<IntegerType>::value;
				if (minus && !this->stream->read(&c, 1)) return false;
				if (digitValue(c) >= 0)
				{
					number = 0;
					while (digitValue(c) >= 0)
					{
						number = number * (IntegerType) radix + (IntegerType) digitValue(c);
						if (!this->stream->read(&c, 1)) return true; // Last element before EOF
					}
					if (isDelimeter(c))
					{
						if (minus) number = -number;
						return true;
					}
					else continue;
				}

				while (!isDelimeter(c))
					if (!this->stream->read(&c, 1)) return false;
			}
		}

		/**
		 * Reads sequence of numbers by two output iterators
		 * Returns true in case of success, false otherwise
		 */
		template<class OutputIterator> bool operator () (OutputIterator begin, OutputIterator end)
		{
			for (; begin != end; ++begin)
				if (!this->operator() (*begin)) return false;
			return true;
		}

		/**
		 * Returns current radix
		 */
		unsigned int getRadix() const
		{
			return radix;
		}

		/**
		 * Set a radix (if it is valid, e.g. 1 < radix <= 36)
		 */
		void setRadix(unsigned int nRadix)
		{
			if (isValidRadix(nRadix)) radix = nRadix;
		}

		/**
		 * Returns set of delimeters
		 */
		std::set<char> getDelimeters() const
		{
			std::set<char> answer;
			for (int i = 0; i < alphabet; ++i)
				if (delimeterMask[i])
					answer.insert(i);
			return answer;
		}

		void resetDelimeters()
		{
			for (int i = 0; i < alphabet; ++i)
				delimeterMask[i] = 0;
		}

		/**
		 * Applies a new set of delimeters
		 */
		template <typename ForwardIterator>
		void setDelimeters(ForwardIterator begin, ForwardIterator end)
		{
			resetDelimeters();
			for (; begin != end; ++begin)
				addDelimeter(*begin);
			if (std::is_signed<IntegerType>::value)
				removeDelimeter('-');
		}

		/**
		 * Checks whether c is delimeter
		 */
		bool isDelimeter(unsigned char c) const
		{
			return delimeterMask[c];
		}

		/**
		 * Adds delimeter to the set
		 */
		void addDelimeter(unsigned char c)
		{
			if (!std::is_signed<IntegerType>::value || c != '-')
				delimeterMask[c] = 1;
		}

		/**
		 * Removes delimeter from the set
		 */
		void removeDelimeter(unsigned char c)
		{
			delimeterMask[c] = 0;
		}

	private:
		static const int alphabet = 256;

		unsigned int radix;
		bool delimeterMask[alphabet];

		/**
		 * Checks whether radix is valid
		 */
		bool isValidRadix(unsigned int r)
		{
			return r >= 2 && r <= 36;
		}

		/**
		 * Returns integer value of digit and -1 if invalid character
		 */
		int digitValue(char c)
		{
			int value = isdigit(c) ? c - '0' : tolower(c) - 'a' + 10;
			return (0 <= value && value < (int) radix) ? value : -1;
			return -1;
		}
};


/**
 * Specification of InputStreamReader for all floating point types
 * TODO: Reading numbers in exponential form, decimal and common fractions
 * TODO: Multi-radix
 * TODO: Adjustable floating point character
 */
template<typename FloatType> class InputStreamReader
		<FloatType, typename std::enable_if< std::is_floating_point<FloatType>::value >::type>
			: public AbstractReader<FloatType>
{
	public:
		bool operator() (FloatType &number)
		{
			return this->ready();
		}
};


/**
 * Specification of InputStreamReader for std::strings
 * TODO: Reading tokens with adjustable set of delimeters, including line by line and word by word
 */
template<> class InputStreamReader
		<std::string, void>
			: public AbstractReader<std::string>
{
	public:
		bool operator() (std::string &element)
		{
			return this->ready();
		}
};


#endif // INPUTSTREAMREADER_H
