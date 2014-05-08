#ifndef PRINT_H
#define PRINT_H

// We declare operator << for objects we want to print using googletest
// and it requires operator << for printed object
// to be declared in the same namespace where object was declared

namespace std {
inline std::ostream& operator << (std::ostream& os,
                                  const tuple<int,int>& val) {
    os << std::get<0>(val) << " " << std::get<1>(val);
    return os;
  }
inline std::ostream& operator << (std::ostream& os,
                                  const vector<vector<bool>>& val) {
  for (auto it = val.begin(); it != val.end(); ++it) {
    for (auto jt = it->begin(); jt != it->end(); ++jt)
      os << ((int)(*jt));
    os << endl;
  }
  return os;
}
}//namespace std
#endif //PRINT_H