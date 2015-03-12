#include "Treap.h"
#include <cassert>
#include <iostream>

using namespace std;

int main() {
    Treap t;


    for (int c = 0; c < 1000000; ++c) {
        t.insert(c);
    }

    cout << "Valid: " << valid(t) << endl
        << "Max height: " << maxHeight(t) << endl
        << "Avg height: " << avgHeight(t) << endl;
    cin.get();

	return 0;
}