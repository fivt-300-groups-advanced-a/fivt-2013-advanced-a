
template<typename ReturnType, typename MetaInformation,
         typename MergeMeta, typename MergeReturn, typename MergeMetaReturn>
class GeneralSegmentTree {
  
  private:
    struct Vertex {
      Vertex(): ret(ReturnType()), meta(NULL), l(0), r(0) {}
      Vertex *l, *r;
      ReturnType ret;
      MetaInformation *meta;
    };

    void clean(Vertex* &cur) {
      if (!cur) return;
      clean(cur->l);
      clean(cur->r);
      delete cur;
    }

    void make_childs(Vertex *cur){
      if (!cur->l) cur->l = new Vertex();
      if (!cur->r) cur->r = new Vertex();
    }
    
    void push(Vertex *cur, long long left, long long right) {
      if (left==right) return;
      if (!cur->meta) return;
      long long m = (left+right)/2;
      merge_meta(cur->l->meta, *(cur->meta), left, m);
      merge_meta(cur->r->meta, *(cur->meta), m+1, right);
      merge_meta_return(cur->l->ret, *(cur->meta), left, m);
      merge_meta_return(cur->r->ret, *(cur->meta), m+1, right);
      delete cur->meta;
      cur->meta = 0;
    }

    ReturnType inside_get(Vertex *cur, long long cur_left, long long cur_right, long long left, long long right) {
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

    void inside_change(Vertex *cur, long long cur_left, long long cur_right, long long left, long long right, MetaInformation &to_change) {
      if (right > cur_right || cur_left > left || left > right) return;
      make_childs(cur);
      push(cur, cur_left, cur_right);
      if (cur_left==left && cur_right==right) {
        merge_meta(cur->meta, to_change, left, right);
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
    
    GeneralSegmentTree<ReturnType, MetaInformation, MergeMeta, MergeReturn, MergeMetaReturn> (long long l, long long r) {
      global_left = l;
      global_right = r;
      head = new Vertex();
    }
/*  
    ~GeneralSegmentTree<ReturnType, MetaInformation, MergeMeta, MergeReturn, MergeMetaReturn> {
      clean(head);
    }
*/

  private:
    Vertex* head = NULL;
    long long global_left, global_right;
    MergeMeta merge_meta;
    MergeReturn merge_return;
    MergeMetaReturn merge_meta_return;
};
