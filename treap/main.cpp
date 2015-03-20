#include "Treap.h"

#include <cassert>
#include <iostream>
#include <chrono>
#include <functional>
#include <random>

using namespace std;


template <typename T>
void test(const char * name, T fn) {
    cout << name << ": ";
    auto start = chrono::high_resolution_clock::now();
    fn();
    auto end = chrono::high_resolution_clock::now();
    cout << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;
}

void stats(const Treap & tr) {
    cout << "Valid: " << valid(tr) << endl
         << "Max height: " << maxHeight(tr) << endl
         << "Avg height: " << avgHeight(tr) << endl
         << "Elements: " << tr.size() << endl << endl;
}

const int test_size = 5000000;

int main() {
    Treap treap;

    mt19937 mt;
    uniform_int_distribution<int> dist(numeric_limits<int>::min(), numeric_limits<int>::max());
    auto rng = bind(dist, mt);
    int _ = 5;

    test("Ascending inserts", [&treap, &rng, &_] {
        for (int c = 0; c < test_size; ++c) {
            _ += rng();
            treap.insert(c);
        }
    });
    stats(treap);

    test("Inserts + deltes", [&treap, &rng] {
        for (int c = 0; c < test_size; ++c)  {
            int r = rng();
            if (r & 1) {
                treap.remove(c);
            } else {
                treap.insert(r);
            }
        }
    });
    stats(treap);

    treap.clear();

    test("Random inserts", [&treap, &rng] {
        for (int c = 0; c < test_size; ++c) {
            treap.insert(rng());
        }
    });
    stats(treap);

 
    cin.get();

	return 0;
}