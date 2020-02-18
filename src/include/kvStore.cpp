#include<stdint.h>
#include<string.h>
#include <iostream>
#include<bits/stdc++.h>

using namespace std;

struct Slice {
    uint8_t size;
    char* data;

    Slice(string a)
    {
        size = a.length();
        printf("LEngth of %s is %d\n", a, size);
        data = malloc(a.length());
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
