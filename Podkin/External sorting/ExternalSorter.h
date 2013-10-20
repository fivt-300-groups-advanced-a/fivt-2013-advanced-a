#ifndef EXTERNALSORTER_H
#define EXTERNALSORETR_H

#include <string>
#include <sstream>
#include <queue>
#include "FileReader.h"
#include "BinaryReader.h"
#include "FileWriter.h"
#include "BinaryWriter.h"
#include <vector>
#include <iostream>

template<typename Type, class Comparator>
class PairComparator{
    public:
        bool operator()(std::pair<Type, int> x, std::pair<Type, int> y){
            Comparator comp;
            return comp(y.first, x.first);
        } 
};

template<typename Type>
class ExternalSorter{
    public:
        template <class Reader, class Writer, class Sorter, class Comparator>
        void operator()(int Memory, Reader &in, Writer &out, Sorter sort, Comparator comp){
            AvailableMemory = Memory;
            ProcessSort(in, out, sort, comp);
        }
    private:
        template <class Reader, class Writer, class Sorter, class Comparator>
        void ProcessSort(Reader &in, Writer &out, Sorter sort, Comparator comp){
            bool NextBlock = true;
            int NumberOfFiles = -1;
            while(NextBlock){
                std::vector<Type> Block = ReadBlock(in);
                if ((int)(Block.size()) != AvailableMemory)
                    NextBlock = false;
                SortBlock(Block, sort, comp);
                NumberOfFiles++;
                WriteBlock(Block, NumberOfFiles);
            }
            std::priority_queue< std::pair<Type, int>, std::vector< std::pair<Type, int> >, PairComparator <Type, Comparator> > Queue;
            std::vector <FileReader<Type> > readers(NumberOfFiles + 1);
            for (int i = 0; i <= NumberOfFiles; i++){
                readers[i].SetStream(TemporaryFiles[i]);
                Type Element;
                if (readers[i](Element))
                    Queue.push(std::make_pair(Element, i));
            }
            while(!Queue.empty()){
                std::pair<Type, int> Pair = Queue.top();
                out(Pair.first);
                int FileIndex = Pair.second;
                Queue.pop();
                Type Element;
                if (readers[FileIndex](Element)){
                    Queue.push(std::make_pair(Element, FileIndex));
                }
            }
            out.Close();
            for (int i = 0; i <= NumberOfFiles; i++){
               readers[i].Close();
               remove(TemporaryFiles[i].c_str());
            }
        }
        template <class Sorter, class Comparator>
        void SortBlock(std::vector<Type> &Block, Sorter sort, Comparator comp){
            sort(Block.begin(), Block.end(), comp);
        }
        template <class Reader>
        std::vector<Type> ReadBlock(Reader &in){
            int CurrentSize, Element;
            std::vector <Type> CurrentVector(AvailableMemory);
            for (CurrentSize = 0; CurrentSize < AvailableMemory; CurrentSize++){
                if (in(Element) == false)
                    break;
                CurrentVector[CurrentSize] = Element;
            }
            CurrentVector.resize(CurrentSize);
            return CurrentVector;
        }
        void WriteBlock(std::vector<Type> &Block, int FileIndex){
            TemporaryFiles.push_back(SetFileName(FileIndex));
            FileWriter<Type> out(TemporaryFiles[FileIndex]);
            for (int Index = 0; Index < (int)Block.size(); Index++)
                out(Block[Index]);
        }
        std::string SetFileName(int FileIndex){
            std::stringstream StringStream;
            StringStream << FileIndex;
            return (std::string)("temporary_file_") + StringStream.str() + (std::string)(".txt");
        }
        std::vector<std::string> TemporaryFiles;
        int AvailableMemory;
};

#endif
