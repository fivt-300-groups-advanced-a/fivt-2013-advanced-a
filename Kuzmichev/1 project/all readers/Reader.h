#include <fstream>

class Reader
{
	protected:
		istream * inputStream;	
		filebuf fileBuffer;
	public:
		Reader(const char * fileName)
		{
			fileBuffer.open(fileName, ios::in);
			inputStream = new istream(&fileBuffer);
		}
		template <class Type>
		bool operator () (Type & x)
		{
			(*inputStream) >> x;
			return true;
		}
};