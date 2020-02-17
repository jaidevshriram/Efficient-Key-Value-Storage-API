#include <stdint.h>
#include <string>
#include <bits/stdc++.h>

struct Slice {
   public:
    uint8_t size;
    char* data;

    Slice(std::string a) {
        size = a.length();
        data = (char*)malloc(a.length());
        memcpy(data, a.data(), a.length());
    }
};

// class kvStore {
//     kvStore(uint64_t max_entries);
//     bool get(Slice& key, Slice& value);
//     bool put(Slice& key, Slice& value);
//     bool del(Slice& key);
//     bool get(int N, Slice& key, Slice& value);
//     bool del(int N);
// };
