#include <bits/stdc++.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "kvStore.cpp"
using namespace std;

const string DESCRIPTION[] = {"GET", "PUT", "DELETE", "GET N", "DELETE N"};

string sliceToStr(Slice &a) {
    string ret = "";

    for (int i = 0; i < a.size; i++)
        ret += a.data[i];

    return ret;
}

void strToSlice(string l, Slice &a) {
    a.size = l.length();
    a.data = (char *)malloc(a.size);
    strncpy(a.data, l.c_str(), a.size);
}

string random_key(int stringLength) {
    string k = "";
    string letters = "";

    for (char i = 'a'; i <= 'z'; i++)
        letters += i;
    for (char i = 'A'; i <= 'Z'; i++)
        letters += i;

    for (int i = 0; i < stringLength; i++)
        k = k + letters[rand() % 52];

    return k;
}

string random_value(int stringLength) {
    string v = "";
    string letters = "";

    for (int i = 32; i < 127; i++)
        letters += (char)i;

    for (int i = 0; i < stringLength; i++)
        v = v + letters[rand() % letters.size()];

    return v;
}

const uint MAX_KEYS = 10000000, INSERTS = 100, NUM_OPS = 0;
const long CLOCKS_PER_SECOND = 1000000;
const uint key_size = 64, val_size = 255;

kvStore kv(MAX_KEYS);

map<string, string> db;

long long db_size = 0;
Slice s_key, s_value;
uint OPS_COUNTER = 0;

/*
 * MODIFIED
 * Commented out this useless function
 */
void *myThreadFun(void *vargp) {
    int transactions = 0;
    clock_t start = clock();
    int time = 10;
    clock_t tt = clock();
    while ((float(tt - start) / CLOCKS_PER_SECOND) <= time) {
        for (int i = 0; i < 10000; i++) {
            transactions += 1;
            int x = rand() % 5;
            if (x == 0) {
                string key = random_key(rand() % key_size + 1);
                Slice s_key, s_value;
                strToSlice(key, s_key);
                bool ans = kv.get(s_key, s_value);
            } else if (x == 1) {
                string key = random_key(rand() % key_size + 1);
                string value = random_value(rand() % 255 + 1);
                Slice s_key, s_value, temp;
                strToSlice(key, s_key);
                strToSlice(value, s_value);

                bool check = kv.get(s_key, temp);
                bool ans = kv.put(s_key, s_value);

                if (check == false)
                    db_size++;
            } else if (x == 2) {
                int temp = db_size;
                if (temp == 0)
                    continue;
                int rem = rand() % temp;
                Slice s_key, s_value;
                bool check = kv.get(rem, s_key, s_value);
                check = kv.del(s_key);
                db_size--;
            } else if (x == 3) {
                int temp = db_size;
                if (temp == 0)
                    continue;
                int rem = rand() % temp;
                Slice s_key, s_value;
                bool check = kv.get(rem, s_key, s_value);
            } else if (x == 4) {
                int temp = db_size;
                if (temp == 0)
                    continue;
                int rem = rand() % temp;
                bool check = kv.del(rem);
                db_size--;
            }
        }
        tt = clock();
    }

    cout << transactions / time << endl;
    return NULL;
}

struct timespec ts;
long double st, en, total = 0;

