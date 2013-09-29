#include <iostream>
#include <fstream>
#include <string>
class TxtFileReader{
private:
	std::ifstream *inputStream;
public:
	TxtFileReader(){
		inputStream=NULL;
	}
	void assign(const char *fileName){
		inputStream=new std::ifstream(fileName);
	}
	void assign(const std::string &fileName){
		inputStream=new std::ifstream(fileName.c_str());
	}
	TxtFileReader(const char *fileName) {
		assign(fileName);
	}
	TxtFileReader(const std::string &fileName) {
		assign(fileName);
	}
	template <class T> bool operator ()(T &element)
	{
		return (*inputStream)>>element;
	}
	void close(){
		if (inputStream) {
				(*inputStream).close();
				delete inputStream;
				inputStream=NULL;
		}
	}
	~TxtFileReader(){
		close();
	}
};

class BinaryFileReader{
private:
	std::ifstream *inputStream;
public:
	BinaryFileReader(){
		inputStream=NULL;
	}
	void assign(const char *fileName) {
		inputStream=new std::ifstream(fileName,std::fstream::binary);
	}
	void assign(const std::string &fileName) {
		inputStream=new std::ifstream(fileName.c_str(),std::ofstream::binary);
	}
	BinaryFileReader(const char *fileName) {
		assign(fileName);
	}
	BinaryFileReader(const std::string &fileName) {
		assign(fileName);
	}
	template <class T> bool operator()(T &element)
	{
		return (*inputStream).read((char*)&element,sizeof(T));
	}
	void close(){
		if (inputStream) {
				(*inputStream).close();
				delete inputStream;
				inputStream=NULL;
		}
	}
	~BinaryFileReader(){
		close();
	}
};