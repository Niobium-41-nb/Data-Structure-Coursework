// HashTable.h
#pragma once
#include <list>
#include <vector>
#include <functional>

template <typename K, typename V>
class HashTable {
private:
    struct Node {
        K key;
        V value;
        Node(K k, V v) : key(k), value(v) {}
    };

    int size;
    std::vector<std::list<Node>> table;

    int hashFunction(K key) {
        return std::hash<K>{}(key) % size;
    }

public:
    HashTable(int tableSize = 100) : size(tableSize), table(tableSize) {}

    void insert(K key, V value) {
        int index = hashFunction(key);
        for (auto& node : table[index]) {
            if (node.key == key) {
                node.value = value; // 更新现有键
                return;
            }
        }
        table[index].emplace_back(key, value);
    }

    bool find(K key, V& outValue) {
        int index = hashFunction(key);
        for (auto& node : table[index]) {
            if (node.key == key) {
                outValue = node.value;
                return true;
            }
        }
        return false;
    }

    bool remove(K key) {
        int index = hashFunction(key);
        auto& chain = table[index];
        for (auto it = chain.begin(); it != chain.end(); ++it) {
            if (it->key == key) {
                chain.erase(it);
                return true;
            }
        }
        return false;
    }
};