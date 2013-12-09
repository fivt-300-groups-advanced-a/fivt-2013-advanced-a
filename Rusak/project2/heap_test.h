
template<typename T, typename Cmp>
class HeapTest {
  
  public: 
    bool check_invariants(Heap<T, Cmp> &heap) {
      bool ok = true;
      bool was[100];
      for (Heap<T, Cmp>::Vertex* head = heap.head; ok && head; head = head->sibling) {
        if (was[head->degree]) ok = false;
        was[head->degree] = 1;
        if (head->sibling && head->sibling->degree >= head->degree) ok = false;
        check_invariants(head, head->degree);
      }
      return ok;
    }

    bool check_invariants(Heap<T, Cmp>::Vertex* vert, int deg) {
      if (!vert) return true;
      if (vert->degree!=deg) return true;
      int wd = deg-1;
      bool ok = true;
      for (Heap<T, Cmp>::Vertex* fir = vert->child; ok && fir; fir = fir->sibling, wd--) {
        ok = ok && check_invariants(fir, wd) && cmp(vert->key, fir->key);
      }
      return ok;
    }

    HeapTest(Cmp comp) {
      cmp = comp;
    }

  private:
    Cmp cmp;    
};

typename<typename T, typename Cmp = std::less<T> > MakeHeapTest(Cmp cmp = Cmp()) {
  return HeapTest<T, Cmp>(cmp);
}
