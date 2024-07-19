#include <iostream>
#include <string>
#include "BST.h"  

int main() {
    // 创建一个 BST 对象
    BST<int, std::string> bst;

    // 插入一些键值对
    bst.put(Key<int>(1), "Value 1");
    bst.put(Key<int>(2), "Value 2");
    bst.put(Key<int>(3), "Value 3");

    // 打印 BST 中的所有键和值
    std::cout << "All keys and values in the BST:" << std::endl;
    for (const auto& key : bst.keys(Key<int>(1), Key<int>(3))) {
        std::string value = bst.get(key);
        std::cout << "Key: " << key.toString() << ", Value: " << value << std::endl;
    }

    // 获取并打印最大键和最小键
    Key<int> maxKey = bst.max();
    Key<int> minKey = bst.min();
    std::cout << "Max key: " << maxKey.toString() << std::endl;
    std::cout << "Min key: " << minKey.toString() << std::endl;

    // 获取并打印选择的键
    Key<int> selectedKey = bst.select(1);
    std::cout << "Key at index 1: " << selectedKey.toString() << std::endl;

    // 打印以特定范围的键
    auto rangeKeys = bst.keys(Key<int>(1), Key<int>(3));
    std::cout << "Keys in range [1, 3]:" << std::endl;
    for (const auto& key : rangeKeys) {
        std::cout << "Key: " << key.toString() << std::endl;
    }

    // 删除最小键并打印剩余的键
    bst.deleteMin();
    std::cout << "Keys after deleting the minimum:" << std::endl;
    for (const auto& key : bst.keys(Key<int>(1), Key<int>(3))) {
        std::cout << "Key: " << key.toString() << std::endl;
    }

    // 删除特定键并打印剩余的键
    bst.deleteForKey(Key<int>(2));
    std::cout << "Keys after deleting key 2:" << std::endl;
    for (const auto& key : bst.keys(Key<int>(1), Key<int>(3))) {
        std::cout << "Key: " << key.toString() << std::endl;
    }

    return 0;
}