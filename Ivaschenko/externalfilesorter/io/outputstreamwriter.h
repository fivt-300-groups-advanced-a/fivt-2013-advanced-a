#ifndef OUTPUTSTREAMWRITER_H
#define OUTPUTSTREAMWRITER_H

#include "abstractwriter.h"

#include <vector>

template<typename DataType> class OutputStreamWriter : public AbstractWriter<DataType>
{
	public:
		explicit OutputStreamWriter(std::ostream &out = std::cout)
		{
			delimeter = ", ";
			this->stream = &out;
			startLine = true;
		}

		void unbindStream()
		{
			if (!startLine) newLine();
			this->stream = 0;
		}

		void setDelimeter(const std::string &nDelimeter)
		{
			delimeter = nDelimeter;
		}

		std::string getDelimeter() const
		{
			return delimeter;
		}

		bool newLine()
		{
			if (!this->ready() || !(*this->stream << std::endl)) return false;
			startLine = true;
			return true;
		}

		bool operator () (const DataType &element)
		{
			if (!this->ready()) return false;
			if (!startLine && !(*this->stream << delimeter)) return false;
			if (!(*this->stream << element)) return false;
			startLine = false;
			return true;
		}

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
