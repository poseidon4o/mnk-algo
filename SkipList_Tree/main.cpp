#include "SkipList.h"

#include <random>
#include <chrono>
#include <iostream>

using namespace std;

int main() {
    srand(10);
    SkipList sl(100, 999);

    for (int c = 0; c < 100; ++c) {
        for (int r = 101; r < 989; ++r) {
            sl.Insert(r);
        }
        for (int r = 988; r >= 101; --r) {
            sl.Delete(r);
        }
    }

    return 0;
};