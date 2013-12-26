#ifndef TESTING_UTILITIES_H

#define TESTING_UTILITIES_H

#include <vector>
#include <cstdlib>
#include <algorithm>

#include "../min_max_sum_tree.h"

namespace testing_utilities {

    struct GetMinMaxSum {
        MinMaxSum<int> operator() (std::vector<int> &slow_tree, int l, int r) {
            int sum = 0;
            int min = slow_tree[l];
            int max = slow_tree[r];
            for (int i = l; i <= r; i++) {
                sum += slow_tree[i];
                min = std::min(min, slow_tree[i]);
                max = std::max(max, slow_tree[i]);
            }
            return MinMaxSum<int>(min, max, sum);
        }
    };

    struct GetCntEqualitySegments {
        int operator() (std::vector<int> &slow_tree, int l, int r) {
            int cnt = 1;
            for (int i = l + 1; i <= r; i++)
                if (slow_tree[i] != slow_tree[i - 1])
                    cnt++;
            return cnt;
        }
    };

    /*
     * return random number from [-100000, 100000]
     */
    int randomInt(){
        return rand() % 200001 - 100000;
    }

    template <class Tree>
        void initIntTrees(std::vector<int> &slow_tree, Tree &tree) {
            int sz = rand() % 10000;
            slow_tree.resize(sz);
            for (int i = 0; i < sz; i++)
                slow_tree[i] = randomInt();
            tree = Tree(slow_tree);
    }

    void add(std::vector<int> &slow_tree, int l, int r, int val) {
        for (int i = l; i <= r; i++)
            slow_tree[i] += val;
    }

    void assign(std::vector<int> &slow_tree, int l, int r, int val) {
        for (int i = l; i <= r; i++)
            slow_tree[i] = val;
    }

    template<class Tree, class Get>
        void testAddTree() {
            Get get;
            std::vector<int> slow_tree;
            Tree tree;
            initIntTrees<Tree>(slow_tree, tree);
            int sz = slow_tree.size();

            int cnt_queries = rand() % 100000;
            for (int i = 0; i < cnt_queries; i++) {
                int query_type = rand() % 2;
                int l = rand() % sz, r = rand() % sz;
                if (l > r)
                    std::swap(l ,r);
                if (query_type == 0) {
                    ASSERT_EQ(tree.get(l, r), get(slow_tree, l, r));
                } else {
                    int val = randomInt();
                    tree.add(l, r, val);
                    add(slow_tree, l, r, val);
                }
            }
        }

    template<class Tree, class Get>
        void testAssignmentTree() {
            Get get;
            std::vector<int> slow_tree;
            Tree tree;
            initIntTrees<Tree>(slow_tree, tree);
            int sz = slow_tree.size();

            int cnt_queries = rand() % 100000;
            for (int i = 0; i < cnt_queries; i++) {
                int query_type = rand() % 2;
                int l = rand() % sz, r = rand() % sz;
                if (l > r)
                    std::swap(l ,r);
                if (query_type == 0) {
                    ASSERT_EQ(tree.get(l, r), get(slow_tree, l, r));
                } else {
                    int val = randomInt();
                    tree.assign(l, r, val);
                    assign(slow_tree, l, r, val);
                }
            }
        }

    template<class Tree, class Get>
        void testAssignmentAddTree() {
            Get get;
            std::vector<int> slow_tree;
            Tree tree;
            initIntTrees<Tree>(slow_tree, tree);
            int sz = slow_tree.size();

            int cnt_queries = rand() % 100000;
            for (int i = 0; i < cnt_queries; i++) {
                int query_type = rand() % 3;
                int l = rand() % sz, r = rand() % sz;
                if (l > r)
                    std::swap(l, r);
                if (query_type == 0) {
                    ASSERT_EQ(tree.get(l, r), get(slow_tree, l, r));
                } else if (query_type == 1) {
                    int val = randomInt();
                    tree.assign(l, r, val);
                    assign(slow_tree, l, r, val);
                } else {
                    int val = randomInt();
                    tree.add(l, r, val);
                    add(slow_tree, l, r, val);
                }
            }
        }
}

#endif
