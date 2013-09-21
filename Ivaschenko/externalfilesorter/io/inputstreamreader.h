#ifndef INPUTSTREAMREADER_H
#define INPUTSTREAMREADER_H

#include <set>

#include "abstractreader.h"

template<typename DataType> class InputStreamReader : public AbstractReader<DataType>
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

template<> class InputStreamReader<int> : public AbstractReader<int>
{
	public:
		explicit InputStreamReader(std::istream &in = std::cin)
		{
			this->stream = &in;
			radix = 10;
			addDelimeter(' ');
		}

		bool operator() (int &element)
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
						element = element * radix + digitValue(c);
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

		int getRadix() const
		{
			return radix;
		}

		void setRadix(int nRadix)
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
		int radix;
		std::set<char> delimeters;

		bool isValidRadix(int r)
		{
			return r >= 2 && r <= 36;
		}

		int digitValue(char c)
		{
			if (isdigit(c)) return c - '0';
			if (isalpha(c))
			{
				int value = tolower(c) - 'a' + 10;
				return (0 <= value && value < radix) ? value : -1;
			}
			return -1;
		}
};

#endif // INPUTSTREAMREADER_H
