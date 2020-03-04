#include <bits/stdc++.h>
#include "trie_comp.cpp"
using namespace std;

class kvStore {
    uint64_t maxSize;

   public:
    kvStore(uint64_t maxSize) {
        this->maxSize = maxSize;
    }

    Trie db;

    bool get(Slice &key, Slice &value) {
        return db.get_val(key, value);
    }

    bool put(Slice &key, Slice &value) {
        db.insert(key, value);

        /*
         * TODO
         * UPDATE THIS
         */
        return true;
    }

    bool del(Slice &key) {
        return db.del(key);
    }

    bool get(int N, Slice &key, Slice &value) {
        db.get_val_N(N, key, value);
        return true;
    }

    bool del(int N) {
        Slice key, value;
        db.get_val_N(N, key, value);
        db.del(key);
        return true;
    }
};
