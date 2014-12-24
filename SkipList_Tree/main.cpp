#include "SkipList.h"

#include <random>
#include <chrono>
#include <iostream>

using namespace std;

int main() {
    srand(10);
    SkipList sl(100, 999);

    auto start = chrono::high_resolution_clock::now();

    for (int c = 0; c < 100; ++c) {
        for (int r = 101; r < 989; ++r) {
            sl.Insert(r);
        }
        for (int r = 988; r >= 101; --r) {
            sl.Delete(r);
        }
    }

    auto end = chrono::high_resolution_clock::now();

    cout << chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cin.get();

    return 0;
};