#include <bits/stdc++.h>
#include "../src/trie_comp.cpp"
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
         * insert doesn't replace old values
         */
        return true;
    }

    bool del(Slice &key) {
        return db.del(key);
    }

    bool get(int N, Slice &key, Slice &value) {
        db.get_val_N(N, key, value);
        // TODO: implement this
        return true;
    }

    bool del(int N) {
        // TODO: implement this
        return true;
    }
};