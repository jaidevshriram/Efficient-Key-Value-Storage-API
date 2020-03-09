#include <iostream>
#include <map>
#include <bits/stdc++.h>

#define TRIE_LIST_SIZE 8000000
#define TRIE_ARRAY_SIZE 52

#define SLICE_LIST_SIZE 0

using namespace std;

struct Slice {
    uint8_t size;
    char *data;

    ~Slice() {
        if(data != NULL) 
            free(data);
    }
};

struct TrieNode {
    int childrenCount;
    int wordsBelow;
    char *word;
    Slice *value;
    TrieNode* children[TRIE_ARRAY_SIZE];
};

class Trie {

   public:
    
    TrieNode *root;
    TrieNode *freeTrieList;
    Slice *freeSliceList;

    TrieNode* getTrieNode() {
        
        TrieNode *temp;

        if(freeTrieList) {
            temp = freeTrieList;
            freeTrieList = (TrieNode *) freeTrieList->word;
        } else {
#ifdef EBUG
            cout<<"Empty List\n";
#endif
            temp = (TrieNode *)malloc(sizeof(TrieNode));
        }

        temp->childrenCount = 0;
        temp->wordsBelow = 0;
        temp->word = NULL;
        temp->value = NULL;
        
        for(register int i=0; i<TRIE_ARRAY_SIZE; i++)
            temp->children[i] = NULL;

        return temp;
    }

    void freeTrieNode(TrieNode *node) {
        node->word = (char *) freeTrieList;
        freeTrieList = node;
    }

    Slice* getSlice() {
        
        Slice *temp;

        if(freeSliceList) {
            temp = freeSliceList;
            freeSliceList = (Slice *) freeSliceList->data;
        } else {
            temp = (Slice *)malloc(sizeof(Slice));
        }

        temp->size = 0;
        temp->data = NULL;

        return temp;
    }

    Trie() {

        root = NULL;
        
        // Initialize the TrieNodeBlock (the list of memory blocks)
        freeTrieList = (TrieNode *)malloc(sizeof(TrieNode));
        freeTrieList->word = NULL;

        // Generate the entire free list
        for(int i=0; i<TRIE_LIST_SIZE; i++) {
            TrieNode *temp = (TrieNode *)malloc(sizeof(TrieNode));
            temp->word = (char *) freeTrieList;
            freeTrieList = temp;
        }
    
        freeSliceList = (Slice *)malloc(sizeof(Slice));
        freeSliceList->data = NULL;

        for(int i=0; i<SLICE_LIST_SIZE; i++) {
            Slice *temp = (Slice *)malloc(sizeof(Slice));
            temp->data = (char *) freeSliceList;
            freeSliceList = temp;
        }

   }


    ~Trie() {

        for(TrieNode *block = freeTrieList; block!=NULL; ) {
            TrieNode *temp = block;
            block = (TrieNode *) block->word;
            free(temp);
        }

        int i=0;
        for(Slice *block = freeSliceList; block!=NULL; ) {
            Slice *temp = block;
            block = (Slice *) block->data;
            free(temp);
        }
        
        if(root!=NULL)
            free(root);
    }

    void test_list() {
        TrieNode *test = getTrieNode();
        //Slice *test = getSlice();
        if(test==NULL)
            printf("\rERROR\n");
    }

};

int main(void) {

    Trie t;
     
    long double total = 0;

    for(int i=0; i<8000005; i++) {
        struct timespec ts;
        clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
        long double st = ts.tv_nsec / (1e9) + ts.tv_sec;
        t.test_list();
        clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
        long double en = ts.tv_nsec / (1e9) + ts.tv_sec;
        total += (en - st);
    }

    cout<<"Total time: "<<total<<endl;
}
