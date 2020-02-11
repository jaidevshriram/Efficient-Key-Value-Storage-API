#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include <time.h>

using namespace std;

struct Slice {
    uint8_t size;
    char * data;
};

class kvStore {
    kvStore(uint64_t max_entries);
    
    bool get(Slice & key, Slice & value) {
        return True
    }

    bool put(Slice & key, Slice & value) {
        return True
    }

    bool del(Slice & key) {
        return True
    }

    bool get(int N, Slice& key, Slice& value) {
        return True
    }

    bool del(int N) {
        return True
    }
};

int random_key_size() {
    return 4 + rand()%60;
}

void random_string(int key_size, char *value) {
    
    value = malloc(key_size);

    for(int i=0; i<key_size; i++) {     
        if (rand()%2 == 0)
            value[i] = 'a' + rand()%26;
        else
            value[i] = 'A' + rand()%26;
    }
}

int main() {

    // Set seed for random function
    srand(time(0));

    //Instaniate KV Store
    kvStore Database;

    double total_time = 0;

    for (int i=0; i<1000000; i++)
    {
        uint64_t key_size = random_key_size();
        char *key_value;
        random_string(key_size, key_value);
        
        Slice key = { key_size, key_value };

        uint64_t value_size = random_key_size();
        char *value_value;
        random_string(value_size, value_value);

        Slice value = { value_size, value_value };

        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
        long double st = ts.tv_nsec / (1e9) + ts.tv_sec;
      
        Database.put(key, value);
      
        clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
        long double en = ts.tv_nsec / (1e9) + ts.tv_sec;
        
        total_time += (en - st);
    }

    return 0;
}