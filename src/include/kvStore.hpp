#include <stdint.h>
#include "./btree.hpp"
#include <string.h>

struct Slice {
    uint8_t size;
    char* data;
};

string sliceToStr(Slice& a) {
    string ret = "";

    for (int i = 0; i < a.size; i++)
        ret += a.data[i];

    return ret;
}

void strToSlice(string l, Slice& a) {
    a.size = l.length();
    a.data = (char*)malloc(a.size);
    strncpy(a.data, l.c_str(), a.size);
}

class kvStore {
    BTree t;

    kvStore(uint64_t max_entries) : t(5){};

    bool get(Slice& key, Slice& value) {
        string _key = sliceToStr(key);
        searchStruct* res = t.search(_key);

        if (!res || !res->exists)
            return false;

        string val = res->b->keys[res->i].value;
        strToSlice(val, value);

        return true;
    }

    bool put(Slice& key, Slice& value) {
        string _key = sliceToStr(key);
        string _val = sliceToStr(value);

        searchStruct* res = t.search(_key);
        bool exists = res && res->exists;

        t.insert((keyVal){_key, _val});

        return exists;
    }

    bool del(Slice& key) {
        string _key = sliceToStr(key);
        t.remove(_key);

        return true;
    }

    bool get(int N, Slice& key, Slice& value) {
        string _key = sliceToStr(key), _val = sliceToStr(value);
        keyVal kv;

        t.get(N + 1, kv);

        strToSlice(kv.key, key);
        strToSlice(kv.value, value);

        return true;
    }

    bool del(int N) {
        keyVal kv;
        t.get(N + 1, kv);

        t.remove(kv.key);

        return true;
    }
};
