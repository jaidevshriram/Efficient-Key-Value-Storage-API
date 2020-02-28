#include <iostream>
#include <string.h>
#include <map>
#include <bits/stdc++.h>
using namespace std;

struct Slice {
    /*
     * TODO
     * uint8_t will take much less space (4x lesser)
     * have to check if it works
     */
    int size;
    char* data;

    Slice()
    {
        size = 0;
        data = NULL;
    }

    Slice(const char *a)
    {
        size = strlen(a);   
        
        if(size == 0)
        {
            printf("\t\033[1;31mTHERE IS NO STRING WHAT EVEN IS HAPPENING\033[0m\n");
            //exit(-1);
        }

        data = (char *)malloc(size + 1);
        strcpy(data, a);
        
        //printf("%s from %s : DONE COPYING\n", data, a);
    }
};

class Trie {
    struct TrieNode {
        char letter;
        bool is_word;
        char *word;
        void *arr[52];
        Slice *value;
        int children;

        TrieNode() {
            is_word = false;
        }
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

    void insert(Slice &key, Slice* &value, TrieNode* curr) {
        
        TrieNode *new_root = curr;
        int len = 0;
        
        while (curr != NULL) {
            curr->children++;
            if (len < key.size) {
                int x = (key.data[len] > 90) ? key.data[len] - 97 + 26
                                                : key.data[len] - 65;
                if(curr->arr[x] == NULL && curr!=new_root) {
                    curr -> is_word = true;
                    char *temp = (char *)malloc(key.size - len + 3);
                    
                    if(temp == NULL)
                    {
                        printf("Malloc fail\n");
                        exit(-1);
                    }
                    
                    strncpy(temp, key.data + len, key.size - len + 1);
                    temp[key.size - len] = '\0';
                    curr->word = temp;
                    
                    curr->value = (Slice *)malloc(sizeof(Slice));
                    curr->value->size = value->size;
                    curr->value->data = (char *)malloc(sizeof(char) * value->size);
                    
                    for (int j = 0; j < value->size; j++) {
                        curr->value->data[j] = value->data[j];
                    }
                    
                    return;
                }
                else if(curr->arr[x] == NULL)
                {
                    TrieNode *new_node = (TrieNode *)malloc(sizeof(TrieNode));
                    new_node->letter = key.data[len];
                    new_node->children = 0;
                    new_node->is_word = false;
                    for (int i = 0; i < 52; i++) {
                        new_node->arr[i] = NULL;
                    }
                    new_node->value = NULL;
                    curr->arr[x] = new_node;
                    curr = (TrieNode *)curr->arr[x];
                }
                else {
                    curr = (TrieNode *)curr->arr[x];
                }
            } else if (len == key.size) {
                
                if(curr == NULL || value==NULL)
                {
                    cout << "NULL: Unreachable state!"<<endl;
                }

                curr->value = (Slice *)malloc(sizeof(Slice));
                curr->value->size = value->size;
                curr->value->data = (char *)malloc(sizeof(char) * value->size);
                for (int j = 0; j < value->size; j++) {
                    curr->value->data[j] = value->data[j];
                }
                return;           
            }
            len++;
        }
    }

