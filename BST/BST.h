
#pragma once

#include <memory>
#include <sstream>
#include <stdexcept>
#include <vector>

using std::shared_ptr;
using std::make_shared;
using std::vector;

template<typename Value>
class Key {
public:
    Key(Value value) : key(value) {}

    int compareTo(const Key& other) const;

    std::string toString () const;

private:
    Value key;
};
template <typename Value>
std::string Key<Value>::toString() const
{
    std::ostringstream oss;
    oss << this->key;
    return oss.str();
}
template <typename Value>
int Key<Value>::compareTo(const Key& other) const {
    if (this->key > other.key)
        return 1;
    else if (this->key < other.key)
        return -1;
    return 0;
}

template<typename KeyType, typename Value>
class BST {
    struct Node {
        Key<KeyType> key;
        Value value;
        shared_ptr<Node> left;
        shared_ptr<Node> right;
        int N = 1;  // 以该节点为根的子树中的节点的个数

        Node(Key<KeyType> key, Value value) : key(key), value(value), left(nullptr), right(nullptr) {}
    };

public:
    int size();

    Value get(const Key<KeyType>& key);

    shared_ptr<Node> findNode(const Key<KeyType>& key);

    void put(const Key<KeyType>& key, const Value& value);

    Key<KeyType> max();

    Key<KeyType> min();

    Key<KeyType> floor(const Key<KeyType>& key);

    // Select以0开始计数
    Key<KeyType> select(int index);

    void deleteMin();

    void deleteForKey(const Key<KeyType>& key);

    vector<Key<KeyType>> keys(const Key<KeyType>& lo, const Key<KeyType>& hi);

private:
    shared_ptr<Node> root;

    int size(const shared_ptr<Node>& x);

    shared_ptr<Node> findNode(const shared_ptr<Node>& x, const Key<KeyType>& key);

    shared_ptr<Node> put(shared_ptr<Node> x, const Key<KeyType>& key, const Value& value);

    shared_ptr<Node> max(shared_ptr<Node> x);

    shared_ptr<Node> min(shared_ptr<Node> x);

    shared_ptr<Node> floor(shared_ptr<Node> x, const Key<KeyType>& key);

    Key<KeyType> select(shared_ptr<Node> x, const int index);

    // 返回删除最小树后的树
    shared_ptr<Node> deleteMin(shared_ptr<Node> x);

    // 返回删除子节点后的树
    shared_ptr<Node> deleteForKey(shared_ptr<Node> x, const Key<KeyType>& key);

    void keys(const shared_ptr<Node>& x, vector<Key<KeyType>>& vec, const Key<KeyType>& lo, const Key<KeyType>& hi);
};

template<typename KeyType, typename Value>
int BST<KeyType, Value>::size() {
    return size(root);
}

template<typename KeyType, typename Value>
Value BST<KeyType, Value>::get(const Key<KeyType>& key) {
    auto node = findNode(key);
    if (node == nullptr) {
        throw std::runtime_error("Key not found");
    }
    return node->value;
}

template <typename KeyType, typename Value>
shared_ptr<typename BST<KeyType, Value>::Node> BST<KeyType, Value>::findNode(const Key<KeyType>& key) {
    return findNode(root, key);
}

template <typename KeyType, typename Value>
void BST<KeyType, Value>::put(const Key<KeyType>& key, const Value& value) {
    if (root == nullptr) root = make_shared<Node>(key, value);
    put(root, key, value);
}

template <typename KeyType, typename Value>
Key<KeyType> BST<KeyType, Value>::max() {
    return max(root)->key;
}

template <typename KeyType, typename Value>
Key<KeyType> BST<KeyType, Value>::min() {
    return min(root)->key;
}

template <typename KeyType, typename Value>
Key<KeyType> BST<KeyType, Value>::floor(const Key<KeyType>& key) {
    return floor(root, key)->key;
}

template <typename KeyType, typename Value>
Key<KeyType> BST<KeyType, Value>::select(int index) {
    return select(root, index);
}

template <typename KeyType, typename Value>
void BST<KeyType, Value>::deleteMin() {
    // root如果左子树为空那么delete root，将root置换为右子树
    root = deleteMin(root);
}

template <typename KeyType, typename Value>
void BST<KeyType, Value>::deleteForKey(const Key<KeyType>& key) {
    root = deleteForKey(root, key);
}

template <typename KeyType, typename Value>
vector<Key<KeyType>> BST<KeyType, Value>::keys(const Key<KeyType>& lo, const Key<KeyType>& hi) {
    vector<Key<KeyType>> vec;
    keys(root, vec, lo, hi);
    return vec;
}

