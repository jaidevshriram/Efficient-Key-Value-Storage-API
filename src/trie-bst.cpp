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

struct TrieNode;

struct llNode {
    llNode() {}

    llNode (TrieNode * n) {
        this->node = n;
        this->next = 0;
    }

    TrieNode * node;
    llNode * next;
};

class List {
    public:
        llNode * root, * tail;
        uint8_t length;

        List() {
            root = tail = NULL;
            length = 0;
        }

        void insert(TrieNode * n) {
            length++;

            if(!root)
                root = tail = new llNode(n);

            tail->next = new llNode(n);
            tail = tail->next;
        }

        llNode * next(llNode * curr) {
            if(!curr)
                return NULL;

            return curr->next;
        }
};

struct Node {
    Node(char k, TrieNode * n) {
        this->k = k;
        this->l = this->r = NULL;
        this->node = n;
    }

    char k;
    Node * l, * r;
    TrieNode *node;
};

class BST {
    void __getAll(List * l, Node * rt) {
        if(!rt)
            return;

        __getAll(l, rt->l);
        (*l).insert(rt->node);
        __getAll(l, rt->r);
    }

    public:
    Node * root;

    BST() {
        root = 0;
    }

    void insert(char k, TrieNode * n) {
        if(!root) {
            root = new Node(k, n);
        }

        Node * curr = root,
             * prev = 0;

        while(curr) {
            prev = curr;

            if(k < curr->k)
                curr = curr->l;
            else
                curr = curr->r;
        }

        if(k < prev->k)
            prev->l = new Node(k, n);
        else
            prev->r = new Node(k, n);
    }

    Node * get(char k) {
        if(!root)
            return NULL;

        Node * curr = root;
        while(curr) {
            if(curr->k == k)
                return curr;
            if(k < curr->k)
                curr = curr->l;
            else
                curr = curr->r;
        }

        return NULL;
    }

    List getAll() {
        if(!root)
            return List();

        List l;
        __getAll(&l, root);
        return l;
    }
};


struct TrieNode {
    TrieNode() : arr() { }

    char letter;
    // map<char, void *> mp;
    // void *arr[52];
    BST arr;
    Slice *value;
    int children;
};

class Trie {
   public:
    TrieNode *root = new TrieNode();
    Trie() {
        root->letter = '&';
        root->children = 0;
        root->value = NULL;
    }

    void insert(Slice &key, Slice &value) {
        int len = 0;
        TrieNode *curr = root;
        while (curr != NULL) {
            curr->children++;
            if (len < key.size) {
                int x = (key.data[len] > 90) ? key.data[len] - 97 + 26
                                             : key.data[len] - 65;
                // if (curr->arr[x] == NULL) {
                Node * xVal = curr->arr.get(x);
                if (!xVal) {
                    TrieNode *new_node = new TrieNode();
                    new_node->letter = key.data[len];
                    new_node->children = 0;
                    new_node->value = NULL;
                    curr->arr.insert(x, new_node);
                    // cout << &new_node << '\n';
                }
                curr = curr->arr.get(x)->node;
            } else if (len == key.size) {
                // curr->value.data = value.data;
                curr->value = (Slice *)malloc(sizeof(Slice));
                curr->value->size = value.size;
                curr->value->data = (char *)malloc(sizeof(char) * value.size);
                for (int j = 0; j < value.size; j++) {
                    curr->value->data[j] = value.data[j];
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
                Node * xVal = curr->arr.get(x);
                if (!xVal)
                    return 0;
                curr = xVal->node;
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
        Slice v;
        if (!get_val(key, v))
            return 0;
        int len = 0;
        TrieNode *curr = root;

        while (curr != NULL) {
            curr->children--;
            if (len < key.size) {
                int x = (key.data[len] > 90) ? key.data[len] - 97 + 26
                                             : key.data[len] - 65;
                // if (curr->arr[x] == NULL)
                Node * xVal = curr->arr.get(x);
                if (!xVal)
                    return 0;
                TrieNode *p = curr;
                curr = xVal->node;
                if (p->children == 0 && p != root)
                    free(p);
                else {
                    if (curr->children == 1)
                        // TODO: DELETE
                        // p->arr.del(x);
                        // p->arr[x] = NULL;
                        xVal;
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

    // bool get_val_N(int n, Slice &key, Slice &value) {
    //     string s;
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

    // int count_ptr(TrieNode *a) {
    //     int x = 0;
    //     // int x = 0;
    //     for (int i = 0; i < 52; i++) {
    //         if (a->arr[i] != NULL) {
    //             x++;
    //             x += count_ptr((TrieNode *)a->arr[i]);
    //         }
    //     }
    //     return x;
    // }
};


/*
int main(void) {
    Trie t;
    Slice a("n");
    Slice b("na");
    Slice c("nan");
    Slice d("nna");
    Slice e("nanna");
    // Slice c("nb");
    Slice val("ragsga");

    t.insert(a, val);
    t.insert(b, val);
    t.insert(c, val);
    t.insert(d, val);
    t.insert(e, val);
    // t.insert(c, val);

    // cout << t.del(a) << endl;
    // cout << t.get_val(a, val) << endl;
    // t.del(b);
    // cout<< t.get_val(b, val) <<endl;
    cout << t.del_N(1) << '\n';
    cout << t.get_val_N(1, a, val) << '\n';
    cout << a.data << '\n';
    cout << val.data << '\n';
    cout << t.get_val_N(2, a, val) << '\n';
    cout << a.data << '\n';
    cout << val.data << '\n';
    cout << t.get_val_N(3, a, val) << '\n';
    cout << a.data << '\n';
    cout << val.data << '\n';
    cout << t.get_val_N(4, a, val) << '\n';
    cout << a.data << '\n';
    cout << val.data << '\n';
    cout << t.get_val_N(5, a, val) << '\n';
    cout << a.data << '\n';
    cout << val.data << '\n';
    cout << t.count_ptr(t.root) << " " << t.root->children << '\n';
    return 0;
}
*/
