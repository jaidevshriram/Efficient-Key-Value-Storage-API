#pragma GCC optimization ("unroll-loops")
#pragma GCC optimization ("O3")
#pragma GCC target ("avx2")

#include <iostream>
#include <map>
#include <bits/stdc++.h>
using namespace std;

#define TRIE_LIST_SIZE 3500000
#define TRIE_ARRAY_SIZE 52

#define SLICE_LIST_SIZE 6000000

struct Slice {
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

struct TrieNode {
    char letter;
    void *arr[52];
    Slice *value;
    int children;
    Slice *word_span;
    int left, right;  // (both inclusive)
};

class Trie {

   public:

    TrieNode *root;
    TrieNode *freeTrieList;
    Slice *freeSliceList;
    int missedFreeTrieNode;
    int missedFreeSlice;

    TrieNode* getTrieNode() {
        
        TrieNode *temp;

        if(freeTrieList) {
            temp = freeTrieList;
            freeTrieList = (TrieNode *) freeTrieList->value;
        } else {
            missedFreeTrieNode++;
            temp = (TrieNode *)malloc(sizeof(TrieNode));
        }

        temp->left = temp->right = 0;
        temp->children = 0;
        temp->word_span = NULL;
        temp->value = NULL;
        
        for(register int i=0; i<TRIE_ARRAY_SIZE; i++)
            temp->arr[i] = NULL;

        return temp;
    }

    void freeTrieNode(TrieNode *node) {
        node->value = (Slice *) freeTrieList;
        freeTrieList = node;
    }

    Slice* getSlice() {
        
        Slice *temp;

        if(freeSliceList) {
            temp = freeSliceList;
            freeSliceList = (Slice *) freeSliceList->data;
        } else {
            missedFreeSlice++;
            temp = (Slice *)malloc(sizeof(Slice));
        }

        temp->size = 0;
        temp->data = NULL;

        return temp;
    }

    void freeSlice(Slice *node) {
        node->data = (char *) freeSliceList;
        freeSliceList = node;
    }

    Trie() {

        missedFreeTrieNode = 0;

        // Initialize the TrieNodeBlock (the list of memory blocks)
        freeTrieList = (TrieNode *)malloc(sizeof(TrieNode));
        freeTrieList->value = NULL;

        // Generate the entire free list
        for(int i=0; i<TRIE_LIST_SIZE; i++) {
            TrieNode *temp = (TrieNode *)malloc(sizeof(TrieNode));
            temp->value = (Slice *) freeTrieList;
            freeTrieList = temp;
        }
    
        freeSliceList = (Slice *)malloc(sizeof(Slice));
        freeSliceList->data = NULL;

        for(int i=0; i<SLICE_LIST_SIZE; i++) {
            Slice *temp = (Slice *)malloc(sizeof(Slice));
            temp->data = (char *) freeSliceList;
            freeSliceList = temp;
        }

        root = getTrieNode();
        root->letter = '&';
   }

    void delete_recursive(TrieNode *root_node)
    {
        if(root_node) {
            if(root_node->value->data)
                free(root_node->value->data);
            if(root_node->word_span->data)
                free(root_node->word_span->data);
            
            for(int i=0; i<TRIE_ARRAY_SIZE; i++)
                if(root_node->arr[i])
                    delete_recursive((TrieNode *)root_node->arr[i]);

            free(root_node);
        }
    }

    ~Trie() {

        printf("Missed Trie Allocations: %d\n", missedFreeTrieNode);
        printf("Missed Slice Allocations: %d\n", missedFreeSlice);

        // for(TrieNode *block = freeTrieList; block!=NULL; ) {
        //     TrieNode *temp = block;
        //     block = (TrieNode *) block->value;
        //     free(temp);
        // }

        // int i=0;
        // for(Slice *block = freeSliceList; block!=NULL; ) {
        //     Slice *temp = block;
        //     block = (Slice *) block->data;
        //     free(temp);
        // }
        
        // delete_recursive(root);
    }