template<typename KeyType, typename Value>
int BST<KeyType, Value>::size(const shared_ptr<Node>& x) {
    if (x == nullptr) return 0;
    return x->N;
}

template <typename KeyType, typename Value>
shared_ptr<typename BST<KeyType, Value>::Node> BST<KeyType, Value>::findNode(const shared_ptr<Node>& x, const Key<KeyType>& key) {
    if (x == nullptr) return nullptr;
    int cmp = x->key.compareTo(key);
    if (cmp < 0) return findNode(x->right, key);
    else if (cmp > 0) return findNode(x->left, key);
    else return x;
}

template <typename KeyType, typename Value>
shared_ptr<typename BST<KeyType, Value>::Node> BST<KeyType, Value>::put(shared_ptr<Node> x, const Key<KeyType>& key, const Value& value) {
    if (x == nullptr) return make_shared<Node>(key, value);
    int cmp = x->key.compareTo(key);
    if (cmp > 0) x->left = put(x->left, key, value);
    else if (cmp < 0) x->right = put(x->right, key, value);
    else x->value = value;
    x->N = size(x->left) + size(x->right) + 1;
    return x;
}

template <typename KeyType, typename Value>
shared_ptr<typename BST<KeyType, Value>::Node> BST<KeyType, Value>::max(shared_ptr<Node> x) {
    while (true) {
        if (x->right == nullptr) return x;
        return max(x->right);
    }
}

template <typename KeyType, typename Value>
shared_ptr<typename BST<KeyType, Value>::Node> BST<KeyType, Value>::min(shared_ptr<Node> x) {
    while (true) {
        if (x->left == nullptr) return x;
        return min(x->left);
    }
}

template <typename KeyType, typename Value>
shared_ptr<typename BST<KeyType, Value>::Node> BST<KeyType, Value>::floor(shared_ptr<Node> x, const Key<KeyType>& key) {
    // floor:如果key等于自己返回自己，如果小于自己则在自己的左子树找，如果大于自己则在右子树找，
    // 这时候记录一下自己的值，因为有可能自己的右子树没有比key小的，这时候最小的就是自己
    if (x == nullptr) return nullptr;
    const int cmp = x->key.compareTo(key);
    if (cmp == 0) return x;
    else if (cmp > 0) return floor(x->left, key);
    else {
        shared_ptr<Node> t = floor(x->right, key);
        if (t == nullptr) return x;
        else return t;
    }
}

template <typename KeyType, typename Value>
Key<KeyType> BST<KeyType, Value>::select(shared_ptr<Node> x, const int index) {
    if (x == nullptr) throw std::runtime_error("The tree none this index");
    int t = size(x->left);
    if (t == index) return x->key;
    // 你要找4，以3为根即中间线，你在3右边的集合中的排名为4-3-1，select以0索引开始
    else if (t < index) return select(x->right, index - t - 1);
    else return select(x->left, index);
}

template <typename KeyType, typename Value>
shared_ptr<typename BST<KeyType, Value>::Node> BST<KeyType, Value>::deleteMin(shared_ptr<Node> x) {
    if (x->left == nullptr) {
        return x->right;
    }
    x->left = deleteMin(x->left);
    x->N = size(x->left) + size(x->right) - 1;
    return x;
}

template <typename KeyType, typename Value>
shared_ptr<typename BST<KeyType, Value>::Node> BST<KeyType, Value>::deleteForKey(shared_ptr<Node> x, const Key<KeyType>& key) {
    if (x == nullptr) return nullptr;
    const int cmp = key.compareTo(x->key);
    if (cmp < 0) x->left = deleteForKey(x->left, key);
    else if (cmp > 0) x->right = deleteForKey(x->right, key);
    else {
        if (x->left == nullptr) {
            return x->right;
        } else if (x->right == nullptr) {
            return x->left;
        }
        shared_ptr<Node> t = x;
        x = min(t->right);
        x->right = deleteMin(t->right);
        x->left = t->left;
    }
    x->N = size(x->left) + size(x->right) - 1;
    return x;
}

template <typename KeyType, typename Value>
void BST<KeyType, Value>::keys(const shared_ptr<Node>& x, vector<Key<KeyType>>& vec, const Key<KeyType>& lo, const Key<KeyType>& hi) {
    if (x == nullptr) return;
    int cmplo = lo.compareTo(x->key);
    int cmphi = hi.compareTo(x->key);
    if (cmplo < 0) keys(x->left, vec, lo, hi);
    if (cmplo <= 0 && cmphi >= 0) vec.push_back(x->key);
    if (cmphi > 0) keys(x->right, vec, lo, hi);
    
}