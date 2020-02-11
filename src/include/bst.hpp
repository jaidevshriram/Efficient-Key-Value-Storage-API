#include <cstring>

class BST {
    struct Node {
        Node(char ch, void * next) {
            this->ch = ch;
            this->next = next;
            this->left = this->right = 0;
        }

        char ch;
        void * next;
        Node * left, * right;
    };

    Node * root = 0;

    public:
    bool empty() {
        return (bool) root;
    }

    bool insert(char ch, void * next) {
        Node * it = root, * parent;
        bool lastLeft = false;

        if(!it) {
            it = new Node(ch, next);
            return true;
        }

        while(it) {
            parent = it;

            // go left
            if(std::strcmp(&ch, &(it->ch)) < 0) {
                it = it->left;
                lastLeft = true;
            } else if(std::strcmp(&ch, &(it->ch)) > 0) {
                it = it->right;
                lastLeft = false;
            } else {
                // should never reach here
                return false;
            }
        }

        it = new Node(ch, next);

        if (lastLeft)
            parent->left = it;
        else
            parent->right = it;

        return true;
    }

    bool del(char ch) {
        Node * it = root, * parent;
        bool lastLeft = false;

        if(!it)
            return false;

        while(it) {
            parent = it;

            // go left
            if(std::strcmp(&ch, &(it->ch)) < 0) {
                it = it->left;
                lastLeft = true;
            } else if(std::strcmp(&ch, &(it->ch)) > 0) {
                it = it->right;
                lastLeft = false;
            } else {
                // should never reach here
                return false;
            }
        }
    }
};
