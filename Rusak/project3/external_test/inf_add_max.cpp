#include <vector>
#include <climits>
#include <iostream>
#include <fstream>

template<typename ReturnType, typename MetaInformation,
         typename MergeMeta, typename MergeReturn, typename MergeMetaReturn>
class GeneralSegmentTree {
  
  private:
    struct Vertex {
      Vertex(): ret(ReturnType()), is_meta(false), l(0), r(0) {}
      Vertex *l, *r;
      ReturnType ret;
      MetaInformation meta;
      bool is_meta;
    };

    struct MemoryAllocator {
      Vertex* allocate() {
        if (left==0) {
          left = (1ll << (int)blocks.size() );
          begin = new Vertex[left];
          blocks.push_back(begin);
        }
        left--;
        return (begin++);
      }
       
      ~MemoryAllocator() {
        for (size_t i = 0; i < blocks.size(); i++) delete blocks[i];
      }

      int left = 0;
      Vertex* begin;
      std::vector<Vertex*> blocks;
    };


    void make_childs(Vertex *cur){
      if (!cur->l) cur->l = allocator.allocate();
      if (!cur->r) cur->r = allocator.allocate();
    }

    void push(Vertex *cur, long long left, long long right) {
      if (left==right) return;
      if (!cur->is_meta) return;
      long long m = (left+right)/2;
      merge_meta(cur->l->meta, cur->l->is_meta, cur->meta, left, m);
      merge_meta(cur->r->meta, cur->r->is_meta, cur->meta, m+1, right);
      merge_meta_return(cur->l->ret, cur->meta, left, m);
      merge_meta_return(cur->r->ret, cur->meta, m+1, right);
      cur->is_meta = false;
    }

    ReturnType inside_get(Vertex* cur, long long cur_left, long long cur_right, long long left, long long right) {
      if (right > cur_right || cur_left > left || left > right) return ReturnType();
      if (cur_left == left && cur_right == right) {
        return cur->ret;
      }
      make_childs(cur);
      push(cur, cur_left, cur_right);
      long long m = (cur_left+cur_right)/2;
      if (right<=m) return inside_get(cur->l, cur_left, m, left, right);
      if (left>m) return inside_get(cur->r, m+1, cur_right, left, right);
      return merge_return(inside_get(cur->l, cur_left, m, left, m), 
                          inside_get(cur->r, m+1, cur_right, m+1, right),
                          cur_left, cur_right );
    }

    void inside_change(Vertex* cur, long long cur_left, long long cur_right, long long left, long long right, MetaInformation &to_change) {
      if (right > cur_right || cur_left > left || left > right) return;
      make_childs(cur);
      push(cur, cur_left, cur_right);
      if (cur_left==left && cur_right==right) {
        merge_meta(cur->meta, cur->is_meta, to_change, left, right);
        merge_meta_return(cur->ret, to_change, left, right);
        return;
      }
      long long m = (cur_left+cur_right)/2;
      if (right<=m) inside_change(cur->l, cur_left, m, left, right, to_change);
      else if (left>m) inside_change(cur->r, m+1, cur_right, left, right, to_change);
      else {
        inside_change(cur->l, cur_left, m, left, m, to_change);
        inside_change(cur->r, m+1, cur_right, m+1, right, to_change);
      }      
      cur->ret = merge_return(cur->l->ret, cur->r->ret, cur_left, cur_right);
    }

  public:

    ReturnType get(long long l, long long r) {
      return inside_get(head, global_left, global_right, l, r);
    }

    void change(long long l, long long r, MetaInformation to_change) {
      inside_change(head, global_left, global_right, l, r, to_change);
    }
    
    GeneralSegmentTree(long long l, long long r, MergeMeta mm = MergeMeta(), MergeReturn mr = MergeReturn(), MergeMetaReturn mmr = MergeMetaReturn() ) {
      global_left = l;
      global_right = r;
      head = allocator.allocate();
      merge_meta = mm;
      merge_return = mr;
      merge_meta_return = mmr;
    }

  private:
    Vertex* head;
    long long global_left, global_right;
    MergeMeta merge_meta;
    MergeReturn merge_return;
    MergeMetaReturn merge_meta_return;
    MemoryAllocator allocator;
};


// T must have operator + and <
template<typename T, T NeutralSum, T NeutralMin, T NeutralMax>
class AssignAddMinMaxSumSegmentTree {
  public:
    struct ReturnType {
      ReturnType(T x, T y, T z):sum(x), min(y), max(z) {}
      ReturnType():sum(NeutralSum), min(NeutralMin), max(NeutralMax) {}
      T sum;
      T min;
      T max;
    };

  private:
    struct MetaInformation {
      MetaInformation(T x, bool oper):val(x), operation(oper) {}
      MetaInformation() {}
      bool operation; // 0 = add, 1 = assign
      T val;
    };

    struct MergeMeta {
      void operator()(MetaInformation &to, bool & is_meta, const MetaInformation &add_meta, long long l, long long r) {
        if (!is_meta) {
          to = add_meta;
          is_meta = true;
          return;
        }
        if (add_meta.operation) {
          to.val = add_meta.val;
          to.operation = 1;
        }
        else {
          to.val += add_meta.val;
        }
      }
    };

    struct MergeReturn {
      ReturnType operator()(const ReturnType &fir, const ReturnType &sec, long long l, long long r) {
        return ReturnType(fir.sum + sec.sum, (fir.min < sec.min)?fir.min:sec.min, (fir.max < sec.max)?sec.max:fir.max );
      }
    };

    struct MergeMetaReturn {
      void operator()(ReturnType &to, const MetaInformation &add_meta, long long l, long long r) {
        if (add_meta.operation) {
          to.sum = (r-l+1)*add_meta.val;
          to.max = add_meta.val;
          to.min = add_meta.val;
        }
        else {
          to.sum += (r-l+1)*add_meta.val;
          to.max += add_meta.val;
          to.min += add_meta.val;
        }
      }
    };

  public:

    ReturnType get(long long l, long long r) {
      return tree->get(l, r);
    }

    void assign(long long l, long long r, T x) {
      tree->change(l, r, MetaInformation(x, true));
    }

    void add(long long l, long long r, T x) {
      tree->change(l, r, MetaInformation(x, false));
    }


    AssignAddMinMaxSumSegmentTree(long long l, long long r)
    {
      tree = new GeneralSegmentTree<ReturnType, MetaInformation,
                                    MergeMeta, MergeReturn, MergeMetaReturn>(l, r);
    }

    ~AssignAddMinMaxSumSegmentTree() {
      delete tree;
    }

  private:
    GeneralSegmentTree<ReturnType, MetaInformation,
                       MergeMeta, MergeReturn, MergeMetaReturn> *tree;
};

int main() {
  std::ifstream ifs("input.txt");
  std::ofstream ofs("output.txt");
  std::ios_base::sync_with_stdio(0);
  int n, m;
  ifs >> n;
  AssignAddMinMaxSumSegmentTree<long long, 0, LLONG_MAX, LLONG_MIN> tree(1, n);
  for (int i=1;i<=n;i++) {
    int c; ifs >> c;
    tree.assign(i, i, c);
  }
  ifs >> m;
  for (int i=0;i<m;i++) {
    char c = 0;
    while (c!='g' && c!='a') ifs >> c;
    if (c=='a') {
      int l, r, val;
      ifs >> l >> r >> val;
      tree.add(l, r, val);
    }
    else {
      int pos; ifs >> pos;
      ofs << tree.get(pos, pos).max << " ";
    }
  }
  ifs.close();
  ofs.close();
}

