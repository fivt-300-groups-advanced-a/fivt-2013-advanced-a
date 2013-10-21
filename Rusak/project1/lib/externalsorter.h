#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <vector>
#include <algorithm>
#include <queue>

const char *temp_file_name(int num) {
    std::stringstream str;
    str << num;
    std::string result;
    str >> result;
    result = "temp"+result+".txt";
    return result.c_str();
}

template <typename T, typename Reader, typename Cmp> class HeapPair {
  public:
    bool operator<(const HeapPair &ot) const {
      return !cmp(*(element), *(ot.element));
    }
    HeapPair(T *elem, Reader *re, Cmp cmp):reader(re), element(elem), cmp(cmp) {}
    //~HeapPair() {
    //  delete element;
    //}
    Cmp cmp;
    T *element;
    Reader *reader;
};

template<typename T> class ExternalSorter {  
  public :
  template<typename Reader, typename Writer, typename Cmp> void operator()(int RAM, Reader &reader, Writer &writer, Cmp cmp) {
    int count_of_elem = RAM/sizeof(T);
    T element;
    int count_of_files = 0;
    bool good = true;
    while (good) {
      good = redirect_elements(reader, count_of_files++, count_of_elem, cmp);
    }
    count_of_files--;
    merge(writer, count_of_files, cmp);
    delete_temp_files(count_of_files);
  }
  protected:
    void delete_temp_files(int count) {
      for (int i=0;i<count;i++) remove(temp_file_name(i));
    }

  template<typename Reader, typename Cmp> bool redirect_elements(Reader &reader, int num, int count, Cmp cmp) {
    std::vector<T> temp;
    T element;
    bool empty = 1;
    while((count--) && reader(element)) {
      empty = 0;
      temp.push_back(element);
    }
    if (empty) return false;
    std::sort(temp.begin(), temp.end(), cmp);
    BinaryWriter<T> temp_writer(temp_file_name(num));
    for (size_t i = 0; i<temp.size(); i++) temp_writer(temp[i]);
    return true;
  }

  template<typename Writer, typename Cmp> void merge(Writer &writer, int count_of_files, Cmp cmp) {
    std::priority_queue< HeapPair<T, BinaryReader<T>, Cmp > > heap;
    for (int i=0;i<count_of_files;i++) {
      BinaryReader<T> *nr = new BinaryReader<T>(temp_file_name(i));
      T *elem = new T();
      (*nr)(*elem);
      HeapPair<T, BinaryReader<T>, Cmp> z(elem, nr, cmp);
      heap.push(z);
    }
    while (!heap.empty()) {
      HeapPair<T, BinaryReader<T>, Cmp> top = heap.top();
      heap.pop();
      writer(*top.element);
      //std::cout << (*top.element) << "\n";
      T *elem = new T(); 
      if ((*top.reader)(*elem)) {
        HeapPair<T, BinaryReader<T>, Cmp> z(elem, top.reader, cmp);
        heap.push(z);
      }
      else {
        delete top.reader;
      }
    }
  }
};
