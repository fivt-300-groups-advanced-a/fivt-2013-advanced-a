#include <fstream>
#include <iostream>

class TxtFileWriter{
private:
	std::ofstream *outputStream;
public:
	TxtFileWriter(){
		outputStream=NULL;
	}
	void assign(const char *fileName) {
		outputStream=new std::ofstream(fileName);
	}
	void assign(const std::string &fileName){
		outputStream=new std::ofstream(fileName.c_str());
	}
	TxtFileWriter(const char *fileName) {
		assign(fileName);
	}
	TxtFileWriter(const std::string &fileName){
		assign(fileName);
	}
	template <class T> bool operator()(const T &element) {
		return (*outputStream)<<element;
	}
	void close(){
		if (outputStream) {
				(*outputStream).close();
				delete outputStream;
				outputStream=NULL;
		}
	}
	~TxtFileWriter(){
		close();
	}
};

class BinaryFileWriter{
private:
	std::ofstream *outputStream;
public:
	BinaryFileWriter(){
		outputStream=NULL;
	}
	void assign(char *fileName) {
		outputStream=new std::ofstream(fileName,std::ofstream::binary);
	}
	void assign(const std::string &fileName) {
		outputStream=new std::ofstream(fileName,std::ofstream::binary);
	}
	BinaryFileWriter(char *fileName) {
		assign(fileName);
	}
	BinaryFileWriter(const std::string &fileName) {
		assign(fileName);
	}
	template <class T> bool operator()(const T &element) {
		return (*outputStream).write((char*)&element,sizeof(T));
	}
	void close(){
		if (outputStream) {
				(*outputStream).close();
				delete outputStream;
				outputStream=NULL;
		}
	}
	~BinaryFileWriter(){
		close();
	}
};