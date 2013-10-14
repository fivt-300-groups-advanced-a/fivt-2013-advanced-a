#include <fstream>
#include <string>

using namespace std;

class Writer
{
	private:
		//static const string defaultSeparator = " ";
		ostream * outStream;
	public:
		Writer(char * fileName)
		{
			outStream = new ofstream(fileName, ios::out);
			outStream->clear();
		}
		//bool operator (T & x, string separator = defaultSeparator)
		template <typename T>
		bool operator ()(T & x, string separator)
		{
			*(outStream) << x << separator;
			return true;
		}
};