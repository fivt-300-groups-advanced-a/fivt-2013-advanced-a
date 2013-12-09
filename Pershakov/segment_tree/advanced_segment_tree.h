#ifndef ADVANCED_SEGMENT_TREE_H

#define ADVANCED_SEGMENT_TREE_H

#include <vector>

template <class Type, class UpdInfo> class SegmentTree {
    public:
        explicit SegmentTree(vector<Type> &base){
            sz = base.size();
            int power = getPower(sz) * 2;
            tree.resize(power);
            update_tree.resize(power, NULL);
            build(1, 0, sz - 1, base);
        }

        explicit SegmentTree(int new_size){
            sz = new_size;
            int power = getPower(sz) * 2;
            tree.resize(power);
            update_tree.resize(power, NULL);
        }

        Type get(int l, int r){
            return get(1, 0, sz - 1, l, r);
        }
        
        void update(int l, int r, UpdInfo &upd){
            update(1, 0, sz - 1, l, r, upd);
        }

    private:
        std::vector<Type> tree;
        std::vector<UpdInfo*> update_tree;
        int sz;

        int getPower(int sz){
            int power = 1;
            while (power < sz)
                power *= 2;
            return power;
        }

        void build(int v, int vl, int vr, vector<Type> &base){
            if (vl == vr){
                tree[v] = base[vl];
                return;
            }
            int d = (vl + vr) / 2;
            build(v * 2, vl, d);
            build(v * 2 + 1, d + 1, vr);
            tree[v].merge(tree[v * 2], tree[v * 2 + 1]);
        }

        void push(int v, int vl, int vr){
            int mid = (vl + vr ) / 2;
            if (v * 2 < sz){
                update_tree[v * 2]->push(*update_tree[v], vl, mid);
                tree[v * 2].addUpdate(update_tree[v], vl, mid);
            }
            if (v * 2 + 1 < sz){
                update_tree[v * 2 + 1]->push(*update_tree[v], mid + 1, vr);
                tree[v * 2 + 1].addUpdate(update_tree[v], mid + 1, vr);
            }
        }

        Type get(int v, int vl, int vr, int l, int r){
            if (l > r)
                return T();
            if (update_tree[v])
                push(v, vl, vr);
            if (vl == l && vr == r)
                return tree[v];
            int mid = (vl + vr) / 2;
            get(v * 2, vl, mid, l, min(r, mid));
            get(v * 2 + 1, mid + 1, vr, max(l, mid + 1), r);
            tree[v].merge(tree[v * 2], tree[v * 2 + 1]);
        }

        void update(int v, int vl, int vr, int l, int r, UpdInfo &upd){
            if (l > r)
                return;
            if (update_tree[v])
                push(v, vl, vr);
            if (vl == l && vr == r){
                update_tree[v] = upd;
                tree[v].addUpdate(upd, vl, vr);
                return;
            }
            int mid = (vl + vr) / 2;
            update(v * 2, vl, mid, l, min(r, mid), upd);
            update(v * 2 + 1, mid + 1, vr, max(mid + 1, l), r, upd);
            tree[v].merge(tree[v * 2], tree[v * 2 + 1]);
        }
};

#endif
