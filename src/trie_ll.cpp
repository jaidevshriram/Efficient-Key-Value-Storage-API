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
        void *start;
        Slice *value;
        int children;
    };

    struct LLNode {
        char letter;
        LLNode *next;
        TrieNode *t;
    };
    TrieNode *root = (TrieNode *)malloc(sizeof(TrieNode));

   public:
    Trie() {
        root->letter = '&';
        root->children = 0;
        root->start = NULL;
        root->value = NULL;
    }

    void delete_recursive(TrieNode *node) {
        if (node == NULL)
            return;

        for (LLNode *b = (LLNode *)node->start; b != NULL; b = b->next)
            delete_recursive((TrieNode *)b);

        free(node);
    }

    ~Trie() {
        delete_recursive(root);
    }

    bool insert(Slice &key, Slice &value) {
        int len = 0;
        TrieNode *curr = root;
        while (curr != NULL) {
            curr->children++;
            if (len < key.size) {
                int x = (key.data[len] > 90) ? key.data[len] - 97 + 26
                                             : key.data[len] - 65;
                LLNode *aa = NULL;
                for (LLNode *b = (LLNode *)curr->start; b != NULL;
                     b = b->next) {
                    if (b->letter == x) {
                        aa = b;
                        break;
                    }
                }
                if (aa == NULL) {
                    TrieNode *new_node = (TrieNode *)malloc(sizeof(TrieNode));
                    new_node->letter = key.data[len];
                    new_node->children = 0;
                    new_node->value = NULL;
                    new_node->start = NULL;
                    LLNode *l = (LLNode *)malloc(sizeof(LLNode));
                    LLNode *b;
                    if (curr->start == NULL ||
                        ((LLNode *)curr->start)->letter > x) {
                        if (curr->start != NULL)
                            l->next = ((LLNode *)curr->start)->next;
                        else
                            l->next = NULL;
                        curr->start = l;
                        l->letter = x;
                        l->t = new_node;
                    } else {
                        for (b = (LLNode *)curr->start;; b = b->next) {
                            if (b->next == NULL || b->next->letter > x) {
                                // l = b;
                                break;
                            }
                        }
                        l->next = b->next;
                        b->next = l;
                        l->letter = x;
                        l->t = new_node;
                    }
                    aa = l;
                    // cout << &new_node << '\n';
                }
                curr = aa->t;
            } else if (len == key.size) {
                bool rv = 1;
                if (curr->value == NULL)
                    rv = 0;
                // curr->value.data = value.data;
                curr->value = (Slice *)malloc(sizeof(Slice));
                curr->value->size = value.size;
                curr->value->data = (char *)malloc(sizeof(char) * value.size);
                for (int j = 0; j < value.size; j++) {
                    curr->value->data[j] = value.data[j];
                }
                // cout << curr->value.data << '\n';
                return rv;
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
                LLNode *aa = NULL;
                for (LLNode *b = (LLNode *)curr->start; b != NULL;
                     b = b->next) {
                    if (b->letter == x) {
                        aa = b;
                        break;
                    }
                }
                if (aa == NULL) {
                    return 0;
                }
                curr = aa->t;
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

    // bool del(Slice &key) {
    //     Slice v;
    //     if (!get_val(key, v))
    //         return 0;
    //     int len = 0;
    //     TrieNode *curr = root;

    //     while (curr != NULL) {
    //         curr->children--;
    //         if (len < key.size) {
    //             int x = (key.data[len] > 90) ? key.data[len] - 97 + 26
    //                                          : key.data[len] - 65;
    //             if (curr->arr[x] == NULL)
    //                 return 0;
    //             TrieNode *p = curr;
    //             curr = (TrieNode *)curr->arr[x];
    //             if (p->children == 0 && p != root)
    //                 free(p);
    //             else {
    //                 if (curr->children == 1)
    //                     p->arr[x] = NULL;
    //             }
    //         } else if (len == key.size) {
    //             if (curr->value == NULL)
    //                 return 0;
    //             else {
    //                 // cout << curr->letter << " " << curr->children << endl;
    //                 if (curr->children == 0) {
    //                     free(curr);
    //                 } else {
    //                     free(curr->value);
    //                     curr->value = NULL;
    //                 }
    //                 return 1;
    //             }
    //         }
    //         len++;
    //     }

    //     return 1;
    // }

    // bool get_val_N(int n, Slice &key, Slice &value) {
    //     string s;
    //     int len = 0;
    //     TrieNode *curr = root;
    //     while (curr != NULL) {
    //         // cout << curr->children << endl;
    //         if (curr->children < n) {
    //             return 0;
    //         }
    //         if (curr->children == 1) {
    //             while (curr->value == NULL) {
    //                 for (int j = 0; j < 52; j++) {
    //                     if (curr->arr[j] != NULL) {
    //                         curr = (TrieNode *)curr->arr[j];
    //                         s += (j >= 26) ? j + 97 - 26 : j + 65;
    //                         break;
    //                     }
    //                 }
    //             }
    //             key = s;
    //             value.size = curr->value->size;
    //             value.data = (char *)malloc(sizeof(char) * value.size);
    //             for (int i = 0; i < value.size; i++) {
    //                 value.data[i] = curr->value->data[i];
    //             }
    //             return 1;
    //         }
    //         if (n == 1 && curr->value != NULL) {
    //             key = s;
    //             value.size = curr->value->size;
    //             value.data = (char *)malloc(sizeof(char) * value.size);
    //             for (int i = 0; i < value.size; i++) {
    //                 value.data[i] = curr->value->data[i];
    //             }
    //             return 1;
    //         }
    //         if (curr->value != NULL)
    //             n--;
    //         for (int j = 0; j < 52; j++) {
    //             if (curr->arr[j] != NULL) {
    //                 TrieNode *p = (TrieNode *)curr->arr[j];
    //                 if (p->children >= n) {
    //                     s += (j >= 26) ? j + 97 - 26 : j + 65;
    //                     curr = p;
    //                     break;
    //                 } else {
    //                     n -= p->children;
    //                 }
    //             }
    //         }
    //     }
    //     // cur.arr['a'] = &cur;
    //     // cout << endl;
    // }

    // bool del_N(int n) {
    //     Slice a, b;
    //     get_val_N(n, a, b);
    //     del(a);
    // }
};
/*
int main(void) {
    Trie t;
    Slice a("CxEUCIQabPxnFxQUooSyYdMQsfsPWBVbY");
    Slice b("na");
    Slice c("nan");
    Slice d("nna");
    Slice e("nanna");
    // Slice c("nb");
    Slice val("ragsga");

    t.insert(a, val);
    cout << t.get_val(a, val) << endl;
    t.insert(b, val);
    // t.insert(c, val);
    cout << t.get_val(b, val) << endl;
    t.insert(d, val);
    cout << t.get_val(e, val) << endl;
    t.insert(e, val);
    cout << t.get_val(e, val) << endl;
    cout << t.get_val(a, val) << endl;
    // t.insert(c, val);

    // cout << t.del(a) << endl;
    cout << t.get_val(c, val) << endl;
    // t.del(b);
    // cout<< t.get_val(b, val) <<endl;
    // cout << t.del_N(1) << '\n';
    // cout << t.get_val_N(1, a, val) << '\n';
    // cout << a.data << '\n';
    // cout << val.data << '\n';
    // cout << t.get_val_N(2, a, val) << '\n';
    // cout << a.data << '\n';
    // cout << val.data << '\n';
    // cout << t.get_val_N(3, a, val) << '\n';
    // cout << a.data << '\n';
    // cout << val.data << '\n';
    // cout << t.get_val_N(4, a, val) << '\n';
    // cout << a.data << '\n';
    // cout << val.data << '\n';
    // cout << t.get_val_N(5, a, val) << '\n';
    // cout << a.data << '\n';
    // cout << val.data << '\n';
    return 0;
}
*/