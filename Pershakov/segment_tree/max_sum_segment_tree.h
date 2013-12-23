#ifndef MAX_SUM_SEGMENT_TREE_H

#define MAX_SUM_SEGMENT_TREE_H

template <class T> struct Segment {
    Segment() { 
        fake = true;
    }

    Segment(int new_left, int new_right, const T &new_sum) {
        left = new_left;
        right = new_right;
        sum = new_sum;
        fake = false;
    }

    int left, right;
    T sum;
    bool fake;
};

template 
<class T, class Comparator, class Summator, class Multiplier, T neutral_sum>
class MaxSumSegmentTree {
    public:

        MaxSumSegmentTree() { }

        explicit MaxSumSegmentTree(const std::vector<T> &base) {
            std::vector<TreeNode> base_for_tree(base.size());
            for (int i = 0; i < (int)base.size(); i++)
                base_for_tree[i] = TreeNode(base[i], i);
            tree = SegmentTree<TreeNode, UpdInfo>(base_for_tree);
        }

        Segment<T> get(int l, int r) {
            return tree.get(l, r).getSegment();
        }

        void assign(int l, int r, const T &val){
            UpdInfo upd(val);
            tree.update(l, r, upd);
        }

    private:

        typedef UpdInfoAssignment<T> UpdInfo;

        class TreeNode {
            public:

                TreeNode() { 
                    val = suf = pref = Segment<T>();
                }

                TreeNode(const T &new_val, int pos) {
                    val = suf = pref = Segment<T>(pos, pos, new_val);
                    full_sum = new_val;
                }

                Segment<T> getSegment() const {
                    return val;
                }

                void merge(const TreeNode &left, const TreeNode &right) {
                    if (left.val.fake) {
                        copyFrom(right);
                        return;
                    }
                    if (right.val.fake) {
                        copyFrom(left);
                        return;
                    }
                    full_sum = sum(left.full_sum, right.full_sum);
                    mergeSuf(left, right);
                    mergePref(left, right);
                    mergeVal(left, right);
                }

                void addUpdate(const UpdInfo &upd, int l, int r) {
                    if (upd.is_assigned) {
                        if (cmp(neutral_sum, upd.assigned)) {
                            full_sum = mult(upd.assigned, r - l + 1);
                            val = suf = pref = Segment<T>(l, r, full_sum);
                        } else {
                            full_sum = mult(upd.assigned, r - l + 1);
                            val.sum = pref.sum = suf.sum = upd.assigned;
                            val.left = val.right = pref.right = l;
                            suf.left = r;
                        }
                    } 
                }

                Segment<T> val, pref, suf; 
                T full_sum;

            private:

                void copyFrom(const TreeNode &from) {
                    val = from.val;
                    suf = from.suf;
                    pref = from.pref;
                    full_sum = from.full_sum;
                }

                void mergeSuf(const TreeNode &left, const TreeNode &right) {
                    suf = right.suf;
                    if (cmp(suf.sum, sum(left.suf.sum, right.full_sum))) {
                        suf.left = left.suf.left;
                        suf.sum = sum(left.suf.sum, right.full_sum);
                    }
                }

                void mergePref(const TreeNode &left, const TreeNode &right) {
                    pref = left.pref;
                    if (cmp(pref.sum, sum(left.full_sum, right.pref.sum))) {
                        pref.right = right.pref.right;
                        pref.sum = sum(left.full_sum, right.pref.sum);
                    }
                }

                void mergeVal(const TreeNode &left, const TreeNode &right) {
                    if (cmp(left.val.sum, right.val.sum))
                        val = right.val;
                    else
                        val = left.val;
                    if (cmp(val.sum, sum(left.suf.sum, right.pref.sum))) {
                        val.left = left.suf.left;
                        val.right = right.pref.right;
                        val.sum = sum(left.suf.sum, right.pref.sum);
                    }
                }

                Summator sum;
                Comparator cmp;
                Multiplier mult;
        };

        SegmentTree<TreeNode, UpdInfo> tree;
};

#endif
