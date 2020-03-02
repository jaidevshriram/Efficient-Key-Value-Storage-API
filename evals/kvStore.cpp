#include <bits/stdc++.h>
#include "../src/trie_comp.cpp"
using namespace std;

class kvstore {
   public:
    Trie db;

    bool get(string key) {
        Slice a(key), b;
        bool is_valid = db.get_val(a, b);
        return is_valid;
    }

    bool put(string key, string value) {
        Slice a(key), b(value);
        return db.insert(a, b);
        // return true;
    }

    bool del(string key) {
        Slice a(key);
        bool is_valid = true;  // db.del(a);
        return is_valid;
    }

    pair<string, string> get(int N) {
        // Your Code Here
        pair<string, string> temp = make_pair("key", "value");
        return temp;
    }

    bool del(int N) {
        // Your Code Here
        return true;
    }
};
