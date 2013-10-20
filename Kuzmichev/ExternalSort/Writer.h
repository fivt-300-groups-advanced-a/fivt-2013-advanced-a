#include <fstream>
#include <string>

using namespace std;

class Writer
{
	private:
		ostream * outStream;
		string separator;
		//string separator;
	public:
		//explicit Writer(char * fileName, string & sep)
		Writer()
		{
		}
		explicit Writer(char * fileName)
		{
			outStream = new ofstream(fileName, ios::out);
			separator = " ";
			//separator = sep;
		}
		template <typename T>
		bool operator ()(T & x)
		{
			return *(outStream) << x << separator;
		}
		void close()
		{
			outStream->flush();
		}
		~Writer()
		{
			delete outStream;
		}
};