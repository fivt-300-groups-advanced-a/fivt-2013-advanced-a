#include <cstdlib>
#include <algorithm>

template< typename T, typename Cmp>
class Heap {

  private:
    template <class U, class Z> friend class HeapTest;

    class Vertex {
      private:
        friend class Heap;
        template <class U, class Z> friend class HeapTest;

        Vertex(T val):key(val), parent(0), child(0), sibling(0), degree(0) { }
        T key;
        Vertex *parent, *child, *sibling;
        unsigned short degree;
    };
    
  public:
    /*  
    class Reference {
      friend class Heap; 
      
      public:
        Reference() {}
      
      private:
        explicit Reference(Vertex *get):ref(get) {}
        Vertex **ref;
    };
    */

    void merge(Heap<T, Cmp> &other) {
      if (!other.head) return;

      //create fictive vertex
      Vertex* new_head = new Vertex(-1);
      
      //merge lists of vertexes
      Vertex *curh = new_head, *curh1 = head, *curh2 = other.head;
      while (curh1 || curh2) {
        if (!curh2 || (curh1 && curh1->degree < curh2->degree ) ) connect_and_move(curh, curh1);
        else connect_and_move(curh, curh2);
      }

      //reduce vertexes of equal size
      curh = new_head;
      while (curh->sibling->sibling) {
        Vertex *fir = curh->sibling;
        Vertex *sec = fir->sibling;
        Vertex* next = sec->sibling; 
        if (fir->degree == sec->degree && (!next || sec->degree!=next->degree)) {
          if (!cmp(fir->key, sec->key)) std::swap(fir, sec);
          sec->sibling = fir->child;
          fir->child = sec;
          sec->parent = fir;
          curh->sibling = fir;
          fir->sibling = next;
          fir->degree++;
        }
        else {
          curh = curh->sibling;
        }
      }
      head = new_head->sibling;
      delete new_head;

      //clean second heap
      other.head = 0;
    }

    /*  Reference */ void insert(T val) {
      Heap<T, Cmp> nw = Heap(val);
      //Reference result(nw.head);
      merge(nw);
      //return result;
    }

    void extract_min() {
      //find vertex with min key and save its degree
      T minkey = head->key;
      unsigned short minkeydeg = head->degree; 
      for( Vertex* cur = head; cur; cur = cur->sibling ) {
        if (cmp(cur->key, minkey)) {
          minkey = cur->key;
          minkeydeg = cur->degree;
        }
      }

      //delete min vertex 
      Vertex* del;
      if (head->degree==minkeydeg) {
        del = head;
        head = head->sibling;
      }
      else {
        Vertex* cur = head;
        while (cur->sibling->degree != minkeydeg) cur = cur->sibling;
        del = cur->sibling;
        cur->sibling = del->sibling;
      }
      //create heap of childs and merge
      Heap<T, Cmp> heap = Heap();
      Vertex* fir = del->child;
      delete del;
      while (fir) {
        Vertex* next = fir->sibling;
        fir->sibling = heap.head;
        fir->parent = 0;
        heap.head = fir;
        fir = next;
      }
      merge(heap);
    }

    bool empty() {
      return (!head);
    }



    T find_min() {
      Vertex* cur = head;
      T min = cur->key;
      while (cur) {
        if (cmp(cur->key, min)) min = cur->key;
        cur = cur->sibling;
      }
      return min;
    }


    Heap(T val, Cmp comp = Cmp()) {
      head = new Vertex(val);
      cmp = comp;
    }

    Heap(Cmp comp = Cmp()) {
      head = 0;
      cmp = comp;
    }

  private:
    inline void connect_and_move(Vertex* &head, Vertex* &to) {
      head->sibling = to;
      to = to->sibling;
      head = head->sibling;
    }
    
    Vertex* head;
    Cmp cmp;
};

template< typename T, typename Cmp = std::less<T> >
Heap<T, Cmp> MakeHeap(Cmp cmp = Cmp()) {
  return Heap<T, Cmp>(cmp);
}
