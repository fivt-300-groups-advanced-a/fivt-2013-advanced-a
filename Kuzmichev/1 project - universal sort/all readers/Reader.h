#include <fstream>

class Reader
{
	protected:
		istream * inputStream;	
	public:
		Reader(const char * fileName)
		{
			inputStream = new ifstream(fileName, ios::in);
		}
		template <class Type>
		bool operator () (Type & x)
		{
			(*inputStream) >> x;
			return true;
		}
};