    void insert(Slice &key, Slice &value) {
        int len = 0;
        TrieNode *curr = root;
        while (curr != NULL) {
            curr->children++;
            if (len < key.size) {
                int x = (key.data[len] > 90) ? key.data[len] - 97 + 26
                                                : key.data[len] - 65;
                if(curr->arr[x] == NULL && curr!=root && curr->value==NULL) {
                    curr->is_word = true;
                    
                    char *temp = (char *)malloc(key.size - len + 3);
                    
                    if(temp == NULL)
                    {
                        printf("Malloc fail\n");
                        exit(-1);
                    }
                    
                    strncpy(temp, key.data + len, key.size - len + 1);
                    temp[key.size - len] = '\0';
                    curr->word = temp;
                    curr->value = (Slice *)malloc(sizeof(Slice));
                    curr->value->size = value.size;
                    curr->value->data = (char *)malloc(sizeof(char) * value.size);
                    for (int j = 0; j < value.size; j++) {
                        curr->value->data[j] = value.data[j];
                    }
                    
                    return;
                }
                else if(curr->arr[x] == NULL)
                {
                    TrieNode *new_node = (TrieNode *)malloc(sizeof(TrieNode));
                    new_node->letter = key.data[len];
                    new_node->children = 0;
                    new_node->is_word = false;
                    for (int i = 0; i < 52; i++) {
                        new_node->arr[i] = NULL;
                    }
                    new_node->value = NULL;
                    curr->arr[x] = new_node;
                    curr = (TrieNode *)curr->arr[x];
                }
                else {
                    curr = (TrieNode*)curr->arr[x];
                    if (curr->is_word) {
                        curr->is_word = false;
                        x = (key.data[len+1] > 90) ? key.data[len+1] - 97 + 26
                                                : key.data[len+1] - 65;
                        if(strlen(curr->word) == 0)
                        {
                            printf("%c is letter\n", curr->letter);
                        }

                        Slice new_key(curr->word);
                        free(curr->word);
                        insert(new_key, curr->value, curr);
                        curr->value = NULL;
                    }
                }
            } else if (len == key.size) {
                curr->value = (Slice *)malloc(sizeof(Slice));
                curr->value->size = value.size;
                curr->value->data = (char *)malloc(sizeof(char) * value.size);
                for (int j = 0; j < value.size; j++) {
                    curr->value->data[j] = value.data[j];
                }
                return;           
            }
            len++;
        }
    }

    bool get_val(Slice &key, Slice &value) {
        int len = 0;
        TrieNode *curr = root;
        
        if(curr == NULL)
            return 0;

        while (curr != NULL) {
            if (len < key.size) {
                int x = (key.data[len] > 90) ? key.data[len] - 97 + 26
                                             : key.data[len] - 65;

                if(curr!=root && curr->is_word)
                {
                    int temp_len = len;

                    if(strlen(curr->word) != (uint)(key.size - temp_len))
                    {
                        return 0;
                    }
                    
                    for(uint i=0; i<strlen(curr->word); i++)
                    {
                        if(key.data[temp_len + i] != curr->word[i])
                            return 0;
                    }

                    value.size = curr->value->size;
                    value.data = (char *)malloc(sizeof(char) * value.size);
                    for (int j = 0; j < value.size; j++) {
                        value.data[j] = curr->value->data[j];
                    }

                    return 1;
                }

                if (curr->arr[x] == NULL)
                    return 0;

                curr = (TrieNode *)curr->arr[x];
            } else if (len == key.size) {
                if (curr->value == NULL)
                    return 0;
                else {

                    if(curr->is_word)
                        return 0;

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

        // TODO: yoogottamk added this, need to verify with overall logic
        return 0;
    }

    int del(Slice &key) {
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

    void display(TrieNode *root, int level)
    {
        if (root == NULL)
            return;
         for(int i=0; i<level; i++) cout<<"\t";
        printf("------------\n");      
        for(int i=0; i<level; i++) cout<<"\t";
        printf("Level %d\n", level);
         for(int i=0; i<level; i++) cout<<"\t";
        cout << "Children: "<<root->children <<endl;
          for(int i=0; i<level; i++) cout<<"\t";
       cout << "Is_word:" << root->is_word<<endl;
        if(root->is_word)
        {
         for(int i=0; i<level; i++) cout<<"\t";
            cout <<root->letter<<endl;
         for(int i=0; i<level; i++) cout<<"\t";
            cout <<root->word<<endl;
        }
        else
        {
         for(int i=0; i<level; i++) cout<<"\t";
            cout << "Letter:"<< root->letter<<endl;;
            for(int i=0; i<52; i++)
                display((TrieNode *)root->arr[i], level + 1);
        }
          for(int i=0; i<level; i++) cout<<"\t";
       printf("---*--*---\n");
    }

    void display()
    {
        display(root, 0);
    }
};

/*
int main(void) {
    Trie t;
    Slice a("ab");
    Slice b("bb");
    Slice c("abb");
    Slice val("ragsga");

    t.insert(a, val);
    t.insert(b, val);
    t.insert(c, val);
    t.display();
    // cout << t.del(a) << endl;
    cout << t.get_val(a, val) << endl;
    cout<< t.get_val(b, val) <<endl;
    cout<< t.get_val(c, val) <<endl;
    return 0;
}*/
