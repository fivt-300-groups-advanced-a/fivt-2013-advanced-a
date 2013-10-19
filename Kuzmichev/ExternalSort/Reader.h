#include <fstream>

class Reader
{
	protected:
		istream * inputStream;	
	public:
		explicit Reader(const char * fileName)
		{
			inputStream = new ifstream(fileName, ios::in);
		}
		template <class Type>
		bool operator () (Type & x)
		{
			return (*inputStream) >> x;
		}
		~Reader()
		{
			delete inputStream;
		}
};