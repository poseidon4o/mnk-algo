#include "Splay.h"
#include "AVL.h"

#include <iostream>
using namespace std;

int main() {
    AVL t;

    for (int c = 0; c < 100; ++c) {
        t.Insert(c);
    }

    return 0;
}