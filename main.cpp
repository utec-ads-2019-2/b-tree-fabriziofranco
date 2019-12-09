#include <iostream>
#include "btree.h"

using namespace std;

int main(int argc, char *argv[]) {
    BTree<int> TesterTree(3);

    for(auto i=1;i<11;i++)
        TesterTree.Insert(i);

    TesterTree.Print();

    // El destructor está generando un error de punteros, probablemente tengas un puntero que queda libre desde el insert 
    for(auto i=0;i<=10;i+=2)
        TesterTree.Remove(i);

    TesterTree.Print();
    return EXIT_SUCCESS;
}
