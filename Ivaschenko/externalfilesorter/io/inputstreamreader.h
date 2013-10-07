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
 * FIXME: '-' symbol is processed incorrectly!
 */
template<typename IntegerType> class InputStreamReader
		<IntegerType, typename std::enable_if< std::is_integral<IntegerType>::value >::type>
			: public AbstractReader<IntegerType>
{
	public:
		/**
		 * Initialising from any input stream, default radix is 10, defualt delimeter is space and eoln
		 */
		explicit InputStreamReader(std::istream &in = std::cin)
		{
			this->stream = &in;
			radix = 10u;
			addDelimeter(' ');
			addDelimeter('\n');
		}


		/**
		 * Read an element. Reads input char by char searching of group of consecutive digits surrounded by delimeters
		 * Returns true in case of success, false otherwise
		 */
		bool operator() (IntegerType &number)
		{
			if (!this->ready()) return false;
			while (true)
			{
				char c;

				do if (!this->stream->read(&c, 1)) return false;
				while (isDelimeter(c));

				if (digitValue(c) >= 0)
				{
					number = 0;
					while (digitValue(c) >= 0)
					{
						number = number * (IntegerType) radix + (IntegerType) digitValue(c);
						if (!this->stream->read(&c, 1)) return true; // Last element before EOF
					}
					if (isDelimeter(c)) return number;
					else continue;
				}

				do if (!this->stream->read(&c, 1)) return false;
				while (!isDelimeter(c));
			}
		}

		/**
		 * Reads sequence of numbers by two output iterators
		 * Returns true in case of success, false otherwise
		 */
		template<class OutputIterator> bool operator ()
			(
				OutputIterator begin, OutputIterator end
			)
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
			return delimeters;
		}

		/**
		 * Applies a new set of delimeters
		 */
		void setDelimeters(const std::set<char> &nDelimeters)
		{
			delimeters = nDelimeters;
		}

		/**
		 * Checks whether c is delimeter
		 */
		bool isDelimeter(char c) const
		{
			return delimeters.count(c);
		}

		/**
		 * Adds delimeter to the set
		 */
		void addDelimeter(char c)
		{
			delimeters.insert(c);
		}

		/**
		 * Removes delimeter from the set
		 */
		void removeDelimeter(char c)
		{
			delimeters.erase(c);
		}

	private:
		unsigned int radix;
		std::set<char> delimeters;

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
 * TODO:
 *		-> Reading numbers in exponential, decimal and common fractions
 *		-> Multi-radix
 *		-> Adjustable floating point character
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
 * TODO:
 *		-> Reading string with adjustable set of delimeters
 *		-> Including line by line and word by word
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
