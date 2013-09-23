#include <fstream>

using namespace std;

class BinaryReader
{
	protected:
		istream * binStream;
		filebuf fileBuffer;
	public:
		BinaryReader(const char * fileName)
		{
			fileBuffer.open(fileName, ios::in | ios::binary);
			binStream = new istream(&fileBuffer);
		}
		/*BinaryReader(string & fileName)
		{
			const char * charArray = fileName.c_str();
			fileBuffer.open(charArray, ios::in | ios::binary);
			binStream = new istream(&fileBuffer);
		}*/
		bool operator () (char * buf, int amount)
		{
			binStream->read(buf, amount);
			return true;
		}
};