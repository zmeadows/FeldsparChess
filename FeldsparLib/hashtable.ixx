export module unstd.hashtable;

import unstd.core;

template <typename Key, typename Value, Key EMPTY_SENTINEL, Key TOMBSTONE_SENTINEL>
class HashTable {
    struct KVPair {
        Key key;
        Value val;
    };

    // alignas(alignof(KVPair)) U8 m_data[sizeof(KVPair) * STACK_SIZE];
    KVPair* m_ptr;

    U64 m_count;
    U64 m_capacity;
    U64 m_longest_probe;
    F64 max_load_factor;
    bool m_on_stack;

public:
    HashTable& operator=(const HashTable&) = delete;
    HashTable& operator=(HashTable&&) = delete;
    HashTable(HashTable&&) = delete;
    HashTable(const HashTable&) = delete;
};
