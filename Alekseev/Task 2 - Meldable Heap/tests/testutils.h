#ifndef TESTUTILS_H
#define TESTUTILS_H

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <set>

#include "gtest/gtest.h"

#include "leftistheap.h"

template<class T>
class Asserted{};

//template<class T>
//TestAccess<T> acc(T* ptr)
//{
//    return TestAccess<T>(ptr);
//}

//template<class T>
//TestAccess<T> acc(T& ref)
//{
//    return TestAccess<T>(&ref);
//}

namespace std
{
template<typename A, typename B>
ostream& operator << (ostream& out, const std::pair<A, B>& pair)
{
    return out << "<" << pair.first << ", " << pair.second << ">";
}
}

template<typename KeyT, class CompareT>
class Asserted<LeftistHeap<KeyT, CompareT>>
{
public:
    typedef KeyT     Key;
    typedef CompareT Compare;

    typedef LeftistHeap<Key, Compare> Heap;
    typedef typename Heap::Node       Node;
    typedef typename Heap::NodePtr    NodePtr;
    typedef typename Heap::Height     Height;
    typedef typename Heap::Index      Index;

    Asserted(const Compare& compare = Compare()): heap(compare) {}

    bool isEmpty()
    {
        return heap.isEmpty();
    }

    Compare * comparator()
    {
        return heap.comparator();
    }

    template<typename... Args>
    Index emplace(const Args&... args)
    {
        Index ret = heap.emplace(args...);
        controlSet.emplace(args...);
        assertInvariants();
        assertKeysSet(controlSet);
        return ret;
    }
    Index emplace(const Key key)
    {
        Index ret = heap.push(key);
        controlSet.push(key);
        assertInvariants();
        assertKeysSet(controlSet);
        return ret;
    }
    Index set(Index at, const Key &key)
    {
        controlSet.erase(controlSet.find(*at));
        Index ret = heap.set(at, key);
        controlSet.insert(key);
        assertInvariants();
        assertKeysSet(controlSet);
        return ret;
    }
    Key takeTop()
    {
        Key ret = std::move(heap.takeTop());
        controlSet.erase(ret);
        assertInvariants();
        assertKeysSet(controlSet);
        return ret;
    }

    void assertKeysSet(std::multiset<Key, Compare> keys)
    {
        assertKeysSet(heap.root, keys);
        ASSERT_TRUE(keys.empty());
    }

    void assertInvariants()
    {
        assertInvariants(heap.root);
    }

    std::string printableTraverse()
    {
        return printableTraverse(heap.root, 0);
    }

private:
    void assertKeysSet(NodePtr root, std::multiset<Key, Compare> &keys)
    {
        if (!root)
            return;
        assertKeysSet(root->leftSon, keys);
        auto it = keys.find(root->key);
        ASSERT_NE(keys.end(), it) << error("unexpected key", root);
        keys.erase(it);
        assertKeysSet(root->rightSon, keys);
    }
    void assertInvariants(NodePtr root)
    {
        if (!root)
            return;
        assertInvariants(root->leftSon);
        assertInvariants(root->rightSon);

        if (root->leftSon)
        {
            ASSERT_EQ(root, root->leftSon->parent) << error("bad parent", root->leftSon);
            ASSERT_TRUE(heap.compare(root->key, root->leftSon->key)) << error("less than parent", root->leftSon);
        }
        if (root->rightSon)
        {
            ASSERT_EQ(root, root->rightSon->parent) << error("bad parent", root->rightSon);
            ASSERT_TRUE(heap.compare(root->key, root->rightSon->key)) << error("less than parent", root->rightSon);
        }
        Height leftH = root->leftSon ? root->leftSon->minHeight : 0;
        Height rightH = root->rightSon ? root->rightSon->minHeight : 0;

        ASSERT_LE(rightH, leftH);
        ASSERT_EQ(rightH + 1, root->minHeight);
    }

    std::string repr(NodePtr node, std::size_t depth)
    {
        std::ostringstream ret;
        for (size_t i = 0; i < depth; ++i)
            ret << (node == badPlace ? "---" : "   ");
        ret << node;
        if (!node)
        {
            ret << "\n";
            return ret.str();
        }
        ret << " key: " << node->key;
        ret << " minHeight: " << node->minHeight;
        ret << " parent: " << node->parent;

        ret << std::endl;
        return ret.str();
    }
    std::string printableTraverse(NodePtr root, std::size_t depth)
    {
        if (!root)
            return repr(root, depth);

        std::string ret;
        ret += printableTraverse(root->leftSon, depth+1);
        ret += repr(root, depth);
        ret += printableTraverse(root->rightSon, depth+1);
        return ret;
    }
    std::string error(const std::string &msg, NodePtr badPlace)
    {
        std::string ret;
        ret += msg + "\n" + repr(badPlace, 0) + "\ntraverse:\n\n";
        this->badPlace = badPlace;
        ret += printableTraverse(heap.root, 0);
        badPlace = nullptr;
        return ret;
    }

    Heap heap;
    NodePtr badPlace;
    std::multiset<Key, Compare> controlSet;
};

#endif // TESTUTILS_H
