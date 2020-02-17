#include<bits/stdc++.h>
#include "../src/include/btree.hpp"

using namespace std;

class kvstore {
    public:
        BTree t;

        kvstore() : t(3) { }

        bool get(string key) {
            searchStruct * res = t.search(key);
            return res && res->exists;
        }

        bool put(string key, string value) {
            bool exists = t.search(key) != 0;
            t.insert((keyVal) { key, value });

            return exists;
        }

        bool del(string key) {
            bool exists = t.search(key) != 0;
            t.remove(key);
            exists = t.search(key) != 0;
            return exists;
        }

        pair<string,string> get(int N){
            // Your Code Here
            pair<string,string> temp = make_pair("key","value");
            return temp;
        }

        bool del(int N){
            // Your Code Here
            return true;
        }
};
