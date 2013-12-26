#ifndef CNT_EQUALITY_SEGMENTS_TREE_H

#define CNT_EQUALITY_SEGMENTS_TREE_H

#include <vector>

#include "advanced_segment_tree.h"
#include "upd_info.h"

/*
 * Segment tree for counting the number of equality segments 
 * Allows assignment and += updates
 */
template <class T, class Comparator, class Summator, T neutral_sum>
class CntEqualitySegmentsTree {
    public:

        CntEqualitySegmentsTree() { }

        explicit CntEqualitySegmentsTree (const std::vector<T> &base) {
            std::vector<TreeNode> base_for_tree(base.size());
            for (int i = 0; i < (int)base.size(); i++)
                base_for_tree[i] = TreeNode(base[i]);
            tree = SegmentTree<TreeNode, UpdInfo>(base_for_tree);
        }

        int get(int l, int r) {
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
                    cnt = 0;
                    left_value = right_value = T();
                }

                explicit TreeNode(const T &val) {
                    cnt = 1;
                    left_value = right_value = val;
                }

                int getVal() const {
                    return cnt;
                }

                void merge(const TreeNode &left, const TreeNode &right) {
                    if (left.cnt == 0) {
                        cnt = right.cnt;
                        left_value = right.left_value;
                        right_value = right.right_value;
                        return;
                    }
                    if (right.cnt == 0) {
                        cnt = left.cnt;
                        left_value = left.left_value;
                        right_value = left.right_value;
                        return;
                    }
                    left_value = left.left_value;
                    right_value = right.right_value;
                    cnt = left.cnt + right.cnt;
                    if (!cmp(left.right_value, right.left_value) &&
                            !cmp(right.left_value, left.right_value))
                        cnt--;
                }

                void addUpdate(const UpdInfo &upd, int l, int r){
                    if (upd.is_assigned) {
                        cnt = 1;
                        left_value = right_value = 
                            sum(upd.assigned, upd.added);
                    } else {
                        left_value = sum(left_value, upd.added);
                        right_value = sum(right_value, upd.added);
                    }
                }

                int cnt;
                T left_value;
                T right_value;
                Summator sum;
                Comparator cmp;
        };

        SegmentTree<TreeNode, UpdInfo> tree;
};

#endif
