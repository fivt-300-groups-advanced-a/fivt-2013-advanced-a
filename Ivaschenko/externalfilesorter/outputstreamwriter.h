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

		bool operator () (const std::vector<DataType> &v)
		{
			if (!this->ready() || !(*this->stream << "(")) return false;
			for (typename std::vector<DataType>::size_type i = 0; i + 1 < v.size(); i++)
				if (!(*this->stream << v[i] << delimeter)) return false;
			if (!(*this->stream << v.back() << ")")) return false;
			return newLine();
		}

	private:
		std::string delimeter;
		bool startLine;
};

#endif // OUTPUTSTREAMWRITER_H
