#ifndef TESTING_UTILITIES_H

#define TESTING_UTILITIES_H

#include <vector>
#include <cstdlib>
#include <algorithm>

namespace testing_utilities {

    struct Min {
        int operator() (std::vector<int> &slow_tree, int l, int r) {
            int mn = slow_tree[l];
            for (int i = l; i <= r; i++)
                mn = std::min(mn, slow_tree[i]);
            return mn;
        }
    };

    struct Max {
        int operator() (std::vector<int> &slow_tree, int l, int r) {
            int mx = slow_tree[l];
            for (int i = l; i <= r; i++)
                mx = std::max(mx, slow_tree[i]);
            return mx;
        }
    };

    struct Sum {
        int operator() (std::vector<int> &slow_tree, int l, int r) {
            int sum = 0;
            for (int i = l; i <= r; i++)
                sum += slow_tree[i];
            return sum;
        }
    };

    template <class Tree, class TreeNode>
        void initIntTrees(std::vector<int> &slow_tree, Tree &tree) {
            int sz = rand() % 10000;
            slow_tree.resize(sz);
            std::vector<TreeNode> init(sz);
            for (int i = 0; i < sz; i++){
                slow_tree[i] = rand() % 100000;
                init[i] = TreeNode(slow_tree[i]);
            }
            tree = Tree(init);
    }

    void add(std::vector<int> &slow_tree, int l, int r, int val) {
        for (int i = l; i <= r; i++)
            slow_tree[i] += val;
    }

    void put(std::vector<int> &slow_tree, int l, int r, int val) {
        for (int i = l; i <= r; i++)
            slow_tree[i] = val;
    }

    template<class Tree, class TreeNode, class UpdInfo, class Get>
        void testAddTree() {
            Get get;
            std::vector<int> slow_tree;
            Tree tree;
            initIntTrees<Tree, TreeNode>(slow_tree, tree);
            int sz = slow_tree.size();

            int cnt_queries = rand() % 100000;
            for (int i = 0; i < cnt_queries; i++) {
                int query_type = rand() % 2;
                int l = rand() % sz, r = rand() % sz;
                if (l > r)
                    std::swap(l ,r);
                if (query_type == 0) {
                    TreeNode tree_res = tree.get(l, r);
                    int res = get(slow_tree, l, r);
                    ASSERT_EQ(tree_res.getVal(), res);
                } else {
                    int val = rand() % 100000;
                    UpdInfo upd(val);
                    tree.update(l, r, upd);
                    add(slow_tree, l, r, val);
                }
            }
        }

    template<class Tree, class TreeNode, class UpdInfo, class Get>
        void testAssignmentTree() {
            Get get;
            std::vector<int> slow_tree;
            Tree tree;
            initIntTrees<Tree, TreeNode>(slow_tree, tree);
            int sz = slow_tree.size();

            int cnt_queries = rand() % 100000;
            for (int i = 0; i < cnt_queries; i++) {
                int query_type = rand() % 2;
                int l = rand() % sz, r = rand() % sz;
                if (l > r)
                    std::swap(l ,r);
                if (query_type == 0) {
                    TreeNode tree_res = tree.get(l, r);
                    int res = get(slow_tree, l, r);
                    ASSERT_EQ(tree_res.getVal(), res);
                } else {
                    int val = rand() % 100000;
                    UpdInfo upd(val);
                    tree.update(l, r, upd);
                    put(slow_tree, l, r, val);
                }
            }
        }

    template<class Tree, class TreeNode, class UpdInfo, class Get>
        void testAssignmentAddTree() {
            Get get;
            std::vector<int> slow_tree;
            Tree tree;
            initIntTrees<Tree, TreeNode>(slow_tree, tree);
            int sz = slow_tree.size();

            int cnt_queries = rand() % 100000;
            for (int i = 0; i < cnt_queries; i++) {
                int query_type = rand() % 3;
                int l = rand() % sz, r = rand() % sz;
                if (l > r)
                    std::swap(l, r);
                if (query_type == 0) {
                    TreeNode tree_res = tree.get(l, r);
                    int res = get(slow_tree, l, r);
                    ASSERT_EQ(tree_res.getVal(), res);
                } else if (query_type == 1) {
                    int val = rand() % 100000;
                    UpdInfo upd(val, 0, true);
                    tree.update(l, r, upd);
                    put(slow_tree, l, r, val);
                } else {
                    int val = rand() % 100000;
                    UpdInfo upd(0, val, false);
                    tree.update(l, r, upd);
                    add(slow_tree, l, r, val);
                }
            }
        }
}

#endif
