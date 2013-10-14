#ifndef OUTPUTSTREAMWRITER_H
#define OUTPUTSTREAMWRITER_H

#include <vector>

/**
 * Class implementing AbstractWriter interface
 * Used for writing fixed-type data (and sequences of data) to output stream
 */
template<typename DataType> class OutputStreamWriter
{
	public:
		/**
		 * Default destructor unbinds stream if it's binded
		 */
		~OutputStreamWriter()
		{
			unbindStream();
		}

		/**
		 * Binds stream to the reader
		 */
		void bindStream(std::ostream &in)
		{
			stream = &in;
		}

		/**
		 * Checks if writer is ready to write data
		 */
		bool ready()
		{
			return stream && *stream;
		}

		/**
		 * Returns a pointer to the binded stream
		 * It can be used for adjusting precision or other options for example
		 */
		std::ostream* getStream() const
		{
			return stream;
		}

		/**
		 * Initialising from any output stream (stdout by default)
		 */
		explicit OutputStreamWriter(std::ostream &out = std::cout)
		{
			delimeter = ", ";
			stream = &out;
			startLine = true;
		}

		/**
		 * Unbinds stream, adding new line if necessary
		 */
		void unbindStream()
		{
			if (stream) stream->flush();
			if (!startLine) newLine();
			stream = 0;
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
			if (!ready() || !(*stream << std::endl)) return false;
			startLine = true;
			return true;
		}

		/**
		 * Writes an element
		 * Returns true in case of success, false otherwise
		 */
		bool operator () (const DataType &element)
		{
			if (!ready()) return false;
			if (!startLine && !(*stream << delimeter)) return false;
			if (!(*stream << element)) return false;
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
			if (!ready() || !(*stream << prefix)) return false;
			for (; begin != end;)
			{
				if (!(*stream << *begin)) return false;
				if (++begin != end && !(*stream << delimeter)) return false;
			}
			if (!(*stream << suffix)) return false;
			return newLine();
		}

	private:
		std::ostream *stream;
		std::string delimeter;
		bool startLine;
};

#endif // OUTPUTSTREAMWRITER_H
