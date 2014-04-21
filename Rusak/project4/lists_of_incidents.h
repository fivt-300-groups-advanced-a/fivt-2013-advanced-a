#ifndef LIST_OF_INC
#define LIST_OF_INC

#include <memory>
#include <algorithm>

class ListIterator {
  public: 
    ListIterator() {};
    virtual unsigned int current() const = 0;
    virtual bool next() = 0;
    virtual bool valid() const = 0;
    virtual ~ListIterator() {}
};

class ListOfIncidents {
  public: 
    ListOfIncidents() {}
    virtual std::unique_ptr<ListIterator> get_iterator() const = 0;
    virtual bool is_connected(unsigned int vertex) const = 0;
    virtual void optimize() = 0;
    virtual ~ListOfIncidents() {}
};

template <typename Iterator>
class BitsetIterator : public ListIterator {
  public:
    BitsetIterator(Iterator _begin, Iterator _end) {
      begin = _begin;
      end = _end;
      position = 0;
      if (begin!=end && (*begin)==false) next();
   }
    
    virtual unsigned int current() const override {
      if (!valid()) return -1;
      return position;
    }

    virtual bool next() override {
      if (begin==end) return false;
      do {
        begin++;
        position++;
      }
      while (!((*begin) == true) && !(begin==end));
      if (begin==end) return false;
      return true;
    }

    virtual bool valid() const override {
      return !(begin==end);
    }

    virtual ~BitsetIterator() {}

  private:
    Iterator begin, end;
    int position;
};

template <typename Iterator>
class VectorIterator : public ListIterator {
  public:
    VectorIterator(Iterator _begin, Iterator _end) {
      begin = _begin;
      end = _end;
   }
    
    virtual unsigned int current() const override {
      if (!valid()) return -1;
      return *begin;
    }

    virtual bool next() override {
      if (begin==end) return false;
      begin++;
      return true;
    }

    virtual bool valid() const override {
      return !(begin==end);
    }

    virtual ~VectorIterator() {}

  private:
    Iterator begin, end;
    int position;
};


class BitsetList : public ListOfIncidents {
  public: 
    BitsetList(const std::vector<unsigned int> &list) {
      int sz = *std::max_element(list.begin(), list.end());
      incidents.resize(sz+1);
      for (int i=0;i<list.size();i++) incidents[list[i]] = true;
      optimized = true;
    }

    BitsetList(const std::vector<bool> &list) {
      incidents = list; 
      optimized = false;
    }

    virtual std::unique_ptr<ListIterator> get_iterator() const override {
      std::unique_ptr<ListIterator> get( new BitsetIterator<std::vector<bool>::const_iterator >(incidents.begin(), incidents.end() ) );
      return std::move(get);
    }

    virtual void optimize() override {
      if (!optimized) {
        unsigned int last = -1;
        for (int pos=0;pos<(int)incidents.size();pos++)
          if (incidents[pos]) last = pos;
        incidents.resize(last+1);
        optimized = true;
      }
    }

    virtual bool is_connected(unsigned int vertex) const override {
      if (vertex >= incidents.size() ) return false;
      return incidents[vertex];
    }

    virtual ~BitsetList() {}

  private:
    std::vector<bool> incidents;
    bool optimized;
};

class VectorList : public ListOfIncidents {
  public: 
    VectorList(const std::vector<unsigned int> &list) {
      incidents = list;
      optimized = false;
    }

    VectorList(const std::vector<bool> &list) {
      for (int pos=0;pos<(int)list.size();pos++)
        if (list[pos]) incidents.push_back(pos);
      optimized = true;
    }

    virtual std::unique_ptr<ListIterator> get_iterator() const override {
      std::unique_ptr<ListIterator> get(new VectorIterator<std::vector<unsigned int>::const_iterator >(incidents.begin(), incidents.end() ) );
      return std::move(get);
    }

    virtual void optimize() override {
      if (!optimized) {
        std::sort(incidents.begin(), incidents.end());
        optimized = true;
      }
    }

    virtual bool is_connected(unsigned int vertex) const override {
      if (optimized) {
        return std::binary_search(incidents.begin(), incidents.end(), vertex);
      }
      else {
        return std::find(incidents.begin(), incidents.end(), vertex) != incidents.end();
      }
    }

    virtual ~VectorList() {}

  private:
    std::vector<unsigned int> incidents;
    bool optimized;
};


#endif
