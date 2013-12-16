template<typename T, typename Cmp>
class HeapTest {

  typedef typename Heap<T, Cmp>::Vertex Vertex_type;
  typedef Heap<T, Cmp> Heap_type;
  
  public: 
    bool check_inv_vert(Vertex_type* vert, int deg) {
      if (!vert) return true;
      if (vert->degree!=deg) return false;
      int wd = deg-1;
      bool ok = true;
      for (Vertex_type* fir = vert->child; ok && fir; fir = fir->sibling, wd--) {
        ok = ok & check_inv_vert(fir, wd) & (!cmp(fir->key, vert->key));
      }
      if (!ok) {
        std::cout << "OH\n";
        exit(0);
      }
      return ok;
    }

    bool check_inv_heap(Heap_type &heap) {
      bool ok = true;
      bool was[100];
      for (int i=0;i<100;i++) was[i] = 0;
      for (Vertex_type* head = heap.head; ok && head; head = head->sibling) {
        if (was[head->degree]) ok = false;
        was[head->degree] = 1;
        if (head->sibling && head->sibling->degree <= head->degree) ok = false;
        ok = ok & check_inv_vert(head, head->degree);
      }
      return ok;
    }

    int size_vert(Vertex_type* head) {
      if (!head) return 0;
      int col = 1;
      for (Vertex_type* f = head->child; f; f = f->sibling) col+=size_vert(f);
      return col;
    }

    int size_heap(Heap_type &heap) {
      int col = 0;
      for (Vertex_type* head = heap.head; head; head = head->sibling) col+=size_vert(head);
      return col;
    }

    HeapTest(Cmp comp) {
      cmp = comp;
    }

  private:
    Cmp cmp;    
};

template<typename T, typename Cmp = std::less<T> > 
HeapTest<T, Cmp> MakeHeapTest(Cmp cmp = Cmp()) {
  return HeapTest<T, Cmp>(cmp);
}
