 //Equal to GeneralSegmentTree, but with clever memory allocation and pesistance(todo)
 //You need to relize classes:
 //  ReturnType
 //    Must have ReturnType() constructor, which fill it by neutral elements.
 //    Must consist all data to return.
 //
 //  MetaInformation
 //    Must have MetaInformation() constructor, can do nothing.
 //    Must const all data to save in each vertex.
 //
 //  MergeMeta
 //    Must have:
 //       void operator()(MetaInformation &to, bool is_meta, const MetaInformation &add_meta, long long l, long long r) {}
 //       This function must apply add_meta on "to".
 //       is_meta means, that "to" consist valid meta.
 //
 //  MergeReturn 
 //    Must have:
 //      ReturnType operator()(const ReturnType &fir, const ReturnType &sec) {}
 //      This gets two return types and return their union.
 //
 //  MergeMetaReturn
 //    Must have:
 //     void operator()(ReturnType &to, const MetaInformation &add_meta, long long l, long long r) {}
 //     This gets return type and apply meta on it.
 //
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
        for (size_t i = 0; i < blocks.size(); i++) delete[] blocks[i];
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
      cur->r->is_meta = true;
      cur->l->is_meta = true;
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
                          inside_get(cur->r, m+1, cur_right, m+1, right) );
    }

    void inside_change(Vertex* cur, long long cur_left, long long cur_right, long long left, long long right, MetaInformation &to_change) {
      if (right > cur_right || cur_left > left || left > right) return;
      make_childs(cur);
      push(cur, cur_left, cur_right);
      if (cur_left==left && cur_right==right) {
        merge_meta(cur->meta, cur->is_meta, to_change, left, right);
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
      cur->ret = merge_return(cur->l->ret, cur->r->ret);
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
