#include <fstream>
#include <string>

using namespace std;

template <typename T>
class BinaryWriter
{
	private:
		ostream * binOutStream;
	public:
		BinaryWriter()
		{
		}
		explicit BinaryWriter(char * fileName)
		{
			binOutStream = new ofstream(fileName, ios::out);
		}
		bool operator ()(T & x)
		{
			return binOutStream->write(reinterpret_cast<char *> (&x), sizeof(T));
		}
		~BinaryWriter()
		{
			delete binOutStream;
		}
};