    bool red_children(Slice &key) {
        // cout << key.data << '\n';
        Slice *new_key = getSlice();
        new_key->size = key.size;
        new_key->data = (char *)malloc(sizeof(char) * key.size);
        for (int i = 0; i < key.size; i++) {
            new_key->data[i] = key.data[i];
        }
        int len = 0;
        TrieNode *curr = root;
        while (curr != NULL) {
            curr->children--;
            if (len < key.size) {
                int x = (key.data[len] > 90) ? key.data[len] - 97 + 26
                                             : key.data[len] - 65;
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
                    }
                } else {
                    if (iter == curr->right + 1)
                        continue;
                }

            } else if (len == key.size) {
                return 1;
            }
            // len++;
        }
        // return 0;
        // cur.arr['a'] = &cur;
    }

    bool insert(Slice &key, Slice &value) {
        Slice *new_key = getSlice();
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
                    TrieNode *new_node = getTrieNode();
                    new_node->letter = key.data[len];
                    new_node->word_span = new_key;
                    new_node->left = len;
                    new_node->right = (int)key.size - 1;
                    new_node->children = 1;
                    
                    for (int i = 0; i < 52; i++) {
                        new_node->arr[i] = NULL;
                    }

                    new_node->value = getSlice();

                    new_node->value->size = value.size;
                    new_node->value->data = (char *)malloc(sizeof(char) * value.size);

                    for (int j = 0; j < value.size; j++) {
                        new_node->value->data[j] = value.data[j];
                    }
                    curr->arr[x] = new_node;
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
                
                if (len != (int)key.size) {
                    if (iter == curr->right + 1) {
                        continue;
                    } else {
                        // split here
                        // cout << key.data << " yo im " << '\n';
                        // view_all(0, root);
                        TrieNode *y = getTrieNode();
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
                        for (int i = 0; i < 52; i++) {
                            curr->arr[i] = NULL;
                        }
                        curr->arr[zzz] = y;
                        curr->right = iter - 1;
                        curr->value = NULL;
                        curr->children++;

                        // view_all(0, root);
                        TrieNode *new_node = getTrieNode();
                        new_node->letter = key.data[len];
                        new_node->word_span = new_key;
                        new_node->left = len;
                        new_node->right = (int)key.size - 1;
                        new_node->children = 1;
                        for (int i = 0; i < 52; i++) {
                            new_node->arr[i] = NULL;
                        }
                        new_node->value = getSlice();
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
                    if (iter == curr->right + 1)
                        continue;
                    else {
                        TrieNode *y = getTrieNode();
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
                        for (int i = 0; i < 52; i++) {
                            curr->arr[i] = NULL;
                        }
                        curr->arr[zzz] = y;
                        curr->right = iter - 1;
                        // curr->children++;
                        curr->value = NULL;
                    }
                }

            } else if (len == key.size) {
                bool rv = 1;
                if (curr->value == NULL)
                    rv = 0;
                // curr->value.data = value.data;
                curr->value = getSlice();
                curr->value->size = value.size;
                curr->value->data = (char *)malloc(sizeof(char) * value.size);
                for (int j = 0; j < value.size; j++) {
                    curr->value->data[j] = value.data[j];
                }
                if (rv == 1)
                    red_children(key);
                // cout << curr->value.data << '\n';
                return rv;
            }
            // len++;
        }
        cout << "UNREACHABLE\n";
        return 0;
        // cur.arr['a'] = &cur;
    }

    bool get_val(Slice &key, Slice &value) {
        int len = 0;
        TrieNode *curr = root;
        while (curr != NULL) {
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
        }

        cout << "UNREACHABLE\n";
        return 0;
    }

    bool del(Slice &key) {
        Slice v;

        int len = 0;
        TrieNode *curr = root;

        while (curr != NULL) {
            curr->children--;
            if (len < key.size) {
                int x = (key.data[len] > 90) ? key.data[len] - 97 + 26
                    : key.data[len] - 65;
                if (curr->arr[x] == NULL) {
                    return 0;
                }

                TrieNode* newCurr = (TrieNode *) curr->arr[x];

                if(newCurr->children == 1) {
                    curr->arr[x] = NULL;
                    curr = newCurr;
                    continue;
                }

                if(!curr->children) {
                    freeTrieNode(curr);
                }

                curr = newCurr;

                Slice * pp = curr->word_span;
                int iter = curr->left;
                while(iter <= curr->right && len < (int)key.size && pp->data[iter] == key.data[len]) {
                    len++;
                    iter++;
                }
                if(iter > curr->right) {
                    continue;
                } else {
                    return 0;
                }
            } else if (len == key.size) {
                if(curr->value == NULL) {
                    return 0;
                } else {
                    free(curr->value->data);
                    freeSlice(curr->value);
                    curr->value = NULL;
                    return 1;
                }
            }
        }

        cout << "UNREACHABLE\n";
        return 0;
    }

    bool get_val_N(int n, Slice &key, Slice &value) {
        n++;
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
                            for (int i = curr->left; i <= curr->right; i++) {
                                s += curr->word_span->data[i];
                            }
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
                        // s += (j >= 26) ? j + 97 - 26 : j + 65;
                        for (int i = p->left; i <= p->right; i++) {
                            s += p->word_span->data[i];
                        }
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

// int main(void) {
//     Trie t;
//     srand(time(NULL));
//     Slice val("ragsga");
//     Slice val2("rarara");
//     // {
//     //     Slice a("n");
//     //     Slice b("na");
//     //     Slice c("nan");
//     //     Slice d("nna");
//     //     Slice e("nanna");
//     //     t.insert(a, val);
//     //     t.insert(b, val);
//     //     t.insert(c, val);
//     //     t.insert(d, val);
//     //     t.insert(e, val);

//     //     t.view_all(0, t.root);
//     //     Slice vala("");
//     //     cout << t.get_val(a, vala) << '\n';
//     //     printf("%.*s\n", vala.size, vala.data);
//     //     Slice valb("");
//     //     cout << t.get_val(b, valb) << '\n';
//     //     // cout << valb.data << '\n';
//     //     Slice valc("");
//     //     cout << t.get_val(c, valc) << '\n';
//     //     // cout << valc.data << '\n';
//     //     Slice val4("");
//     //     cout << t.get_val(vala, valb) << '\n';
//     //     cout << t.get_val(val4, valb) << '\n';
//     // }
//     // {
//     //     Slice a("abcd");
//     //     Slice b("abce");
//     //     Slice c("abc");
//     //     t.insert(a, val);
//     //     t.view_all(0, t.root);
//     //     t.insert(b, val);
//     //     t.view_all(0, t.root);
//     //     t.insert(c, val);
//     //     t.view_all(0, t.root);
//     // }
//     // {
//     //     Slice a("abcd");
//     //     Slice b("abd");
//     //     Slice c("ab");
//     //     t.insert(a, val);
//     //     t.insert(b, val);
//     //     t.insert(c, val);
//     //     t.view_all(0, t.root);
//     // }
//     // {
//     //     Slice a("a");
//     //     Slice b("a");
//     //     Slice c("c");
//     //     Slice d("bb");
//     //     Slice e("bc");
//     //     t.insert(a, val);
//     //     t.insert(b, val);
//     //     t.insert(c, val);
//     //     t.insert(d, val);
//     //     t.insert(e, val);
//     //     t.view_all(0, t.root);
//     //     cout << t.get_val_N(0, a, val) << '\n';
//     //     cout << a.data << '\n';
//     //     cout << val.data << '\n';
//     //     cout << t.get_val_N(1, a, val) << '\n';
//     //     cout << a.data << '\n';
//     //     cout << val.data << '\n';
//     //     cout << t.get_val_N(2, a, val) << '\n';
//     //     cout << a.data << '\n';
//     //     cout << val.data << '\n';
//     //     cout << t.get_val_N(3, a, val) << '\n';
//     //     cout << a.data << '\n';
//     //     cout << val.data << '\n';
//     //     // cout << t.get_val_N(4, a, val) << '\n';
//     //     // cout << a.data << '\n';
//     //     // cout << val.data << '\n';
//     // }
//     {
//         Slice ddd;
//         ddd = Slice("wL");
//         t.insert(ddd, val);
//         ddd = Slice("u");
//         t.insert(ddd, val);
//         ddd = Slice("lm");
//         t.insert(ddd, val);
//         ddd = Slice("Zr");
//         t.insert(ddd, val);
//         ddd = Slice("sI");
//         t.insert(ddd, val);
//         ddd = Slice("QvD");
//         t.insert(ddd, val);
//         ddd = Slice("gV");
//         t.insert(ddd, val);
//         ddd = Slice("cXd");
//         t.insert(ddd, val);
//         ddd = Slice("k");
//         t.insert(ddd, val);
//         ddd = Slice("J");
//         t.insert(ddd, val);
//         ddd = Slice("faM");
//         t.insert(ddd, val);
//         ddd = Slice("W");
//         t.insert(ddd, val);
//         ddd = Slice("z");
//         t.insert(ddd, val);
//         ddd = Slice("qxs");
//         t.insert(ddd, val);
//         ddd = Slice("IK");
//         t.insert(ddd, val);
//         ddd = Slice("Mj");
//         t.insert(ddd, val);
//         ddd = Slice("Sok");
//         t.insert(ddd, val);
//         ddd = Slice("e");
//         t.insert(ddd, val);
//         ddd = Slice("tE");
//         t.insert(ddd, val);
//         ddd = Slice("WX");
//         t.insert(ddd, val);
//         ddd = Slice("E");
//         t.insert(ddd, val);
//         ddd = Slice("xY");
//         t.insert(ddd, val);
//         ddd = Slice("QD");
//         t.insert(ddd, val);
//         ddd = Slice("IF");
//         t.insert(ddd, val);
//         ddd = Slice("Omw");
//         t.insert(ddd, val);
//         ddd = Slice("mvv");
//         t.insert(ddd, val);
//         ddd = Slice("omF");
//         t.insert(ddd, val);
//         ddd = Slice("t");
//         t.insert(ddd, val);
//         ddd = Slice("qx");
//         t.insert(ddd, val);
//         ddd = Slice("Tae");
//         t.insert(ddd, val);
//         ddd = Slice("Oo");
//         t.insert(ddd, val);
//         ddd = Slice("H");
//         t.insert(ddd, val);
//         ddd = Slice("g");
//         t.insert(ddd, val);
//         ddd = Slice("qhw");
//         t.insert(ddd, val);
//         ddd = Slice("gDF");
//         t.insert(ddd, val);
//         ddd = Slice("QJ");
//         t.insert(ddd, val);
//         ddd = Slice("I");
//         t.insert(ddd, val);
//         ddd = Slice("D");
//         t.insert(ddd, val);
//         cout << t.get_val_N(29, ddd, val) << '\n';
//         cout << ddd.data << '\n';
//         t.view_all(0, t.root);
//     }
//     // {
//     //     Slice ddd;
//     //     ddd = Slice("qxs");
//     //     t.insert(ddd, val);
//     //     ddd = Slice("qx");
//     //     t.insert(ddd, val);
//     //     // t.view_all(0, t.root);
//     //     ddd = Slice("qhw");
//     //     t.insert(ddd, val);
//     //     t.view_all(0, t.root);
//     // }
//     // {
//     //     Slice ddd;
//     //     ddd = Slice("wLRbBMqbHcdARZoWkKyhidDQscdxRjMowfRXSJYbQ");
//     //     t.insert(ddd, val);
//     //     ddd = Slice("CSgsPqOQMQXNzlGdGwPbtrw");
//     //     t.insert(ddd, val);
//     //     ddd =
//     Slice("yxHOAchwDvmXxRdryxlmNDqtUKwaGMLejUuKWCIbXUBuMEnMEyATD");
//     //     t.insert(ddd, val);
//     //     ddd = Slice("UYoyPAyuLyeIMuOTehzriIcFsKPGgKbbI");
//     //     t.insert(ddd, val);
//     //     ddd =
//     Slice("wUzIfwovYDdwYvVbURCZMGYjgFDxvtnunNEsLSplwUIupfXLzbK");
//     //     t.insert(ddd, val);
//     //     ddd = Slice("JObMSkSKfojNewXGXnNOFwLTwJwnNvbwjcKDmeoUU");
//     //     t.insert(ddd, val);
//     //     ddd = Slice("dZnCqGjlApoPKvXfgVICEtCMKBljOPgtqVVHBGSDVIVheSn");
//     //     t.insert(ddd, val);
//     //     cout << t.get_val_N(1, ddd, val) << '\n';
//     //     cout << ddd.data << '\n';
//     // }
//     // for (int i = 0; i < 10000; i++) {
//     //     Slice s1 = random_key(rand() % 64 + 1);
//     //     Slice s2 = random_key(rand() % 256 + 1);
//     //     t.insert(s1, s2);
//     // }

//     // cout << t.count_ptr(t.root) << " " << t.root->children << '\n';
//     return 0;
// }
