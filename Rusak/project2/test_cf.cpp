#include <iostream>
#include <fstream>

template< typename T, typename Cmp>
class Heap {
  private:
    struct Vertex {
      Vertex(T val):key(val), parent(NULL), child(NULL), sibling(NULL), degree(0) { }
      T key;
      Vertex *parent, *child, *sibling;
      unsigned short degree;
    };

  public:
    /* Debug
    void puts_list(Vertex *head) {
      for (;head;head = head->sibling) std::cout << head->degree << " ";
      std::cout << "\n";
    }

    bool check_list(Vertex *head) {
      if (!head) return true;
      Vertex* cur = head;
      bool ok = true;
      for (;cur->sibling;cur = cur->sibling) {
        if (cur->degree > cur->sibling->degree) ok = false;
      }
      return ok;
    }

    void print_tree() {
      print(head);
    }

    void print(Vertex* head) {
      if (!head) return;
      std::cout << "Val " << head->key << "\n";
      print(head->child);
      print(head->sibling);
    }
    */

    void merge(Heap<T, Cmp> &other) {
      if (other.head == NULL) return;

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
          if (!Cmp()(fir->key, sec->key)) std::swap(fir, sec);
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
    }

    void insert(T val) {
      Heap<T, Cmp> nw = Heap(val);
      merge(nw);
    }

    void extract_min() {
      //find vertex with min key and save its degree
      T minkey = head->key;
      unsigned short minkeydeg = head->degree; 
      for( Vertex* cur = head; cur; cur = cur->sibling ) {
        if (Cmp()(cur->key, minkey)) {
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
        fir->parent = NULL;
        heap.head = fir;
        fir = next;
      }
      merge(heap);
    }


    T find_min() {
      Vertex* cur = head;
      T min = cur->key;
      while (cur) {
        if (Cmp()(cur->key, min)) min = cur->key;
        cur = cur->sibling;
      }
      return min;
    }


    Heap(T val) {
      head = new Vertex(val);
    }

    Heap() {
      head = NULL;
    }

  private:
    inline void connect_and_move(Vertex* &head, Vertex* &to) {
      head->sibling = to;
      to = to->sibling;
      head = head->sibling;
    }
    
    Vertex* head;
};

template< typename T, typename Cmp = std::less<T> >
Heap<T, Cmp> MakeHeap(Cmp cmp = Cmp()) {
  return Heap<T, Cmp>();
}

int main() {
  std::ifstream in;
  std::ofstream out;
  in.open ("qsort.in");
  out.open ("qsort.out");
  std::ios_base::sync_with_stdio(0);
  auto heap = MakeHeap<int>();
  int n; in >> n;
  for (int i=0;i<n;i++) {
    int a; in >> a; heap.insert(a);
  }
  for (int i=0;i<n;i++) {
      out << heap.find_min() << " ";
      heap.extract_min();
  }
  out << "\n";
  return 0;
}
