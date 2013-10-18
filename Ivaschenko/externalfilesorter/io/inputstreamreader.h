#ifndef INPUTSTREAMREADER_H
#define INPUTSTREAMREADER_H

#include <set>
#include <istream>
#include <type_traits>

namespace impl
{
	/**
	 * common part of all InputStreamReader
	 */
	class InputStreamReaderHelper
	{
		public:
			/**
			 * Unbind stream and destruct reader
			 */
			~InputStreamReaderHelper()
			{
				unbindStream();
			}

			/**
			 * Binds stream to the reader, unbinds an old one if needed
			 */
			void bindStream(std::istream &in)
			{
				stream = &in;
			}

			/**
			 * Unbinds currently binded stream
			 */
			void unbindStream()
			{
				stream = 0;
			}

			/**
			 * Returns true is stream is binded, not corrupted and ready to read
			 */
			bool ready() const
			{
				return stream && *stream;
			}

			/**
			 * Returns a pointer to the binded stream
			 * It can be used for adjusting precision or other options for example
			 */
			std::istream* getStream() const
			{
				return stream;
			}

		protected:
			std::istream *stream;
	};
}

/**
 * Class implementing AbstractReader interface
 * Used for reading fixed-type data from input stream
*/
template<typename DataType, typename = void> class InputStreamReader : public impl::InputStreamReaderHelper
{
	public:
		/**
		 * Initialising from any input stream (stdin by default)
		 */
		explicit InputStreamReader(std::istream &in = std::cin)
		{
			bindStream(in);
		}


		/**
		 * Reads an element
		 * Returns true in case of success, false otherwise
		 */
		bool operator() (DataType &element)
		{
			return ready() && (*stream >> element);
		}
};

/**
 * Specification of InputStreamReader for all integer types
 * Features:
 *		- Reading sequences separated by adjustable delimeters
 *		- Reading non-decimal integers
 */
template<typename IntegerType> class InputStreamReader
		<IntegerType, typename std::enable_if< std::is_integral<IntegerType>::value >::type>
		: public impl::InputStreamReaderHelper
{
	public:
		/**
		 * Initialising from any input stream, default radix is 10.
		 * Defualt delimeters are space and eoln
		 */
		explicit InputStreamReader(std::istream &in = std::cin)
		{
			bindStream(in);
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
			if (!ready()) return false;
			char c = 0;
			while (true)
			{
				while (c == 0 || isDelimeter(c))
					if (!stream->read(&c, 1)) return false;

				bool minus = c == '-' && std::is_signed<IntegerType>::value;
				if (minus && !stream->read(&c, 1)) return false;
				if (digitValue(c) >= 0)
				{
					number = 0;
					while (digitValue(c) >= 0)
					{
						number = number * (IntegerType) radix + (IntegerType) digitValue(c);
						if (!stream->read(&c, 1)) return true; // Last element before EOF
					}
					if (isDelimeter(c))
					{
						if (minus) number = -number;
						return true;
					}
					else continue;
				}

				while (!isDelimeter(c))
					if (!stream->read(&c, 1)) return false;
			}
		}

		/**
		 * Reads sequence of numbers by two output iterators
		 * Returns true in case of success, false otherwise
		 */
		template<class OutputIterator> bool operator () (OutputIterator begin, OutputIterator end)
		{
			for (; begin != end; ++begin)
				if (!operator() (*begin)) return false;
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

/**
 * Specification of InputStreamReader for std::strings
 */
template<> class InputStreamReader<std::string, void> : public impl::InputStreamReaderHelper
{
	public:
		/**
		 * Initialise reader from any input stream
		 * Default delimeters are space and end of line
		 */
		InputStreamReader(std::istream &in = std::cin)
		{
			bindStream(in);
			resetDelimeters();
			addDelimeter(' ');
			addDelimeter('\n');
		}

		/**
		 * Reads next token surrounded by delimeters
		 */
		bool operator() (std::string &element)
		{
			if (!ready()) return false;
			element.clear();
			char c;

			do if (!stream->read(&c, 1)) return false;
			while (isDelimeter(c));

			do
			{
				element += c;
				if (!stream->read(&c, 1)) return true;
			}
			while (!isDelimeter(c));
			return true;
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
		bool delimeterMask[alphabet];
};


#endif // INPUTSTREAMREADER_H
