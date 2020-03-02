#include <iostream>
#include <map>
#include <bits/stdc++.h>
using namespace std;

// void *makeTrieNode() {
//     p = malloc();
//     p->
// }

struct Slice {
   public:
    uint8_t size;
    char *data;

    Slice() {
    }

    Slice(string a) {
        size = a.length();
        // cout << to_string(size) << " " << a.length() << '\n';
        data = (char *)malloc(a.length());
        memcpy(data, a.data(), a.length());
    }
};

class Trie {
    Slice *keys = NULL;
    int num;
    struct TrieNode {
        char letter;
        // map<char, void *> mp;
        void *arr[52];
        Slice *value;
        int children;
        Slice *word_span;
        int left, right;  // (both inclusive)
    };

   public:
    TrieNode *root = (TrieNode *)malloc(sizeof(TrieNode));
    Trie() {
        root->letter = '&';
        root->children = 0;
        for (int i = 0; i < 52; i++) {
            root->arr[i] = NULL;
        }
        root->value = NULL;
    }

    // void delete_recursive(TrieNode *node) {
    //     if (node == NULL)
    //         return;

    //     for (int i = 0; i < 52; i++)
    //         if (node->arr[i] != NULL)
    //             delete_recursive((TrieNode *)node->arr[i]);

    //     free(node);
    // }

    // ~Trie() {
    //     delete_recursive(root);
    // }

