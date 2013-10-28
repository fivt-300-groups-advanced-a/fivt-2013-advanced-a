#ifndef BinWRITE

#define BinWRITE

#include <fstream>
#include <string>

template <class T> class BinWriter 
{
	public:
		~BinWriter() 
		{
			if (fout != NULL) {
				(*fout).close();
				if (!flag)
					delete fout;
				fout = NULL;
			}
		}
		explicit BinWriter(const char* Filename) 
		{
			flag = false;
			fout = new std :: ofstream(Filename, std::ofstream::binary);
		}
		explicit BinWriter(const std :: string &Filename) 
		{
			flag = false;
			fout = new std :: ofstream(Filename.c_str(), std::ofstream::binary);
		}
		explicit BinWriter(const std :: ofstream &Stream) 
		{
			flag = true;
			fout = &Stream;
		}
		bool operator () (const T& var) 
		{
			return ((*fout).write((char*) &var, sizeof(T)));
		}
		
	private:
		bool flag;
		std :: ofstream *fout;
};

#endif