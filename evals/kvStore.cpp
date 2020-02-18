#include <bits/stdc++.h>
#ifdef REC
#include "../src/include/btree-rec.hpp"
#else
#include "../src/include/btree.hpp"
#endif

using namespace std;

class kvstore {
   public:
    BTree t;

    kvstore() : t(5) {
    }

    bool get(string key) {
        searchStruct* res = t.search(key);
        return res && res->exists;
    }

    bool put(string key, string value) {
        bool exists = t.search(key) != 0;
        t.insert((keyVal){key, value});

        return exists;
    }

    bool del(string key) {
        bool exists = t.search(key) != 0;
        t.remove(key);
        exists = t.search(key) != 0;
        return exists;
    }

    pair<string, string> get(int N) {
        keyVal kv;
        t.get(N + 1, kv);
        pair<string, string> temp = make_pair(kv.key, kv.value);
        // cout<<
        return temp;
    }

    bool del(int N) {
        keyVal kv;
        t.get(N + 1, kv);
        t.remove(kv.key);
        // Your Code Here
        return true;
    }
};