    bool insert(Slice &key, Slice &value) {
        // cout << key.data << " pop " << '\n';
        Slice *new_key = (Slice *)malloc(sizeof(Slice));
        new_key->size = key.size;
        new_key->data = (char *)malloc(sizeof(char) * key.size);
        for (int i = 0; i < key.size; i++) {
            new_key->data[i] = key.data[i];
        }
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
                    new_node->word_span = new_key;
                    new_node->left = len;
                    new_node->right = (int)key.size - 1;
                    new_node->children = 0;
                    for (int i = 0; i < 52; i++) {
                        new_node->arr[i] = NULL;
                    }
                    new_node->value = (Slice *)malloc(sizeof(Slice));
                    new_node->value->size = value.size;
                    new_node->value->data =
                        (char *)malloc(sizeof(char) * value.size);
                    for (int j = 0; j < value.size; j++) {
                        new_node->value->data[j] = value.data[j];
                    }
                    curr->arr[x] = new_node;
                    // cout << x << " --- \n";
                    return 0;
                    // cout << &new_node << '\n';
                }
                curr = (TrieNode *)curr->arr[x];
                Slice *pp = curr->word_span;
                int iter = curr->left;
                while (iter <= curr->right && len < (int)key.size &&
                       pp->data[iter] == key.data[len]) {
                    len++;
                    iter++;
                }
                if (len != key.size) {
                    if (iter == curr->right + 1) {
                        continue;
                    } else {
                        // split here
                        // curr->word_span
                        TrieNode *y = (TrieNode *)malloc(sizeof(TrieNode));
                        y->value = curr->value;
                        y->right = curr->right;
                        y->left = iter;
                        y->children = curr->children;
                        y->letter = key.data[len];
                        y->word_span = curr->word_span;
                        for (int i = 0; i < 52; i++) {
                            y->arr[i] = curr->arr[i];
                        }
                        int zzz = (pp->data[iter] > 90)
                                      ? pp->data[iter] - 97 + 26
                                      : pp->data[iter] - 65;
                        curr->arr[zzz] = y;
                        curr->right = iter - 1;
                        curr->value = NULL;

                        TrieNode *new_node =
                            (TrieNode *)malloc(sizeof(TrieNode));
                        new_node->letter = key.data[len];
                        new_node->word_span = new_key;
                        new_node->left = len;
                        new_node->right = (int)key.size - 1;
                        new_node->children = 0;
                        for (int i = 0; i < 52; i++) {
                            new_node->arr[i] = NULL;
                        }
                        new_node->value = (Slice *)malloc(sizeof(Slice));
                        new_node->value->size = value.size;
                        new_node->value->data =
                            (char *)malloc(sizeof(char) * value.size);
                        for (int j = 0; j < value.size; j++) {
                            new_node->value->data[j] = value.data[j];
                        }
                        zzz = (key.data[len] > 90) ? key.data[len] - 97 + 26
                                                   : key.data[len] - 65;
                        curr->arr[zzz] = new_node;
                        return 0;
                    }
                } else {
                    // todo
                    if (iter == curr->right + 1)
                        continue;
                    else {
                        TrieNode *y = (TrieNode *)malloc(sizeof(TrieNode));
                        y->value = curr->value;
                        y->right = curr->right;
                        y->left = iter;
                        y->children = curr->children;
                        y->letter = pp->data[iter];
                        y->word_span = curr->word_span;
                        for (int i = 0; i < 52; i++) {
                            y->arr[i] = curr->arr[i];
                        }
                        int zzz = (pp->data[iter] > 90)
                                      ? pp->data[iter] - 97 + 26
                                      : pp->data[iter] - 65;
                        curr->arr[zzz] = y;
                        curr->right = iter - 1;
                        curr->value = NULL;
                    }
                }

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
            // len++;
        }
        // return 0;
        // cur.arr['a'] = &cur;
    }

    bool get_val(Slice &key, Slice &value) {
        int len = 0;
        TrieNode *curr = root;
        while (curr != NULL) {
            curr->children++;
            if (len < key.size) {
                int x = (key.data[len] > 90) ? key.data[len] - 97 + 26
                                             : key.data[len] - 65;
                if (curr->arr[x] == NULL) {
                    return 0;
                }
                curr = (TrieNode *)curr->arr[x];
                Slice *pp = curr->word_span;
                int iter = curr->left;
                while (iter <= curr->right && len < (int)key.size &&
                       pp->data[iter] == key.data[len]) {
                    len++;
                    iter++;
                }
                if (len != key.size) {
                    if (iter == curr->right + 1) {
                        continue;
                    } else {
                        return 0;
                    }
                } else {
                    // todo
                    if (iter == curr->right + 1)
                        continue;
                    else {
                        return 0;
                    }
                }

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
            // len++;
        }
        // int len = 0;
        // TrieNode *curr = root;
        // while (curr != NULL) {
        //     if (len < key.size) {
        //         int x = (key.data[len] > 90) ? key.data[len] - 97 + 26
        //                                      : key.data[len] - 65;
        //         if (curr->arr[x] == NULL)
        //             return 0;
        //         curr = (TrieNode *)curr->arr[x];
        //     } else if (len == key.size) {
        //         if (curr->value == NULL)
        //             return 0;
        //         else {
        //             value.size = curr->value->size;
        //             value.data = (char *)malloc(sizeof(char) * value.size);
        //             for (int j = 0; j < value.size; j++) {
        //                 value.data[j] = curr->value->data[j];
        //             }
        //             return 1;
        //         }
        //     }
        //     len++;
        // }
        // cur.arr['a'] = &cur;
        // cout << endl;
    }

    bool del(Slice &key) {
        int len = 0;
        TrieNode *curr = root;
        while (curr != NULL) {
            curr->children++;
            if (len < key.size) {
                int x = (key.data[len] > 90) ? key.data[len] - 97 + 26
                                             : key.data[len] - 65;
                if (curr->arr[x] == NULL) {
                    return 0;
                }
                curr = (TrieNode *)curr->arr[x];
                Slice *pp = curr->word_span;
                int iter = curr->left;
                while (iter <= curr->right && len < (int)key.size &&
                       pp->data[iter] == key.data[len]) {
                    len++;
                    iter++;
                }
                if (len != key.size) {
                    if (iter == curr->right + 1) {
                        continue;
                    } else {
                        return 0;
                    }
                } else {
                    // todo
                    if (iter == curr->right + 1)
                        continue;
                    else {
                        return 0;
                    }
                }

            } else if (len == key.size) {
                if (curr->value == NULL)
                    return 0;
                else {
                    free(curr->value->data);
                    free(curr->value);
                    curr->value = NULL;
                    return 1;
                }
            }
            // len++;
        }

        return 1;
        // Slice v;
        // if (!get_val(key, v))
        //     return 0;
        // int len = 0;
        // TrieNode *curr = root;

        // while (curr != NULL) {
        //     curr->children--;
        //     if (len < key.size) {
        //         int x = (key.data[len] > 90) ? key.data[len] - 97 + 26
        //                                      : key.data[len] - 65;
        //         if (curr->arr[x] == NULL)
        //             return 0;
        //         TrieNode *p = curr;
        //         curr = (TrieNode *)curr->arr[x];
        //         if (p->children == 0 && p != root)
        //             free(p);
        //         else {
        //             if (curr->children == 1)
        //                 p->arr[x] = NULL;
        //         }
        //     } else if (len == key.size) {
        //         if (curr->value == NULL)
        //             return 0;
        //         else {
        //             // cout << curr->letter << " " << curr->children << endl;
        //             if (curr->children == 0) {
        //                 free(curr);
        //             } else {
        //                 free(curr->value);
        //                 curr->value = NULL;
        //             }
        //             return 1;
        //         }
        //     }
        //     len++;
        // }
    }

    bool get_val_N(int n, Slice &key, Slice &value) {
        string s;
        int len = 0;
        TrieNode *curr = root;
        while (curr != NULL) {
            // cout << curr->children << endl;
            if (curr->children < n) {
                return 0;
            }
            if (curr->children == 1) {
                while (curr->value == NULL) {
                    for (int j = 0; j < 52; j++) {
                        if (curr->arr[j] != NULL) {
                            curr = (TrieNode *)curr->arr[j];
                            s += (j >= 26) ? j + 97 - 26 : j + 65;
                            break;
                        }
                    }
                }
                key = s;
                value.size = curr->value->size;
                value.data = (char *)malloc(sizeof(char) * value.size);
                for (int i = 0; i < value.size; i++) {
                    value.data[i] = curr->value->data[i];
                }
                return 1;
            }
            if (n == 1 && curr->value != NULL) {
                key = s;
                value.size = curr->value->size;
                value.data = (char *)malloc(sizeof(char) * value.size);
                for (int i = 0; i < value.size; i++) {
                    value.data[i] = curr->value->data[i];
                }
                return 1;
            }
            if (curr->value != NULL)
                n--;
            for (int j = 0; j < 52; j++) {
                if (curr->arr[j] != NULL) {
                    TrieNode *p = (TrieNode *)curr->arr[j];
                    if (p->children >= n) {
                        s += (j >= 26) ? j + 97 - 26 : j + 65;
                        curr = p;
                        break;
                    } else {
                        n -= p->children;
                    }
                }
            }
        }
        // cur.arr['a'] = &cur;
        // cout << endl;
    }

    bool del_N(int n) {
        Slice a, b;
        get_val_N(n, a, b);
        del(a);
    }

    void view_all(int depth, TrieNode *curr) {
        cout << "------------------\n";
        cout << depth << '\n';
        if (curr->word_span != NULL) {
            cout << curr->word_span->data << '\n';
            cout << curr->left << '\n';
            cout << curr->right << '\n';
        } else {
            cout << "None\n";
        }

        cout << curr->children << '\n';
        cout << "------------------\n";
        for (int i = 0; i < 52; i++) {
            if (curr->arr[i] != NULL) {
                // int zzz = (key.data[len] > 90) ? key.data[len] - 97 + 26
                //                                : key.data[len] - 65;
                cout << i << " +++ \n";
                view_all(depth + 1, (TrieNode *)curr->arr[i]);
            }
        }
    }

    int count_ptr(TrieNode *a) {
        int x = 0;
        // int x = 0;
        for (int i = 0; i < 52; i++) {
            if (a->arr[i] != NULL) {
                x++;
                x += count_ptr((TrieNode *)a->arr[i]);
            }
        }
        return x;
    }
};

