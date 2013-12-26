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
      void operator()(MetaInformation &to, bool is_meta, const MetaInformation &add_meta, long long l, long long r) {
        to = add_meta;
      }
    };

    struct MergeReturn {
      ReturnType operator()(const ReturnType &fir, const ReturnType &sec) {
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
