#include <iostream>
#include <map>
using namespace std;

struct Slice {

public:
    uint8_t size;
    char* data;

    Slice() {
        
    }

    Slice(string a)
    {
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
        root->value.size = -1;
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
                    new_node->value.size = -1;
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
                curr = (TrieNode *)curr->arr[x];
            } else if (len == key.size) {
                if (curr->value.size == -1)
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
                if (curr->value.size == -1)
                    return 0;
                else {
                    if (curr->children == 0) {
                        free(curr);
                    } else
                        curr->value.size = -1;
                    return 1;
                }
            }
            len++;
        }
    }
};
/*
int main(void) {
    Trie t;
    Slice a, b;
    a.size = 5;
    a.data = (char *)malloc(sizeof(char) * a.size);
    a.data[0] = 'h';
    a.data[1] = 'e';
    a.data[2] = 'l';
    a.data[3] = 'l';
    a.data[4] = 'o';
    b.size = 5;
    b.data = (char *)malloc(sizeof(char) * b.size);
    b.data[0] = 'h';
    b.data[1] = 'h';
    b.data[2] = 'e';
    b.data[3] = 'h';
    b.data[4] = 'h';
    // b.data = {'h', 'e', 'l', 'l', 'o'};
    t.insert(a, b);
    b.data[4] = 'r';
    // a.data[4] = 'r';
    cout << b.data << '\n';
    cout << t.get_val(a, b) << '\n';
    cout << b.data << '\n';
    cout << t.del(a) << '\n';
    cout << t.get_val(a, b) << '\n';
    return 0;
}*/