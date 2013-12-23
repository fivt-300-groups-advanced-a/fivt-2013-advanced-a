#ifndef MIN_MAX_SUM_TREE_H

#define MIN_MAX_SUM_TREE_H

#include <vector>

#include "advanced_segment_tree.h"
#include "upd_info.h"

template<class T> struct MinMaxSum {
    MinMaxSum() { }

    MinMaxSum(const T &new_min, const T &new_max, const T &new_sum) {
        min = new_min;
        max = new_max;
        sum = new_sum;
    }

    bool operator== (const MinMaxSum &oth) const {
        return (min == oth.min && max == oth.max && sum == oth.sum);
    }

    T min, max, sum;
};

template<class T, class Comparator, class Summator> 
MinMaxSum<T> mergeMinMaxSum(const MinMaxSum<T> &left, const MinMaxSum<T> &right) {
    Comparator cmp;
    Summator sum;
    MinMaxSum<T> val;
    val.sum = sum(left.sum, right.sum);
    if (cmp(left.min, right.min))
        val.min = left.min;
    else
        val.min = right.min;
    if (cmp(right.max, left.max))
        val.max = left.max;
    else
        val.max = right.max;
    return val;
}

/*
 * Min-max-sum segment tree with add and assign operations
 */
template <class T, class Comparator, class Summator, class Multiplier, 
         T neutral_min, T neutral_max, T neutral_sum>
class MinMaxSumAssignmentAddTree {
    public:

        MinMaxSumAssignmentAddTree() { }

        explicit MinMaxSumAssignmentAddTree(const std::vector<T> &base) {
            std::vector<TreeNode> base_for_tree(base.size());
            for (int i = 0; i < (int)base.size(); i++)
                base_for_tree[i] = TreeNode(base[i]);
            tree = SegmentTree<TreeNode, UpdInfo>(base_for_tree);
        }

        MinMaxSum<T> get(int l, int r) {
            return tree.get(l, r).getVal();
        }

        void assign(int l, int r, const T &val) {
            UpdInfo upd(val, 0, true);
            tree.update(l, r, upd);
        }

        void add(int l, int r, const T &val) {
            UpdInfo upd(0, val, false);
            tree.update(l, r, upd);
        }

    private:    

        typedef UpdInfoAssignmentAdd<T, Summator, neutral_sum> UpdInfo;

        class TreeNode {
            public:

                TreeNode() { 
                    val = MinMaxSum<int>(neutral_min, neutral_max, neutral_sum);
                }

                explicit TreeNode(const T &new_val) {
                    val = MinMaxSum<int>(new_val, new_val, new_val);
                }

                MinMaxSum<T> getVal() const {
                    return val;
                }

                void merge(const TreeNode &left, const TreeNode &right) {
                    val = mergeMinMaxSum<T, Comparator, Summator>
                        (left.val, right.val);
                }

                void addUpdate(const UpdInfo &upd, int l, int r) {
                    if (upd.is_assigned) {
                        val.min = val.max = sum(upd.assigned, upd.added);
                        val.sum = mult(sum(upd.assigned, upd.added), r - l + 1);
                    } else {
                        val.min = sum(val.min, upd.added);
                        val.max = sum(val.max, upd.added);
                        val.sum = sum(val.sum, mult(upd.added, r - l + 1));
                    }
                }

                MinMaxSum<T> val; 
                Summator sum;
                Comparator cmp;
                Multiplier mult;
        };

        SegmentTree<TreeNode, UpdInfo> tree;
};

/*
 * Min-max-sum segment tree with assign operations
 */
template <class T, class Comparator, class Summator, class Multiplier, 
         T neutral_min, T neutral_max, T neutral_sum>
class MinMaxSumAssignmentTree {
    public:

        MinMaxSumAssignmentTree() { }

        explicit MinMaxSumAssignmentTree (const std::vector<T> &base) {
            std::vector<TreeNode> base_for_tree(base.size());
            for (int i = 0; i < (int)base.size(); i++)
                base_for_tree[i] = TreeNode(base[i]);
            tree = SegmentTree<TreeNode, UpdInfo>(base_for_tree);
        }

        MinMaxSum<T> get(int l, int r) {
            return tree.get(l, r).getVal();
        }

        void assign(int l, int r, const T &val) {
            UpdInfo upd(val);
            tree.update(l, r, upd);
        }

    private:   

        typedef UpdInfoAssignment<T> UpdInfo;

        class TreeNode {
            public:

                TreeNode() { 
                    val = MinMaxSum<T>(neutral_min, neutral_max, neutral_sum);
                }

                explicit TreeNode(const T &new_val) {
                    val = MinMaxSum<T>(new_val, new_val, new_val);
                }

                MinMaxSum<T> getVal() const {
                    return val;
                }

                void merge(const TreeNode &left, const TreeNode &right) {
                    val = mergeMinMaxSum<T, Comparator, Summator>
                        (left.val, right.val);
                }

                void addUpdate(const UpdInfo &upd, int l, int r) {
                    if (upd.is_assigned) {
                        val.min = val.max = upd.assigned;
                        val.sum = mult(upd.assigned, r - l + 1);
                    }
                }

                MinMaxSum<T> val; 
                Summator sum;
                Comparator cmp;
                Multiplier mult;
        };

        SegmentTree<TreeNode, UpdInfo> tree;
};

/*
 * Min-max-sum segment tree with add  operations
 */
template <class T, class Comparator, class Summator, class Multiplier, 
         T neutral_min, T neutral_max, T neutral_sum>
class MinMaxSumAddTree {
    public:

        MinMaxSumAddTree() { }

        explicit MinMaxSumAddTree (const std::vector<T> &base) {
            std::vector<TreeNode> base_for_tree(base.size());
            for (int i = 0; i < (int)base.size(); i++)
                base_for_tree[i] = TreeNode(base[i]);
            tree = SegmentTree<TreeNode, UpdInfo>(base_for_tree);
        }

        MinMaxSum<T> get(int l, int r) {
            return tree.get(l, r).getVal();
        }

        void add(int l, int r, const T &val) {
            UpdInfo upd(val);
            tree.update(l, r, upd);
        }

    private:   

        typedef UpdInfoAdd<T, Summator, neutral_sum> UpdInfo;

        class TreeNode {
            public:

                TreeNode() { 
                    val = MinMaxSum<T>(neutral_min, neutral_max, neutral_sum);
                }

                explicit TreeNode(const T &new_val) {
                    val = MinMaxSum<T>(new_val, new_val, new_val);
                }

                MinMaxSum<T> getVal() const {
                    return val;
                }

                void merge(const TreeNode &left, const TreeNode &right) {
                    val = mergeMinMaxSum<T, Comparator, Summator>
                        (left.val, right.val);
                }

                void addUpdate(const UpdInfo &upd, int l, int r) {
                    val.min = sum(val.min, upd.added);
                    val.max = sum(val.max, upd.added);
                    val.sum = sum(val.sum, mult(upd.added, r - l + 1));
                }

                MinMaxSum<T> val; 
                Summator sum;
                Comparator cmp;
                Multiplier mult;
        };

        SegmentTree<TreeNode, UpdInfo> tree;
};
#endif
