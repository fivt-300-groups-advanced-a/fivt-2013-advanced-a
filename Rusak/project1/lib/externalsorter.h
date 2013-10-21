#include <cstdlib>
#include <sstream>
#include <vector>
#include <algorithm>

template<typename T> class ExternalSorter {  
  template<typename Reader, typename Cmp> void redirect_elements(Reader &reader, int num, int count, Cmp &cmp) {
    std::stringstream str;
    str << num;
    std::string result;
    str >> result;
    result = "temp"+result+".txt";
    std::cout << result << "\n";
    BinaryWriter<T> temp_writer(result.c_str());
    std::vector<T> temp;
    T element;
    while(!reader.eof() && (count--)) {
      reader(element);
      std::cout << element << "\n";
      temp.push_back(element);
    }

      exit(0);
    std::sort(temp.begin(), temp.end(), cmp);
    for (size_t i = 0; i<temp.size(); i++) temp_writer(temp[i]);
  }
  public :
  template<typename Reader, typename Writer, typename Cmp> void operator()(int RAM, Reader &reader, Writer &writer, Cmp cmp) {
    int count_of_elem = RAM/sizeof(T);
    T element;
    int count_of_files = 0;
    while (!reader.eof()) {
      redirect_elements(reader, count_of_files++, count_of_elem, cmp);
    }
  }

};
