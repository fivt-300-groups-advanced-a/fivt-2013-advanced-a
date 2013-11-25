#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <vector>
#include <algorithm>
#include <queue>

const char *temp_file_name(int num) {
    std::string result = "temp"+std::to_string(num);
    return result.c_str();
}

template<typename T, typename Reader, typename Writer, typename Sorter, typename Cmp> 
class ExternalSorter {  
  public :
    explicit ExternalSorter(int RAM, Reader &read, Writer &write) : reader(&read), writer(&write), RAM(RAM), count_of_files(0) {}

    void run() {
      int count_of_elem = RAM/sizeof(T);
      bool good = true;
      while (good) {
        good = redirect_elements(count_of_files++, count_of_elem);
      }
      count_of_files--;
      merge();
      delete_temp_files();
    }

  protected:
    void delete_temp_files() {
      for (int i=0;i<count_of_files;i++) remove(temp_file_name(i));
    }

    bool redirect_elements(int num, int count) {
      std::vector<T> temp;
      T element;
      bool empty = 1;
      while((count--) && (*reader)(element)) {
        empty = 0;
        temp.push_back(element);
      }
      if (empty) return false;
      Sorter()(temp, Cmp());
      BinaryWriter<T> temp_writer(temp_file_name(num));
      for (size_t i = 0; i<temp.size(); i++) temp_writer(temp[i]);
      return true;
    }

    class HeapPair {
      public:
        bool operator<(const HeapPair &ot) const {
          return !Cmp()(*(element), *(ot.element));
        }
        HeapPair(T *elem, int reader):reader(reader), element(elem) {}
        void clear() {
          delete element;
        }
        T *element;
        int reader;
    };


    void merge() {
      std::priority_queue< HeapPair > heap;
      std::vector<BinaryReader<T>> readers(count_of_files);
      for (int i=0;i<count_of_files;i++) {
        readers[i].assign(temp_file_name(i));
      }
      
      if (count_of_files>200) {
        T elem;
        /*
        BinaryReader<T> z;
        z.assign(temp_file_name(277));
        z(elem);
        std::cout << "ELEM: " << elem << "\n";
        z.close();
        */
        std::cout << "good: " << readers[525](elem) << "\n";
        std::cout << "ELEM1: " << elem << "\n";
        exit(0);
      }

      for (int i=0;i<count_of_files;i++) {
        T elem;
        if (!readers[i](elem)) {
          std::cout << "bad file: " << i << "\n";
        }
      }
      if (count_of_files>200) exit(0);

      for (int i=0;i<count_of_files;i++) {
        readers[i].assign(temp_file_name(i));
        T *elem = new T();
        readers[i](*elem);
        HeapPair pair(elem, i);
        heap.push(pair);
      }
      while (!heap.empty()) {
        HeapPair top = heap.top();
        (*writer)(*top.element);
        heap.pop();
        top.clear();
        T *elem = new T(); 
        if (readers[top.reader](*elem)) {
          HeapPair z(elem, top.reader);
          heap.push(z);
        }
        else {
          delete elem;
        }
      }
      std::cout << "COUNT of files:" << count_of_files << "\n";
    }

    Reader *reader;
    Writer *writer;
    int RAM;
    int count_of_files;
};


//Simply return ExternalSorter
template<typename T, typename Reader, typename Writer, typename Sorter = StdSort<T>, typename Cmp = std::less<T>>
ExternalSorter<T, Reader, Writer, Sorter, Cmp> NewExternalSorter(int RAM, Reader &read, Writer &write, Sorter sort = Sorter(), Cmp cmp = Cmp()) {
  return ExternalSorter<T, Reader, Writer, Sorter, Cmp>(RAM, read, write);
}
