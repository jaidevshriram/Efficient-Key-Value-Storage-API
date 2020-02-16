#include <stdint.h>

struct Slice {
    uint8_t size;
    char* data;
};

class kvStore {
    kvStore(uint64_t max_entries);
    bool get(Slice& key, Slice& value);
    bool put(Slice& key, Slice& value);
    bool del(Slice& key);
    bool get(int N, Slice& key, Slice& value);
    bool del(int N);
};
