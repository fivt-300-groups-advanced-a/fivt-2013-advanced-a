#include <fstream>
#include <string>

using namespace std;

template <typename T>
class Writer
{
	private:
		//static const string defaultSeparator = " ";
		ostream * binOutStream;
	public:
		Writer(char * fileName)
		{
			binOutStream = new ofstream(fileName, ios::out);
		}
		//bool operator (T & x, string separator = defaultSeparator)
		bool operator ()(T & x, string separator)
		{
			binOutStream->write(reinterpret_cast<char *> (&x), sizeof(T));
			binOutStream->write(reinterpret_cast<char *> (&separator), sizeof(separator));
		}
};