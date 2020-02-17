#include <iostream>
#include <map>
#include <bits/stdc++.h>
using namespace std;

struct Slice {
   public:
    uint8_t size;
    char *data;

    Slice() {
    }

    Slice(string a) {
        size = a.length();
        data = (char *)malloc(a.length());
        memcpy(data, a.data(), a.length());
    }
};

class Trie {
    struct TrieNode {
        char letter;
        bool is_word;
        string word;
        void *arr[52];
        Slice *value;
        int children;
    };

    TrieNode *root = (TrieNode *)malloc(sizeof(TrieNode));

   public:
    Trie() {
        root->letter = '&';
        root->children = 0;
        root->is_word = false;
        for (int i = 0; i < 52; i++) {
            root->arr[i] = NULL;
        }
        root->value = NULL;
    }

    void delete_recursive(TrieNode *node) {
        if (node == NULL)
            return;

        for (int i = 0; i < 52; i++)
            if (node->arr[i] != NULL)
                delete_recursive((TrieNode *)node->arr[i]);

        free(node);
    }

    ~Trie() {
        delete_recursive(root);
    }

    void insert(Slice &key, Slice &value) {
        int len = 0;
        TrieNode *curr = root;
        while (curr != NULL) {
            curr->children++;
            if (len < key.size) {
                int x = (key.data[len] > 90) ? key.data[len] - 97 + 26
                                                : key.data[len] - 65;
                if(curr->arr[x] == NULL) {
                    curr -> is_word = true;
                    for (int i=len; i<key.size; k++)
                        curr->word.append(key.data[i]);
                }
                else {
                    
                }
            } else if (len == key.size) {

            }
            len++;
        }
    }

    bool get_val(Slice &key, Slice &value) {
        int len = 0;
        TrieNode *curr = root;
        while (curr != NULL) {
            if (len < key.size) {
                int x = (key.data[len] > 90) ? key.data[len] - 97 + 26
                                             : key.data[len] - 65;
                if (curr->arr[x] == NULL)
                    return 0;
                curr = (TrieNode *)curr->arr[x];
            } else if (len == key.size) {
                if (curr->value == NULL)
                    return 0;
                else {
                    value.size = curr->value->size;
                    value.data = (char *)malloc(sizeof(char) * value.size);
                    for (int j = 0; j < value.size; j++) {
                        value.data[j] = curr->value->data[j];
                    }
                    return 1;
                }
            }
            len++;
        }
        // cur.arr['a'] = &cur;
        // cout << endl;
    }

    bool del(Slice &key) {
        int len = 0;
        TrieNode *curr = root;
        while (curr != NULL) {
            curr->children--;
            if (len < key.size) {
                int x = (key.data[len] > 90) ? key.data[len] - 97 + 26
                                             : key.data[len] - 65;
                if (curr->arr[x] == NULL)
                    return 0;
                TrieNode *p = curr;
                curr = (TrieNode *)curr->arr[x];
                if (p->children == 0 && p != root)
                    free(p);
                else {
                    if (curr->children == 1)
                        p->arr[x] = NULL;
                }
            } else if (len == key.size) {
                if (curr->value == NULL)
                    return 0;
                else {
                    // cout << curr->letter << " " << curr->children << endl;
                    if (curr->children == 0) {
                        free(curr);
                    } else {
                        free(curr->value);
                        curr->value = NULL;
                    }
                    return 1;
                }
            }
            len++;
        }

        return 1;
    }
};

// int main(void) {
//     Trie t;
//     Slice a("n");
//     Slice b("na");
//     // Slice c("nb");
//     Slice val("ragsga");

//     t.insert(a, val);
//     t.insert(b, val);
//     // t.insert(c, val);

//     cout << t.del(a) << endl;
//     cout << t.get_val(a, val) << endl;
//     // t.del(b);
//     // cout<< t.get_val(b, val) <<endl;
//     return 0;
// }