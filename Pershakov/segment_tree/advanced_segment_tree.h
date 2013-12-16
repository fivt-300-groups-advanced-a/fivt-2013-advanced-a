#ifndef ADVANCED_SEGMENT_TREE_H

#define ADVANCED_SEGMENT_TREE_H

#include <vector>
#include <algorithm>
#include <cassert>

template <class TreeNode, class UpdInfo> class SegmentTree {
    public:

        SegmentTree(){
            sz = 0;
        }

        explicit SegmentTree(std::vector<TreeNode> &base) {
            sz = base.size();
            int power = getPower(sz) * 2;
            tree.resize(power);
            update_tree.resize(power);
            changed.resize(power, false);
            build(1, 0, sz - 1, base);
        }

        explicit SegmentTree(int new_size) {
            assert(new_size >= 0);
            sz = new_size;
            int power = getPower(sz) * 2;
            tree.resize(power);
            update_tree.resize(power);
            changed.resize(power, false);
        }

        TreeNode get(int l, int r) {
            assert(0 <= l && l <= r && r < sz);
            return get(1, 0, sz - 1, l, r);
        }
        
        void update(int l, int r, UpdInfo &upd) {
            assert(0 <= l && l <= r && r < sz);
            update(1, 0, sz - 1, l, r, upd);
        }

    private:
        std::vector<TreeNode> tree;
        std::vector<UpdInfo> update_tree;
        std::vector<bool> changed;
        int sz;

        int getPower(int sz){
            int power = 1;
            while (power < sz)
                power *= 2;
            return power;
        }

        void build(int v, int vl, int vr, std::vector<TreeNode> &base) {
            if (vl == vr) {
                tree[v] = base[vl];
                return;
            }
            int mid = (vl + vr) / 2;
            build(v * 2, vl, mid, base);
            build(v * 2 + 1, mid + 1, vr, base);
            tree[v].merge(tree[v * 2], tree[v * 2 + 1]);
        }

        void push(int v, int vl, int vr) {
            int mid = (vl + vr) / 2;
            if (vl != vr) {
                update_tree[v * 2].push(update_tree[v], vl, mid);
                tree[v * 2].addUpdate(update_tree[v], vl, mid);
                changed[v * 2] = true;
                update_tree[v * 2 + 1].push(update_tree[v], mid + 1, vr);
                tree[v * 2 + 1].addUpdate(update_tree[v], mid + 1, vr);
                changed[v * 2 + 1] = true;
            }
            changed[v] = false;
            update_tree[v] = UpdInfo();
        }

        TreeNode get(int v, int vl, int vr, int l, int r) {
            if (l > r)
                return TreeNode();
            if (changed[v])
                push(v, vl, vr);
            if (vl == l && vr == r)
                return tree[v];
            int mid = (vl + vr) / 2;
            TreeNode left_res = get(v * 2, vl, mid, l, std::min(r, mid)),
                     right_res = 
                         get(v * 2 + 1, mid + 1, vr, std::max(l, mid + 1), r);
            TreeNode res;
            res.merge(left_res, right_res);
            return res;
        }

        void update(int v, int vl, int vr, int l, int r, UpdInfo &upd) {
            if (l > r)
                return;
            if (changed[v])
                push(v, vl, vr);
            if (vl == l && vr == r) {
                changed[v] = true;
                update_tree[v] = upd;
                tree[v].addUpdate(upd, vl, vr);
                return;
            }
            int mid = (vl + vr) / 2;
            update(v * 2, vl, mid, l, std::min(r, mid), upd);
            update(v * 2 + 1, mid + 1, vr, std::max(l, mid + 1), r, upd);
            tree[v].merge(tree[v * 2], tree[v * 2 + 1]);
        }
};

#endif
