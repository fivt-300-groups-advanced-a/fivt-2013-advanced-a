#ifndef OUTPUTSTREAMWRITER_H
#define OUTPUTSTREAMWRITER_H

#include "abstractwriter.h"

#include <vector>

/**
 * Class implementing AbstractWriter interface
 * Used for writing fixed-type data (and sequences of data) to output stream
 *
 * TODO: specialisation for integer and float types
 */
template<typename DataType> class OutputStreamWriter : public AbstractWriter<DataType>
{
	public:
		/**
		 * Initialising from any output stream (stdout by default)
		 */
		explicit OutputStreamWriter(std::ostream &out = std::cout)
		{
			delimeter = ", ";
			this->stream = &out;
			startLine = true;
		}

		/**
		 * Unbinds stream, adding new line if necessary
		 */
		void unbindStream()
		{
			if (this->stream) this->stream->flush();
			if (!startLine) newLine();
			this->stream = 0;
		}

		/**
		 * Sets new delimeter
		 */
		void setDelimeter(const std::string &nDelimeter)
		{
			delimeter = nDelimeter;
		}

		/**
		 * Returns current delimeter
		 */
		std::string getDelimeter() const
		{
			return delimeter;
		}

		/**
		 * Adds a new line
		 */
		bool newLine()
		{
			if (!this->ready() || !(*this->stream << std::endl)) return false;
			startLine = true;
			return true;
		}

		/**
		 * Writes an element
		 * Returns true in case of success, false otherwise
		 */
		bool operator () (const DataType &element)
		{
			if (!this->ready()) return false;
			if (!startLine && !(*this->stream << delimeter)) return false;
			if (!(*this->stream << element)) return false;
			startLine = false;
			return true;
		}

		/**
		 * Writes a sequence of data using specified prefix, suffix and current delimeter
		 */
		template<class ForwardIterator> bool operator ()
			(
				ForwardIterator begin, ForwardIterator end,
				std::string prefix = "{", std::string suffix = "}"
			)
		{
			if (!this->ready() || !(*this->stream << prefix)) return false;
			for (; begin != end;)
			{
				if (!(*this->stream << *begin)) return false;
				if (++begin != end && !(*this->stream << delimeter)) return false;
			}
			if (!(*this->stream << suffix)) return false;
			return newLine();
		}

	private:
		std::string delimeter;
		bool startLine;
};

#endif // OUTPUTSTREAMWRITER_H
