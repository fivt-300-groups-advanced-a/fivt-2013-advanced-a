#ifndef EXTERNAL_SORTER

#define EXTERNAL_SORTER

#include <string>
#include <string.h>
#include <vector>
#include <queue>
#include <cstdio>
#include <utility>
#include <iostream>
#include <memory>

/*
 * Comparator for pair<T, int> 
 */
template<class T, class Comparator> struct cmp {
    bool operator() 
        (const std::pair<T, int> &a, const std::pair<T, int> &b) const{
        Comparator comparator;
        if (comparator(a.first, b.first))
            return false;
        else if (comparator(b.first, a.first))
            return true;
        else
            return (a < b);
    }
};

const std::string default_filename = "result.txt";

template<class T, class Reader, class Writer, 
     class Sorter, class Comparator> 
class ExternalSorter {
    public:
        explicit ExternalSorter(const std::string &new_filename){
            setFile(new_filename);
            setResultFile(default_filename);
        }

        explicit ExternalSorter(char *new_filename){
            setFile(new_filename);
            setResultFile(default_filename);
        }

        void setFile(const std::string &new_filename){
            filename = new_filename;
        }

        void setFile(char *new_filename){
            int len = strlen(new_filename);
            filename = "";
            for (int i = 0; i < len; i++)
                filename += new_filename[i];
        }

        void setResultFile(const std::string &new_result_filename){
            result_filename = new_result_filename;
        }

        void setResultFile(char *new_result_filename){
            int len = strlen(new_result_filename);
            result_filename = "";
            for (int i = 0; i < len; i++)
                result_filename += new_result_filename[i];
        }
 
        void sort(int new_block_size = 100000000){
            block_size = new_block_size;
            temp_files.clear();
            Reader reader(filename);
            while (true){
                std::vector<T> cur_block = readBlock(reader);
                if (cur_block.size() == 0)
                    break;
                temp_files.push_back(getTempName());
                Sorter sort;
                sort(cur_block);
                writeBlock(cur_block, temp_files[temp_files.size() - 1]);
                if ((int)cur_block.size() < block_size)
                    break;
            }
            merge();
            removeTempFiles();
        }

    private:
        int block_size;
        std::string filename;
        std::string result_filename;
        std::vector<std::string> temp_files;

        std::vector<T> readBlock(Reader &reader){
            T next;
            std::vector<T> block(block_size);
            int cur_sz = 0;
            while (cur_sz < block_size && reader(next)){
                block[cur_sz] = next;
                cur_sz++;
            }
            block.resize(cur_sz);
            return block;
        }

        void writeBlock(const std::vector<T> &to_write, 
                const std::string &filename){
            Writer writer(filename);
            for (int i = 0; i < (int)to_write.size(); i++)
                writer(to_write[i]);
        }

        void merge(){
            typedef std::vector<std::pair<T, int> > HeapContainer;
            typedef std::pair<T, int> HeapElement;
            std::priority_queue<HeapElement, HeapContainer, 
                cmp<T, Comparator> > heap;
            int cnt_blocks = temp_files.size();
            std::vector<Reader> readers(cnt_blocks);
            for (int i = 0; i < cnt_blocks; i++){
                readers[i].setStream(temp_files[i]);
                T buf;
                if (readers[i](buf)){
                    heap.push(std::make_pair(buf, i));                
                }
            }
            Writer writer(result_filename);
            int q = 0;
            while (!heap.empty()){
                q++;
                std::pair<T, int> top = heap.top();
                heap.pop();
                writer(top.first);
                T buf;
                if (readers[top.second](buf))
                    heap.push(std::make_pair(buf, top.second));
            }
        }

        std::string getTempName(){
            char* temp_name = tmpnam(NULL);
            return std::string(temp_name);
        }

        void removeTempFiles(){
            for (int i = 0; i < (int)temp_files.size(); i++)
                remove(temp_files[i].c_str());
        }
};

#endif
