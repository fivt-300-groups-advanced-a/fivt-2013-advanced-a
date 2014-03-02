#ifndef LEFTISTHEAP_H
#define LEFTISTHEAP_H

#include <functional>

template<class T> class Asserted;

template<typename KeyT, class CompareT = std::less<KeyT> >
class LeftistHeap
{
public:
    typedef KeyT Key;
    typedef CompareT Compare;

    class Index
    {
    public:
        Index(): _node(nullptr) {}

        const Key& operator*() const
        {
            return _node->key;
        }
        const Key& operator->() const
        {
            return _node->key;
        }

    private:
        friend class LeftistHeap<Key, Compare>;
        typedef typename LeftistHeap<Key, Compare>::NodePtr NodePtr;

        explicit Index(NodePtr node): _node(node) {}
        NodePtr _node;
    };

    LeftistHeap():                                heapSize(0), root(nullptr), compare(Compare()) {}
    explicit LeftistHeap(const Compare &compare): heapSize(0), root(nullptr), compare(compare) {}
    explicit LeftistHeap(Compare &&compare):      heapSize(0), root(nullptr), compare(std::move(compare)) {}

    LeftistHeap(LeftistHeap<Key, Compare>&& that): heapSize(0), root(nullptr), compare(std::move(that.compare))
    {
        std::swap(heapSize, that.heapSize);
        std::swap(root, that.root);
    }

    ~LeftistHeap()
    {
        purgeTree(root);
    }

    bool isEmpty() const
    {
        return !root;
    }

    const Key &top() const
    {
        return root->key;
    }
    Index topIndex() const
    {
        return Index(root);
    }

    Index push(const Key &key)
    {
        NodePtr node = new Node(key);
        merge(root, root, node, nullptr);
        ++heapSize;
        return Index(node);
    }

    template<typename... Args>
    Index emplace(const Args&... args)
    {
        NodePtr node = new Node(Key(args...));
        merge(root, root, node, nullptr);
        ++heapSize;
        return Index(node);
    }

    Index set(Index at, const Key &key)
    {
        unlink(at._node);
        at._node->key = key;
        merge(root, root, at._node, nullptr);
        return at;
    }

    void pop()
    {
        remove(root);
    }
    void popAt(Index at)
    {
        remove(at._node);
    }

    Key takeTop()
    {
        Key ret = std::move(root->key);
        remove(root);
        return ret;
    }
    Key takeAt(Index at)
    {
        Key ret = std::move(at._node->key);
        remove(at._node);
        return ret;
    }

    void swap(LeftistHeap<Key, Compare> &that)
    {
        std::swap(root, that.root);
        std::swap(compare, that.compare);
    }

    void absorb(LeftistHeap<Key, Compare> &that)
    {
        merge(root, root, that.root, nullptr);
        that.heapSize = 0;
        that.root = nullptr;
    }

    Compare * comparator()
    {
        return &compare;
    }

    std::size_t size() const
    {
        return heapSize;
    }

private:
    typedef std::size_t Height;

    friend class LeftistHeap<Key, Compare>::Index;
    friend class Asserted<LeftistHeap<Key, Compare>>;

    struct Node
    {
        explicit Node(const Key &key): parent(nullptr), leftSon(nullptr), rightSon(nullptr), key(key), minHeight(1) {}
        explicit Node(Key &&key): parent(nullptr), leftSon(nullptr), rightSon(nullptr), key(std::move(key)), minHeight(1) {}

        Node *parent, *leftSon, *rightSon;
        Key key;
        Height minHeight;
    };
    typedef Node * NodePtr;

    Height minHeight(NodePtr node)
    {
        return node ? node->minHeight : 0;
    }
    bool isRightSon(NodePtr node)
    {
        return node && node->parent && node->parent->rightSon == node;
    }

    void updateMinHeight(NodePtr node)
    {
        if (!node)
            return;
        if (minHeight(node->leftSon) < minHeight(node->rightSon))
            std::swap(node->leftSon, node->rightSon);
        node->minHeight = minHeight(node->rightSon) + 1;
    }

    void updateUp(NodePtr node)
    {
        while (node)
        {
            Height before = node->minHeight;
            updateMinHeight(node);
            if (before == node->minHeight)
                return;
            node = node->parent;
        }
    }

    void merge(NodePtr &dest, NodePtr a, NodePtr b, NodePtr parent)
    {
        if (!a || !b)
            dest = a ? a : b;
        else
        {
            if (compare(b->key, a->key))
                std::swap(a, b);
            merge(a->rightSon, a->rightSon, b, a);
            dest = a;
        }
        if (dest)
            dest->parent = parent;
        updateMinHeight(dest);
    }

    void remove(NodePtr node)
    {
        unlink(node);
        delete node;
        --heapSize;
    }

    void unlink(NodePtr node)
    {
        assert(node);

        NodePtr& parentLink = node->parent ? (isRightSon(node) ? node->parent->rightSon : node->parent->leftSon) : root;

        merge(parentLink, node->leftSon, node->rightSon, node->parent);
        node->leftSon = node->rightSon = nullptr;
        if (node->parent)
            updateUp(node->parent);
    }

    void purgeTree(NodePtr root)
    {
        if (!root)
            return;
        purgeTree(root->leftSon);
        purgeTree(root->rightSon);
        delete root;
        --heapSize;
    }

    std::size_t heapSize;
    NodePtr root;
    Compare compare;
};

#endif // LEFTISTHEAP_H
