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
        // map<char, void *> mp;
        void *arr[52];
        Slice value;
        int children;
    };

    TrieNode *root = (TrieNode *)malloc(sizeof(TrieNode));

   public:
    Trie() {
        root->letter = '&';
        root->children = 0;
        for (int i = 0; i < 52; i++) {
            root->arr[i] = NULL;
        }
        root->value.size = UINT8_MAX;
    }

    void delete_recursive(TrieNode *node)
    {
        if(node == NULL)
            return;

        for (int i=0; i<52; i++)
            if(node->arr[i] != NULL)
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
                if (curr->arr[x] == NULL) {
                    TrieNode *new_node = (TrieNode *)malloc(sizeof(TrieNode));
                    new_node->letter = key.data[len];
                    new_node->children = 0;
                    for (int i = 0; i < 52; i++) {
                        new_node->arr[i] = NULL;
                    }
                    new_node->value.size = UINT8_MAX;
                    curr->arr[x] = new_node;
                    // cout << &new_node << '\n';
                }
                curr = (TrieNode *)curr->arr[x];
            } else if (len == key.size) {
                // curr->value.data = value.data;
                curr->value.size = value.size;
                curr->value.data = (char *)malloc(sizeof(char) * value.size);
                for (int j = 0; j < value.size; j++) {
                    curr->value.data[j] = value.data[j];
                }
                // cout << curr->value.data << '\n';
                return;
            }
            len++;
        }
        // cur.arr['a'] = &cur;
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
                // cout << curr->letter << "-";

                curr = (TrieNode *)curr->arr[x];
            } else if (len == key.size) {
                // cout << curr->letter << endl;
                // cout << curr->value.size << endl;
                if (curr->value.size == UINT8_MAX)
                    return 0;
                else {
                    value = curr->value;
                    // cout << curr->value.data << '\n';
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
                if (curr->value.size == UINT8_MAX)
                    return 0;
                else {
                    // cout << curr->letter << " " << curr->children << endl;
                    if (curr->children == 0) {
                        free(curr);
                    } else {
                        curr->value.size = UINT8_MAX;
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