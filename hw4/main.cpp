#include "Splay.h"

#include <iostream>
using namespace std;

int main() {
    SplayTree tree;


    tree.Insert(0);
    tree.Insert(1);
    tree.Insert(2);

    tree.Find(0);
    tree.Find(1);



    cin.get();
    return 0;
}