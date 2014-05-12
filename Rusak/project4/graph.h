#ifndef GRAPH
#define GRAPH

#include "lists_of_incidents.h"
#include "factories.h"

class Graph {
  typedef std::vector<std::unique_ptr<ListOfIncidents> > data_type;
  public: 
    Graph(data_type &_data) {
      data = std::move(_data);
    }

    Graph(std::vector<std::unique_ptr<ListFactory> > vec_factory) {
      data.resize(vec_factory.size());
      for (int i=0;i<data.size();i++) {
        data[i] = std::move(vec_factory[i]->get_list());
      }
    }

    unsigned int size() const {
      return data.size();
    }

    const ListOfIncidents& get_list(unsigned int vertex) const {
      return *data[vertex];
    }

  private:
    data_type data;
};

#endif