int main() {
    long double total = 0;
    srand(time(0));

    for (int i = 0; i < INSERTS; i++) {
        string key = random_key(rand() % key_size + 1);

        while (db.find(key) != db.end())
            key = random_key(rand() % key_size + 1);

        string value = random_value(rand() % 255 + 1);
        db[key] = value;
        strToSlice(key, s_key);
        strToSlice(value, s_value);

        clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
        st = ts.tv_nsec / (1e9) + ts.tv_sec;
        kv.put(s_key, s_value);
        clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
        en = ts.tv_nsec / (1e9) + ts.tv_sec;
        total += (en - st);

        db_size = db.size();
        printf("\r%8d", i);
    }

    printf("\rInsertion of %u values took %Lfs\n", INSERTS, total);

    total = 0;

    bool incorrect = false;

    for (int i = 0; i < NUM_OPS; i++, OPS_COUNTER++) {
        int x = rand() % 5;
        if (x == 0) {
            // DESCRIPTION: GET
            string key = random_key(rand() % key_size + 1);
            // string key = random_key(key_size);
            strToSlice(key, s_key);

            clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
            st = ts.tv_nsec / (1e9) + ts.tv_sec;
            bool ans = kv.get(s_key, s_value);
            clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
            en = ts.tv_nsec / (1e9) + ts.tv_sec;
            total += (en - st);

            map<string, string>::iterator itr = db.find(key);
            if ((ans == false && itr != db.end()) ||
                (ans == true && itr->second != sliceToStr(s_value))) {
                incorrect = true;
                cout << "\nIncorrect GET for key " << key << "\nFound in kv? "
                     << ans << "\nFound in db? " << (itr != db.end())
                     << "\nValues equal? "
                     << (itr->second == sliceToStr(s_value)) << endl;
                cout << "Value in kv is " << sliceToStr(s_value)
                     << " and in db is " << itr->second << endl;
                cout << "Size of val in db is " << itr->second.size() << endl;
            }
        } else if (x == 1) {
            // DESCRIPTION: PUT
            int k = rand() % key_size + 1;
            int v = rand() % 255 + 1;
            string key = random_key(k);
            string value = random_value(v);
            db[key] = value;
            // printf("PUT OP: %s with val %s\n", key, value);
            strToSlice(key, s_key);
            strToSlice(value, s_value);

            clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
            st = ts.tv_nsec / (1e9) + ts.tv_sec;
            bool ans = kv.put(s_key, s_value);
            clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
            en = ts.tv_nsec / (1e9) + ts.tv_sec;
            total += (en - st);

            Slice check;
            bool check2 = kv.get(s_key, check);
            db_size = db.size();

            if (check2 == false || value != sliceToStr(check)) {
                incorrect = true;
                cout << "\nSome error with put, will check later" << endl;
            }
        } else if (x == 2) {
            // DESCRIPTION: DELETE
            int rem = rand() % db.size();
            map<string, string>::iterator itr = db.begin();
            advance(itr, rem);
            string key = itr->first;
            strToSlice(key, s_key);

            clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
            st = ts.tv_nsec / (1e9) + ts.tv_sec;
            bool check = kv.del(s_key);
            clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
            // printf("DEL OP\n");
            en = ts.tv_nsec / (1e9) + ts.tv_sec;
            total += (en - st);

            db.erase(itr);
            db_size--;

            bool check2 = kv.get(s_key, s_value);
            if (check2 == true) {
                incorrect = true;
                cout << "\nExpected to not find key " << key
                     << " in kv, found it" << endl;
            }
        } else if (x == 3) {
            // DESCRIPTION: GET N
            int rem = rand() % db_size;

            clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
            st = ts.tv_nsec / (1e9) + ts.tv_sec;
            bool check = kv.get(rem, s_key, s_value);
            clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
            en = ts.tv_nsec / (1e9) + ts.tv_sec;
            total += (en - st);

            map<string, string>::iterator itr = db.begin();
            for (int i = 0; i < rem; i++)
                itr++;

            if (itr->first != sliceToStr(s_key) ||
                itr->second != sliceToStr(s_value)) {
                incorrect = true;
                cout << "\nN: " << rem << '\n';
                cout << "get(n)\nkeys same? "
                     << (itr->first == sliceToStr(s_key)) << "\nvalues same? "
                     << (itr->second == sliceToStr(s_value)) << endl;
                cout << "Key: Supposed to be " << itr->first << "\n Found "
                     << sliceToStr(s_key) << '\n';
            }
        } else if (x == 4) {
            // DESCRIPTION: DELETE N
            int rem = rand() % db_size;
            map<string, string>::iterator itr = db.begin();
            for (int i = 0; i < rem; i++)
                itr++;
            string key = itr->first;

            clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
            st = ts.tv_nsec / (1e9) + ts.tv_sec;
            bool check = kv.del(rem);
            clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
            en = ts.tv_nsec / (1e9) + ts.tv_sec;
            total += (en - st);

            db.erase(itr);
            db_size--;
            strToSlice(key, s_key);
            bool check2 = kv.get(s_key, s_value);

            if (check2 == true) {
                incorrect = true;
                cout << "\nSupposed to delete key " << key;
                cout << "\nFound nth key " << key << " after deleting\n";
            }
        }

        //  printf("\r%8d", i);
        printf("\n%8d %10s::%Lfs", i, DESCRIPTION[x].c_str(), total);

        if (incorrect == true) {
            cout << "\rError in operation " << DESCRIPTION[x] << "\n Completed "
                 << OPS_COUNTER << " operations\n";
            return 0;
        }
    }

    cout << "\r" << NUM_OPS << " operations finished in " << total << "s\n";

    /*
     * MODIFIED
     * Commented out till the end
     */
    int threads = 4;

    pthread_t tid[threads];
    for (int i = 0; i < threads; i++) {
        tid[i] = i;
        pthread_create(&tid[i], NULL, myThreadFun, (void *)&tid[i]);
    }
    for (int i = 0; i < threads; i++)
        pthread_join(tid[i], NULL);
    return 0;
}
