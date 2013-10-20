#ifndef READ

#define READ

#include <fstream>
#include <string>

template <class T> class Reader 
{
	public:
		~Reader() 
		{
			if (fin != NULL) {
				(*fin).close();
				if (!flag)
					delete fin;
				fin = NULL;
			}
		}
		explicit Reader() 
		{	
			flag = true;
		}
		explicit Reader(const char* Filename) 
		{	
			flag = false;
			fin = new std :: ifstream(Filename);
		}
		explicit Reader(const std :: string &Filename) 
		{
			flag = false;
			fin = new std :: ifstream(Filename.c_str());
		}
		explicit Reader(const std :: ifstream &Stream) 
		{
			flag = true;
			fin = &Stream;
		}
		bool operator () (T& var) 
		{
			return (*fin >> var);
		}
		
	private:
		bool flag;
		std :: ifstream *fin;
};

#endif