
template<typename T, typename Return> 
class VectorTree {
  public:
    explicit VectorTree(int sz) {
      mem.resize(sz);
    }
  
    Return get(long long l, long long r) {
      Return ret(mem[l], mem[l], mem[l]);
      for (int i=l+1;i<=r;i++) {
        ret.sum += mem[i];
        if (mem[i] < ret.min) ret.min = mem[i];
        if (ret.max < mem[i]) ret.max = mem[i];
      }
      return ret;
    }
    void assign(long long l, long long r, T elem) {
      for (int i=l;i<=r;i++) mem[i] = elem;
    } 
    void add(long long l, long long r, T elem) {
      for (int i=l;i<=r;i++) mem[i] += elem;
    }
  private:
    std::vector<T> mem;

};
