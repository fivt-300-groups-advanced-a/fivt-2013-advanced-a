#include <fstream>

using namespace std;

template <typename T>
class BinaryReader
{
	protected:
		istream * binStream;
	public:
		BinaryReader()
		{
		}
		explicit BinaryReader(const char * fileName)
		{
			binStream = new ifstream(fileName, ios::in | ios::binary);
		}
		bool operator () (T & new_token)
		{
			return binStream->read(reinterpret_cast<char*>(&new_token), sizeof(T));
		}
		~BinaryReader()
		{
			delete binStream;
		}
};