#include <iostream>
#include <map>
#include "kvStore.hpp"
using namespace std;
// #include "bst.h"

class Trie {
    struct TrieNode {
        char letter;
        // map<char, void *> mp;
        void *arr[52];
        Slice value;
        int children;
    };
    TrieNode *root;

   public:
    Trie() {
        root->letter = '&';
        root->children = 0;
        for (int i = 0; i < 52; i++) {
            root->arr[i] = NULL;
        }
        root->value.size = -1;
    }
    void insert(Slice &key, Slice &value) {
        int len = 0;
        TrieNode *curr = root;
        while (curr == NULL) {
            if (len < key.size) {
                int x = (key.data[len] > 90) ? key.data[len] - 97 + 26
                                             : key.data[len] - 65;
                if (curr->arr[x]) {
                                }
            } else if (len == key.size) {
                curr->value = value;
                return;
            }
            len++;
        }
        // cur.arr['a'] = &cur;
    }
};

int main(void) {
    Trie t;
    return 0;
}