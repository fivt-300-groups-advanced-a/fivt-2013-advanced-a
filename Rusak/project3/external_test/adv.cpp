#include <iostream>
#include <fstream>

// Equal to GeneralSegmentTree, but with clever memory allocation and pesistance
#include <vector>

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
      merge_meta(cur->l->meta, cur->meta, left, m);
      cur->l->is_meta = true;
      merge_meta(cur->r->meta, cur->meta, m+1, right);
      cur->r->is_meta = true;
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
        merge_meta(cur->meta, to_change, left, right);
        cur->is_meta = true;
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
    
    GeneralSegmentTree(long long l, long long r) {
      global_left = l;
      global_right = r;
      head = allocator.allocate();
    }

  private:
    Vertex* head;
    long long global_left, global_right;
    MergeMeta merge_meta;
    MergeReturn merge_return;
    MergeMetaReturn merge_meta_return;
    MemoryAllocator allocator;
};

template<typename T, T NeutralSum>
class AssignSumSegmentTree {
  private:
    struct ReturnType {
      explicit ReturnType(T x):sum(x) {}
      ReturnType():sum(NeutralSum) {}
      T sum;
    };

    struct MetaInformation {
      explicit MetaInformation(T x):assign(x) {}
      MetaInformation() {}
      T assign;
    };

    struct MergeMeta {
      void operator()(MetaInformation &to, const MetaInformation &add_meta, long long l, long long r) {
        to.assign = add_meta.assign;
      }
    };

    struct MergeReturn {
      ReturnType operator()(const ReturnType &fir, const ReturnType &sec, long long l, long long r) {
        return ReturnType(fir.sum + sec.sum);
      }
    };

    struct MergeMetaReturn {
      void operator()(ReturnType &to, const MetaInformation &add_meta, long long l, long long r) {
        to.sum = (r-l+1)*add_meta.assign;
      }
    };

  public:
    T sum(long long l, long long r) {
      return tree->get(l, r).sum;
    }

    void assign(long long l, long long r, T x) {
      tree->change(l, r, MetaInformation(x));
    }

    AssignSumSegmentTree(long long l, long long r) {
      tree = new GeneralSegmentTree<ReturnType, MetaInformation,
                                    MergeMeta, MergeReturn, MergeMetaReturn>(l, r);
    }

    ~AssignSumSegmentTree() {
      delete tree;
    }

  private:
    GeneralSegmentTree<ReturnType, MetaInformation,
                       MergeMeta, MergeReturn, MergeMetaReturn> *tree;
};

int main() {
  std::ifstream ifs("sum.in");
  std::ofstream ofs("sum.out");
  std::ios_base::sync_with_stdio(0);
  int n, m;
  ifs >> n >> m;
  AssignSumSegmentTree<long long, 0> tree(1, n);
  for (int i=0;i<m;i++) {
    int l, r;
    char c = 0;
    while (c!='A' && c!='Q') ifs >> c;
    ifs >> l >> r;
    if (c=='A') {
      int x; ifs >> x;
      tree.assign(l, r, x);
    }
    else {
      ofs << tree.sum(l, r) << "\n";
    }
  }
  ifs.close();
  ofs.close();
  return 0;
}

