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

template <typename T, typename Cmp> class HeapPair {
  public:
    bool operator<(const HeapPair &ot) const {
      return !cmp(*(element), *(ot.element));
    }
    HeapPair(T *elem, int reader, Cmp cmp):reader(reader), element(elem), cmp(cmp) {}
    void clear() {
      delete element;
    }
    Cmp cmp;
    T *element;
    int reader;
};

template<typename T> class ExternalSorter {  
  public :
  template<typename Reader, typename Writer, typename Sorter = StdSort<T>, typename Cmp = std::less<T>> void operator()(int RAM, Reader &reader, Writer &writer,  Cmp cmp = Cmp(), Sorter sorter = Sorter()) {
    int count_of_elem = RAM/sizeof(T);
    int count_of_files = 0;
    bool good = true;
    while (good) {
      good = redirect_elements(reader, count_of_files++, count_of_elem, sorter, cmp);
    }
    count_of_files--;
    merge(writer, count_of_files, cmp);
    delete_temp_files(count_of_files);
  }
  protected:
    void delete_temp_files(int count) {
      for (int i=0;i<count;i++) remove(temp_file_name(i));
    }

  template<typename Reader, typename Cmp, typename Sorter> bool redirect_elements(Reader &reader, int num, int count, Sorter sorter, Cmp cmp) {
    std::vector<T> temp;
    T element;
    bool empty = 1;
    while((count--) && reader(element)) {
      empty = 0;
      temp.push_back(element);
    }
    if (empty) return false;
    sorter(temp, cmp);
    BinaryWriter<T> temp_writer(temp_file_name(num));
    for (size_t i = 0; i<temp.size(); i++) temp_writer(temp[i]);
    return true;
  }

  template<typename Writer, typename Cmp> void merge(Writer &writer, int count_of_files, Cmp cmp) {
    std::priority_queue< HeapPair<T, Cmp > > heap;
    std::vector<BinaryReader<T>> readers(count_of_files);
    for (int i=0;i<count_of_files;i++) {
      readers[i].assign(temp_file_name(i));
      T *elem = new T();
      readers[i](*elem);
      HeapPair<T, Cmp> pair(elem, i, cmp);
      heap.push(pair);
    }
    while (!heap.empty()) {
      HeapPair<T, Cmp> top = heap.top();
      writer(*top.element);
      heap.pop();
      top.clear();
      T *elem = new T(); 
      if (readers[top.reader](*elem)) {
        HeapPair<T, Cmp> z(elem, top.reader, cmp);
        heap.push(z);
      }
    }
  }
};
