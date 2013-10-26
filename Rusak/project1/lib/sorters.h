#include <algorithm>
#include <vector>
 
template<typename T>
struct StdSort {
  template <typename Cmp> void operator()(std::vector<T> &data, Cmp cmp) {
    std::sort(data.begin(), data.end(), cmp);
  }
};

template<typename T>
struct StdStableSort {
  template<typename Cmp> void operator()(std::vector<T> &data, Cmp cmp) {
    std::stable_sort(data.begin(), data.end(), cmp);
  }
};
