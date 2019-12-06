#ifndef NODE_H
#define NODE_H

#include <vector>
#include <cmath>

using namespace std;

template<typename T>
class BTree;

template<typename T>
class Node {

public:
    int Degree, Minimas, Actuales;
    vector<int> keys;
    vector<Node<T> *> children;
    bool isLeaf;

    explicit Node(int size, bool isLeaf = true) : Degree(size), isLeaf(isLeaf) {
        keys.resize(size - 1); children.resize(size);
        Minimas = ceil(size / 2) - 1; Actuales = 0;
    }

    bool ImFull(){
        return Actuales == keys.size();
    }
    
    void split(int pos, Node<T> *node) {
        int flag = ceil((Degree - 1) / 2);
        auto temp = new Node<T>(node->Degree, node->isLeaf);

        temp->Actuales = flag - (Degree % 2);

        if(!node->isLeaf)
            for(int j=0 ; j <= temp->Actuales ; j++)
                temp->children[j] = node->children[flag + 1 + j];

        for(int j=0 ; j < temp->Actuales ; j++)
            temp->keys[j] = node->keys[flag + 1 + j];


        node->Actuales = flag;

        for(int i = Actuales + 1 ; i > pos + 1 ; i--)
            children[i] = children[i-1];

        children[pos+1] = temp;

        for(int i=Actuales ; i > pos ; i--)
            keys[i] = keys[i-1];

        keys[pos]= node->keys[flag];
        Actuales++;
    }

    void SimpleInsertion(T key) {
        int i = Actuales;

        if(!isLeaf) {
            while(i > 0 and key < keys[i-1])
                i--;
            if(children[i]->ImFull()){
                split(i, children[i] );
                if(keys[i] < key)
                    i++;
            }
            children[i]->SimpleInsertion(key);
            return;
        }

        while( i > 0 and key < keys[i-1]) {
            keys[i] = keys[i-1];
            i--;
        }
        keys[i] = key;
        Actuales++;
    }
    

    bool RemoveLogic(int key) {
       int index = getIndex(key);
        if(index < Actuales and keys[index] == key){
            if(isLeaf)
                DeleteLeafKey(index);
            else
                DeleteInternalKey(index);
        }
        else{
            if(isLeaf)
                return false;
            bool checker = (index == Actuales);
            if(children[index]->Actuales < Minimas)
                fill(index);
            if(checker and index > Actuales)
                children[index-1]->RemoveLogic(key);
            else
                children[index]->RemoveLogic(key);
        }
        return true;
    }

    void fill(int index) {
        if (index != 0 and children[index-1]->Actuales >= Minimas)
            PrevRotation(index);
        else if (index != Actuales and children[index+1]->Actuales >= Minimas)
            NextRotation(index);
        else
            if (index != Actuales)
                MergeNode(index);
            else
                MergeNode(index - 1);
    }


    void PrevRotation(int index) {
        auto hijo = children[index]; auto hermano = children[index-1];

        for(int i= hijo->Actuales - 1 ; i >= 0; i--)
            hijo->keys[i+1] = hijo->keys[i];

        
        if(!hijo->isLeaf)
            for(int i=hijo->Actuales ; i >= 0 ; i--)
                hijo->children[i+1] = hijo->children[i];

            
        hijo->keys[0] = keys[index-1];
        if(!hijo->isLeaf)
            hijo->children[0] = hermano->children[hermano->Actuales];
        keys[index-1] = hermano->keys[hermano->Actuales - 1];
        hijo->Actuales++;
        hermano->Actuales--;
    }

    void NextRotation(int index) {
        auto hijo = children[index]; auto hermano = children[index+1];
        hijo->keys[ hijo->Actuales] = keys[index];

        if (!hijo->isLeaf)
            hijo->children[ hijo->Actuales+1] = hermano->children[0];

        keys[index] = hermano->keys[0];

        for(int i=1; i<hermano->Actuales; i++)
            hermano->keys[i-1] = hermano->keys[i];

        if(!hermano->isLeaf)
            for(int i=1; i<=hermano->Actuales ; i++)
                hermano->children[i-1] = hermano->children[i];
        hijo->Actuales = hijo->Actuales + 1;
        hermano->Actuales = hermano->Actuales - 1;
    }

    void DeleteLeafKey(int index){
        for(int i=index+1; i < Actuales ; i++)
            keys[i-1] = keys[i];
        Actuales--;
    }

    void DeleteInternalKey(int index){
        auto key = keys[index];
        if (children[index]->Actuales >= Minimas) {
            auto prev = getPrev(index);
            keys[index] = prev;
            children[index]->RemoveLogic(prev);
        }
        else if (children[index+1]->Actuales >= Minimas) {
            auto next = getNext(index);
            keys[index] = next;
            children[index+1]->RemoveLogic(next);
        }
        else {
            MergeNode(index);
            children[index]->RemoveLogic(key);
        }
    }

    void MergeNode(int index) {
        auto hijo = children[index];
        auto hermano = children[index+1];
        hijo->keys[Minimas - 1] = keys[index];

        for(int i=0; i<hermano->Actuales; i++)
            hijo->keys[i + Minimas] = hermano->keys[i];

        if(!hijo->isLeaf)
            for(int i=0; i<hermano->Actuales;++i)
                hijo->children[i + Minimas] = hermano->children[i];

        for(int i=index+2; i < Actuales ; i++)
            children[i-1] = children[i];

        for(int i=index+1; i < Actuales ; i++)
            keys[i-1] = keys[i];

        hijo->Actuales = hijo->Actuales + hermano->Actuales + 1;
        Actuales--;
        delete hermano;
    }

    T getPrev(int index) {
        auto temp = children[index];
        while(!temp->isLeaf)
            temp = temp->children[temp->Actuales];
        return temp->keys[ temp->Actuales-1];
    }
    T getNext(int index) {
        auto temp = children[index+1];
        while(!temp->isLeaf)
            temp = temp->children[0];
        return temp->keys[0];
    }
    int getIndex(int key) {
        int i=0;
        while(i < Actuales and keys[i] < key)
            i++;
        return i;
    }
    void ImDone() {
        for(auto i :children) {
            if(i)
                i->ImDone();
        }
        delete this;
    }
    ~Node() {
        keys.clear(); children.clear();
    }

    friend class BTree<T>;
};

#endif