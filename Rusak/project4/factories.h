#ifndef FACTORIES
#define FACTORIES

#include "lists_of_incidents.h"
#include <vector>

class ListFactory {
  public: 
    virtual std::unique_ptr<ListOfIncidents> get_list() = 0;
    virtual void add_vertex(unsigned int) = 0;
    virtual std::string get_type() = 0;
    virtual ~ListFactory() {}
};

class VectorFactory : public ListFactory {
  public: 
    VectorFactory(unsigned int num):number_of_vertexes(num) {}

    virtual std::unique_ptr<ListOfIncidents> get_list() override {
      auto res = std::unique_ptr<ListOfIncidents>(new VectorList(list));
      list.clear();
      return std::move(res);
    }

    virtual void add_vertex(unsigned int num) override {
      list.push_back(num);
    }

    virtual std::string get_type() override {
      return "VectorFactory";
    }

    virtual ~VectorFactory() {}

  private: 
    unsigned int number_of_vertexes;
    std::vector<unsigned int> list;
};

class BitsetFactory : public ListFactory {
  public: 
    BitsetFactory(unsigned int num):number_of_vertexes(num) {}

    virtual std::unique_ptr<ListOfIncidents> get_list() override {
      auto res = std::unique_ptr<ListOfIncidents>(new BitsetList(list));
      list.clear();
      return std::move(res);
    }

    virtual void add_vertex(unsigned int num) override {
      if (num>=list.size()) list.resize(num+1);
      list[num] = 1;
    }

    virtual std::string get_type() override {
      return "BitsetFactory";
    }

    virtual ~BitsetFactory() {}

  private: 
    unsigned int number_of_vertexes;
    std::vector<bool> list;
};

class CleverFactory : public ListFactory {
  enum list_type { bitset, vector };

  public: 
    CleverFactory(unsigned int num):number_of_vertexes(num), type(vector) {}

    virtual std::unique_ptr<ListOfIncidents> get_list() override {
      if (type == vector) {
        auto res = std::unique_ptr<ListOfIncidents>(new VectorList(list_vector));
        list_vector.clear();
        return std::move(res);
      }
      else {
        auto res = std::unique_ptr<ListOfIncidents>(new BitsetList(list_bitset));
        list_bitset.clear();
        return std::move(res);
      }
    }

    virtual void add_vertex(unsigned int num) override {
      if (type == vector) {
        list_vector.push_back(num);
        if (list_vector.size()*32 >= number_of_vertexes) {
          while (!list_vector.empty()) {
            unsigned int cur = list_vector.back();
            if (cur>=list_bitset.size()) list_bitset.resize(cur+1);
            list_bitset[cur] = 1;
            list_vector.pop_back();
          }
          type = bitset;
        }
      }
      else {
        if (num>=list_bitset.size()) list_bitset.resize(num+1);
        list_bitset[num] = 1;
      }
    }

    virtual std::string get_type() override {
      std::string res = "Clever --> ";
      if (type == bitset) res+="Bitset";
      else res+="Vector";
      return res;
    }

    virtual ~CleverFactory() {}

  private: 
    unsigned int number_of_vertexes;
    std::vector<unsigned int> list_vector;
    std::vector<bool> list_bitset;
    list_type type;
};

#endif


