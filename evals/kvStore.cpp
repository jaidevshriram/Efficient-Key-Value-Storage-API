#define NTRIES 1

#include <bits/stdc++.h>
#include "trie_comp.cpp"
#include <pthread.h>

using namespace std;

class kvStore {
    uint64_t maxSize;
    pthread_rwlock_t lock;
    Trie db[NTRIES];
    // uint children[NTRIES];
    uint bucketSize;

   public:
    kvStore(uint64_t maxSize) {
        this->maxSize = maxSize;
        pthread_rwlock_init(&lock, NULL);
        bucketSize = (52 / NTRIES) + 1;
    }

    bool get(Slice &key, Slice &value) {
        // cout << "GET\n";
        pthread_rwlock_wrlock(&lock);
        bool ret = db[getIndex(key.data[0])].get_val(key, value);
        pthread_rwlock_unlock(&lock);

        return ret;
    }

    bool put(Slice &key, Slice &value) {
        // cout << "PUT\n";
        uint index = getIndex(key.data[0]);

        pthread_rwlock_wrlock(&lock);
        bool ret = db[index].insert(key, value);
        pthread_rwlock_unlock(&lock);

        return ret;
    }

    bool del(Slice &key) {
        // cout << "DEL\n";
        Slice value;

        if (!key.data)
            return false;

        uint index = getIndex(key.data[0]);

        pthread_rwlock_wrlock(&lock);
        bool exists = db[index].get_val(key, value);
        // pthread_rwlock_unlock(&lock);

        if (!exists)
            return false;

        // pthread_rwlock_wrlock(&lock);
        bool ret = db[index].del(key);
        pthread_rwlock_unlock(&lock);

        return ret;
    }

    bool get(int N, Slice &key, Slice &value) {
        // cout << "GET N\n";
        pthread_rwlock_wrlock(&lock);
        int index = 0;
        int ncp = N;

        for (int i = 0; i < NTRIES; i++) {
            int ch = db[i].root->children;
            if (ch > ncp)
                break;
            ncp -= ch;
            index++;
        }
        // cout << index << " " << ncp << '\n';
        db[index].get_val_N(ncp, key, value);
        pthread_rwlock_unlock(&lock);

        return true;
    }

    bool del(int N) {
        // cout << "DEL N\n";
        Slice key, value;

        // string pr = "===" + to_string(N) + " [";

        // for (int i = 0; i < NTRIES; i++)
        //     pr += to_string(children[i]) + ", ";

        pthread_rwlock_wrlock(&lock);
        int index = 0;
        int ncp = N;

        for (int i = 0; i < NTRIES; i++) {
            int ch = db[i].root->children;
            if (ch > ncp)
                break;
            ncp -= ch;
            index++;
        }
        // pr += "] " + to_string(index) + " " + to_string(N) + "===";

        // cout << pr << endl;

        // cout << N << " " << index << " ++++ " << ncp << '\n';
        bool exists = db[index].get_val_N(ncp, key, value);
        // pthread_rwlock_unlock(&lock);

        if (!exists)
            return false;

        index = getIndex(key.data[0]);

        // pthread_rwlock_wrlock(&lock);
        bool ret = db[index].del(key);
        pthread_rwlock_unlock(&lock);

        return ret;
    }

    uint8_t getIndex(char f) {
        uint8_t x = (f > 90) ? f - 97 + 26 : f - 65;
        return x / bucketSize;
    }
};
