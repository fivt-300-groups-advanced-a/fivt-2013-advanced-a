
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
      void operator()(MetaInformation &to, bool is_meta, const MetaInformation &add_meta, long long l, long long r) {
        if (!is_meta) {
          to = add_meta;
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
      ReturnType operator()(const ReturnType &fir, const ReturnType &sec) {
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

