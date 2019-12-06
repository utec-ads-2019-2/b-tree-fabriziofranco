#ifndef BTREE_H
#define BTREE_H

#include "node.h"

template <typename T>
class BTree {
private:
    Node<T>* root;
    int Degree;
    void TraverseInOrder(Node<T>* node) {
        int i;
        for (i = 0; i < node->Actuales; ++i) {
            if (!node->isLeaf and  node->children[i])
                TraverseInOrder(node->children[i]);
            cout << " " << node->keys[i];
        }
        if (!node->isLeaf)
            TraverseInOrder(node->children[i]);
    }
    Node<T> *Search(int key, Node<T>* node) {
        int i = 0;
        while (i < node->Actuales and key > node->keys[i])
            i++;

        if(i == Degree - 1) return nullptr;

        if (node->keys[i] == key) return node;

        if (node->isLeaf) return nullptr;

        return Search(key, node->children[i]);
    }
    void SetRoot(T data) {
        root = new Node<T>(Degree, true);
        root->keys[0] = data;
        root->Actuales++;
    }
    void NeedToSplitNode(T key) {
        auto temp = new Node<T>(Degree, false);
        temp->children[0] = root;
        temp->split(0, root);
        auto i= 0;
        if (temp->keys[0] < key)
            i++;
        temp->children[i]->SimpleInsertion(key);
        root = temp;
    }

public:

    explicit BTree(int Degree) : Degree(Degree), root(nullptr) {};
    bool Search(int key) {
        if (!root)
            return false;
        else
            return Search(key, root);
    }
    bool Insert(T key){
        if (Search(key))
            return false;
        else
            if (!root)
                SetRoot(key);
            else
                if (root->ImFull())
                    NeedToSplitNode(key);
                else
                    root->SimpleInsertion(key);
        return true;
    }



    bool Remove(int key) {
        bool result=false;

        if(!root)
            throw runtime_error("Empty three");

        result = root->RemoveLogic(key);

        if(!root->Actuales) {
            auto tmp = root;
            if(root->isLeaf)
                root = nullptr;
            else {
                root = root->children[0];

                if(root->isLeaf ) {
                    root = nullptr;
                    delete root;
                }
            }
            delete tmp;
        }
        return result;
    }

    void Print() {
        if(root)
            TraverseInOrder(root);
        cout<<endl;
    }

    ~BTree() {
        if(root)
            root->ImDone();
    }
};

#endif