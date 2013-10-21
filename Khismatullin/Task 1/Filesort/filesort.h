#ifndef FILESORT

#define FILESORT

#include "../IO/reader.h"
#include "../IO/breader.h"
#include "../IO/writer.h"
#include "../IO/bwriter.h"
#include "../RAMsort/ramsort.h"
#include "../Heap/heap.h"

#include <string>
#include <fstream>
#include <cassert>
#include <vector>
#include <stdio.h>


std::string inttostr(int x) 
{
	if (x == 0) 
	{
		return "0";
	}
	std::string res = "";
	while (x > 0) 
	{
		res.push_back('0' + x % 10);
		x /= 10;
	}
	reverse(res.begin(), res.end());
	return res;
}

template<class T, class comp>
class mycmp
{
	public:
		bool operator () (std::pair<T, std::size_t> a, std::pair<T, std::size_t> b) {
			comp cmp;
			return cmp(a.first, b.first);
		}
};

template<class T, class comp>
void _filesort(Reader<T> in, Writer<T> out, std::size_t mem) //сколько весит поток?
{
	Writer<std::string> err("log.txt");
	
	std::size_t mem0 = mem;
	mem /= 2;
	mem-= 300;
	assert((mem >= sizeof(T)));
	size_t count = mem / sizeof(T);
	bool flag = false;
	std::vector<T> v;
	T x;
	std::size_t cnt = 0, all = 0;
	while (!flag) 
	{
		while (v.size() < mem / sizeof(T)) 
		{
			if (!in(x)) {
				flag = true;
				break;
			}
			v.push_back(x);
		}
		if (v.size() == 0) {
			break;
		}
		head_sort<T, comp>(v);
		all += v.size();
		std::string file = "##__";
		file.append(inttostr(cnt));
		file.append("__##.bft");
		BinWriter<T> bout(file);
		for(std::size_t i = 0; i < v.size(); i++) {
			bout(v[i]);
		} 
		cnt++;
		v.clear();
	}
	BinReader<T> bin[cnt];
	
	Heap<std::pair<T, std::size_t>, mycmp<T, comp> > heap(cnt);
	for(std::size_t i = 0; i < cnt; i++) {
		std::string file = "##__";
		file.append(inttostr(i));
		file.append("__##.bft");
		BinReader<T> tmp(file);
		T x;
		tmp(x);
		bin[i] = tmp;
		heap.add(std::make_pair(x, i));
	}
	
	for(std::size_t i = 0; i < all; i++) {
		std::pair<T, size_t> x = heap.Extract_min();
		out(x.first);
		err(inttostr(x.second));
		if (bin[x.second](x.first)) {
			heap.add(x);
		}
	}
	
	for(std::size_t i = 0; i < cnt; i++) 
	{
		std::string file = "##__";
		file.append(inttostr(cnt));
		file.append("__##.bft");
		remove(file.c_str()); //грустно
	}
	delete[] bin;
}

template<class T, class comp>
void filesort(std::string in, std::string out, std::size_t mem) 
{
	assert(in != out);
	Reader<T> fin(in);
	Writer<T> fout(out);
	_filesort<T, comp>(fin, fout, mem);	
}

template<class T, class comp>
void filesort(char* in, char* out, std::size_t mem) 
{
	assert(in != out);
	Reader<T> fin(in);
	Writer<T> fout(out);
	_filesort<T, comp>(fin, fout, mem);	
}

template<class T, class comp>
void filesort(std::ifstream& in, std::ofstream& out, std::size_t mem) 
{
	Reader<T> fin(in);
	Writer<T> fout(out);
	_filesort<T, comp>(fin, fout, mem);	
}

#endif