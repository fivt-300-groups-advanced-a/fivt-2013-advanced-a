#ifndef ABSTRACTWRITER_H
#define ABSTRACTWRITER_H

#include <iostream>

/**
 * Interface representing abstract fixed-type writer
 */
template <typename DataType> class AbstractWriter
{
	public:
		/**
		 * Default destructor unbinds stream if it's binded
		 */
		virtual ~AbstractWriter()
		{
			unbindStream();
		}

		/**
		 * Abstract method that writes one element to the stream
		 * Returns true in case of success, false otherwise
		 */
		virtual bool operator () (const DataType &element) = 0;

		/**
		 * Binds stream to the reader
		 */
		virtual void bindStream(std::ostream &in)
		{
			stream = &in;
		}

		/**
		 * Unbinds currently binded stream
		 */
		virtual void unbindStream()
		{
			if (this->stream) this->stream->flush();
			stream = 0;
		}

		/**
		 * Checks if writer is ready to write data
		 */
		virtual bool ready()
		{
			return stream && *stream;
		}

		/**
		 * Returns a pointer to the binded stream
		 * It can be used for adjusting precision or other options for example
		 */
		virtual std::ostream* getStream() const
		{
			return stream;
		}

	protected:
		std::ostream *stream;
};

#endif // ABSTRACTWRITER_H
