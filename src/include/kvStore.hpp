#include <stdint.h>
#include<bits/stdc++.h>

struct Slice {

public:
    uint8_t size;
    char* data;

    Slice(string a)
    {
        size = a.length();
        data = malloc(a.length());
        memcpy(data, a.data(), a.length());
    }
};

class kvStore {
    kvStore(uint64_t max_entries);
    bool get(Slice& key, Slice& value);
    bool put(Slice& key, Slice& value);
    bool del(Slice& key);
    bool get(int N, Slice& key, Slice& value);
    bool del(int N);
};