// #ifdef BENCH
// int main(void) {
//     Trie t;
//     srand(time(NULL));
//     Slice val("ragsga");
//     Slice val2("");
//     {
//         Slice a("n");
//         Slice b("na");
//         Slice c("nan");
//         Slice d("nna");
//         Slice e("nanna");
//         t.insert(a, val);
//         t.insert(b, val);
//         t.insert(c, val);
//         t.insert(d, val);
//         t.insert(e, val);

//         // t.view_all(0, t.root);
//         Slice vala("");
//         cout << t.get_val(a, vala) << '\n';
//         printf("%.*s\n", vala.size, vala.data);
//         Slice valb("");
//         cout << t.get_val(b, valb) << '\n';
//         // cout << valb.data << '\n';
//         Slice valc("");
//         cout << t.get_val(c, valc) << '\n';
//         // cout << valc.data << '\n';
//         Slice val4("");
//         cout << t.get_val(vala, valb) << '\n';
//         cout << t.get_val(val4, valb) << '\n';
//     }
//     // {
//     //     Slice a("abcd");
//     //     Slice b("abd");
//     //     Slice c("abc");
//     //     t.insert(a, val);
//     //     t.insert(b, val);
//     //     t.insert(c, val);
//     // t.view_all(0, t.root);
//     // }
//     // {
//     //     Slice a("abcd");
//     //     Slice b("abd");
//     //     Slice c("ab");
//     //     t.insert(a, val);
//     //     t.insert(b, val);
//     //     t.insert(c, val);
//     // t.view_all(0, t.root);
//     // }
//     // cout << a.size << '\n';
//     // Slice c("nb");

//     // t.insert(c, val);

//     // // cout << t.del(a) << endl;
//     // // cout << t.get_val(a, val) << endl;
//     // // t.del(b);
//     // // cout<< t.get_val(b, val) <<endl;
//     // cout << t.del_N(1) << '\n';
//     // cout << t.get_val_N(1, a, val) << '\n';
//     // cout << a.data << '\n';
//     // cout << val.data << '\n';
//     // cout << t.get_val_N(2, a, val) << '\n';
//     // cout << a.data << '\n';
//     // cout << val.data << '\n';
//     // cout << t.get_val_N(3, a, val) << '\n';
//     // cout << a.data << '\n';
//     // cout << val.data << '\n';
//     // cout << t.get_val_N(4, a, val) << '\n';
//     // cout << a.data << '\n';
//     // cout << val.data << '\n';
//     // cout << t.get_val_N(5, a, val) << '\n';
//     // cout << a.data << '\n';
//     // cout << val.data << '\n';

//     // for (int i = 0; i < 10000; i++) {
//     //     Slice s1 = random_key(rand() % 64 + 1);
//     //     Slice s2 = random_key(rand() % 256 + 1);
//     //     t.insert(s1, s2);
//     // }

//     // cout << t.count_ptr(t.root) << " " << t.root->children << '\n';
//     return 0;
// }