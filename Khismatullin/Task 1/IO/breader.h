#ifndef BinREAD

#define BinREAD
#include <iostream>
#include <fstream>
#include <string>

template <class T> class BinReader
{
	public:
		~BinReader()
		{
			if (fin != NULL) {
				(*fin).close();
				if (!flag)
					delete fin;
				fin = NULL;
			}
		}
		BinReader()
		{
			flag = true;
		}
		explicit BinReader(const char* Filename)
		{
			flag = false;
			fin = new std :: ifstream(Filename, std::ifstream::binary);
		}
		explicit BinReader(const std :: string &Filename)
		{
			flag = false;
			//std::cout << "GGDFGFGFGFGFGF\n";
			fin = new std :: ifstream(Filename.c_str(), std::ifstream::binary);
		}
		explicit BinReader(std :: ifstream &Stream)
		{
			flag = true;
			fin = &Stream;
		}
		bool operator () (T& var)
		{
			return (fin->read((char*) &var, sizeof(T)));
		}

	private:
		bool flag;
		std :: ifstream *fin;
};

#endif