#ifndef WRITE

#define WRITE

#include <fstream>
#include <string>

template <class T> class Writer 
{
	public:
		~Writer() 
		{
			if (fout != NULL) {
				(*fout).close();
				if (!flag)
					delete fout;
				fout = NULL;
			}
		}
		explicit Writer(const char* Filename) 
		{
			flag = false;
			fout = new std :: ofstream(Filename);
		}
		explicit Writer(const std :: string &Filename) 
		{
			flag = false;
			fout = new std :: ofstream(Filename.c_str());
		}
		explicit Writer(const std :: ofstream &Stream) 
		{
			flag = true;
			fout = &Stream;
		}
		bool operator () (const T& var) 
		{
			return (*fout << var << std::endl);
		}
		
	private:
		bool flag;
		std :: ofstream *fout;
};

#endif