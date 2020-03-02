#include <iostream>
#include <map>
#include <bits/stdc++.h>

#define TRIE_LIST_SIZE 0
#define TRIE_ARRAY_SIZE 52

using namespace std;

struct Slice {
    uint8_t size;
    char *data;

    ~Slice() {
        if(data != NULL) 
            free(data);
    }
};

struct SlideNodeBlock {
    SlideNodeBlock *next;
    Slice *node;
};

struct TrieNode {
    int childrenCount;
    int wordsBelow;
    char *word;
    Slice *value;
    TrieNode* children[TRIE_ARRAY_SIZE];
};

struct TrieNodeBlock {
    TrieNodeBlock *next;
    TrieNode *node;
};

class Trie {

   public:
    
    TrieNode *root;
    TrieNodeBlock *freeTrieList;
    Slice *freeSliceList;

    TrieNode* getTrieNode() {
        
        TrieNode *temp;

        if(freeTrieList) {
            temp = freeTrieList->node;
            freeTrieList = freeTrieList->next;
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
        
        for(int i=0; i<TRIE_ARRAY_SIZE; i++)
            temp->children[i] = NULL;

        return temp;
    }

    Trie() {
        
        // Initialize the TrieNodeBlock (the list of memory blocks)
        freeTrieList = (TrieNodeBlock *)malloc(sizeof(TrieNodeBlock));
        freeTrieList->next = NULL;
        freeTrieList->node = (TrieNode *)malloc(sizeof(TrieNode));

        // Generate the entire free list
        for(int i=0; i<TRIE_LIST_SIZE; i++) {
            TrieNodeBlock *temp = (TrieNodeBlock *)malloc(sizeof(TrieNode));
            
            temp->next = freeTrieList;
            temp->node = (TrieNode *)malloc(sizeof(TrieNode));
            
            freeTrieList = temp;
        }

        freeSliceList = (SlideNodeBlock *)malloc(sizeof(SliceNodeBlock));
        freeSliceList->next = NULL;
        freeSliceList->node = (Slice *)malloc(sizeof(Slice));

        for(int i=0; i<SLICE_LIST_SIZE; i++) {
            SliceNodeBlock *temp = (SliceNodeBlock *)malloc(sizeof(SliceNodeBlock));
            
            temp->next = freeSliceList;
            temp->node = (Slicce *)malloc(sizeof(Slice));

            freeSliceList = temp;
        }
    }


    ~Trie() {

        for(TrieNodeBlock *block = freeTrieList; block!=NULL; ) {
            TrieNodeBlock *temp = block;
            block = block->next;
            free(temp->node);
            free(temp);
        }

        for(SliceNodeBlock *block = freeSliceList; block!=NULL; ) {
            SliceNodeBlock *temp = block;
            block = block->next;
            free(temp->node);
            free(temp);
        }
        
    }

    void test_list() {
        TrieNode *test = getTrieNode();
        if(test==NULL)
            printf("ERROR\n");
    }

};

int main(void) {

    Trie t;
     
    long double total = 0;

    cout<<sizeof(TrieNodeBlock)<<" and  "<<sizeof(TrieNode)<<endl;

    for(int i=0; i<8000000; i++) {
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
