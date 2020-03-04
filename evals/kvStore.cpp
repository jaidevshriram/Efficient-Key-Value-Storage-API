#include <bits/stdc++.h>
#include "trie_comp.cpp"
#include <pthread.h>

using namespace std;

class kvStore {
    uint64_t maxSize;
    pthread_rwlock_t lock;


   public:
    kvStore(uint64_t maxSize) {
        this->maxSize = maxSize;
        pthread_rwlock_init(&lock, NULL);
    }

    Trie db;

    bool get(Slice &key, Slice &value) {
        pthread_rwlock_rdlock(&lock);
        bool ret = db.get_val(key, value);
        pthread_rwlock_unlock(&lock);

        return ret;
    }

    bool put(Slice &key, Slice &value) {
        pthread_rwlock_wrlock(&lock);
        bool ret = db.insert(key, value);
        pthread_rwlock_unlock(&lock);

        return ret;
    }

    bool del(Slice &key) {
        Slice value;

        pthread_rwlock_rdlock(&lock);
        bool exists = db.get_val(key, value);
        pthread_rwlock_unlock(&lock);

        if(!exists) return false;

        pthread_rwlock_wrlock(&lock);
        bool ret = db.del(key);
        pthread_rwlock_unlock(&lock);

        return ret;
    }

    bool get(int N, Slice &key, Slice &value) {
        pthread_rwlock_rdlock(&lock);
        db.get_val_N(N, key, value);
        pthread_rwlock_unlock(&lock);

        return true;
    }

    bool del(int N) {
        Slice key, value;

        pthread_rwlock_rdlock(&lock);
        bool exists = db.get_val_N(N, key, value);
        pthread_rwlock_unlock(&lock);

        if(!exists) return false;

        pthread_rwlock_wrlock(&lock);
        bool ret = db.del(key);
        pthread_rwlock_unlock(&lock);

        return ret;
    }
};
