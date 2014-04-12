#ifndef FACTORIES
#define FACTORIES

class ListFactory {
  std::unique_ptr<ListOfIncidents> get_list(unsigned int vertex) = 0;
  virtual ~ListFactory() {}
};

/* 
class PerfectFactory : ListFactory {
  public: 
    PerfectFactory(const std::vector<std::vector<bool> > &_matrix) {
      matrix = &_matrix;
      lists = 0;
      is_matrix = true;
    }
    PerfectFactory(const std::vector<std::vector<unsigned int> > &lists) {
      matrix = 0;
      lists = &_lists;
      is_matrix = false;
    }

    std::unique_ptr<ListOfIncidents> get_list(unsigned int vertex) {
      if (is_matrix) {
        return std::unique_ptr(new VectorList((*matrix)[vertex]));
      }
      else {
        return std::unique_ptr(new VectorList((*lists)[vertex]));
      }
    }

    virtual ~VectorFactory() {}

  private: 
    bool is_matrix;
    const std::vector<std::vector<bool> > *matrix;
    const std::vector<std::vector<unsigned int> > *lists;
};
*/

template< typename To, typename From>
class BasicFactory : ListFactory {
  public: 
    BasicFactory(const From &_data):data(_data) {}

    std::unique_ptr<ListOfIncidents> get_list(unsigned int vertex) {
      return std::unique_ptr(new To(data[vertex]));
    }

    virtual ~BitsetFactory() {}

  private: 
    const From &data;
};

template<typename To, typename From>
BasicFactory<To, From>& make_basic_factory(const From &data) {
  return BasicFactory<To, From>(data);
}


#endif


