#include "Treap.h"
#include <cassert>

int main() {
    Treap t;
    srand(5);

    for (int c = 0; c < 10000000; ++c) {
        t.insert(c);
    }

    assert(valid(t));

	return 0;
}