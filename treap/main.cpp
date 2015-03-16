#include "Treap.h"
#include <cassert>
#include <iostream>

using namespace std;

int main() {
    Treap t;
    const int elements = 1000;
    bool is_valid = valid(t);

    for (int c = 0; c < elements; ++c) {
        t.insert(c);
        is_valid = valid(t) && is_valid;
    }

    
    for (int c = 0; c < elements; ++c) {
        t.remove(elements - c - 1);
        is_valid = valid(t) && is_valid;
    }

    cout << is_valid;

    //cout << "Valid: " << valid(t) << endl
    //    << "Max height: " << maxHeight(t) << endl
    //    << "Avg height: " << avgHeight(t) << endl;
    cin.get();

	return 0;
}