#ifndef INPUTSTREAMREADER_H
#define INPUTSTREAMREADER_H

#include <set>
#include <type_traits>

#include "abstractreader.h"

template<typename DataType, typename Enable = void> class InputStreamReader : public AbstractReader<DataType>
{
	public:
		explicit InputStreamReader(std::istream &in = std::cin)
		{
			this->stream = &in;
		}

		bool operator() (DataType &element)
		{
			return this->hasNext() && (*this->stream >> element);
		}
};

template<typename IntegerType> class InputStreamReader
		<IntegerType, typename std::enable_if< std::is_integral<IntegerType>::value >::type>
			: public AbstractReader<IntegerType>
{
	public:
		explicit InputStreamReader(std::istream &in = std::cin)
		{
			this->stream = &in;
			radix = 10u;
			addDelimeter(' ');
		}

		bool operator() (IntegerType &element)
		{
			while (true)
			{
				char c;

				do if (!this->stream->read(&c, 1)) return false;
				while (isDelimeter(c));

				if (digitValue(c) >= 0)
				{
					element = 0;
					while (digitValue(c) >= 0)
					{
						element = element * (IntegerType) radix + (IntegerType) digitValue(c);
						if (!this->stream->read(&c, 1)) return false;
					}
					if (isDelimeter(c)) return element;
					else continue;
				}

				do if (!this->stream->read(&c, 1)) return false;
				while (!isDelimeter(c));
			}
		}

		template<class OutputIterator> bool operator ()
			(
				OutputIterator begin, OutputIterator end
			)
		{
			for (; begin != end; ++begin)
				if (!this->operator() (*begin)) return false;
			return true;
		}

		unsigned int getRadix() const
		{
			return radix;
		}

		void setRadix(unsigned int nRadix)
		{
			if (isValidRadix(nRadix)) radix = nRadix;
		}

		std::set<char> getDelimeters() const
		{
			return delimeters;
		}

		void setDelimeters(const std::set<char> &nDelimeters)
		{
			delimeters = nDelimeters;
		}

		bool isDelimeter(char c) const
		{
			return delimeters.count(c);
		}

		void addDelimeter(char c)
		{
			delimeters.insert(c);
		}

		void removeDelimeter(char c)
		{
			delimeters.erase(c);
		}

	private:
		unsigned int radix;
		std::set<char> delimeters;

		bool isValidRadix(unsigned int r)
		{
			return r >= 2 && r <= 36;
		}

		/*
		 * returns integer value of digit and -1 if invalid character
		 */
		int digitValue(char c)
		{
			if (isdigit(c)) return c - '0';
			if (isalpha(c))
			{
				int value = tolower(c) - 'a' + 10;
				return (0 <= value && value < (int) radix) ? value : -1;
			}
			return -1;
		}
};

#endif // INPUTSTREAMREADER_H
