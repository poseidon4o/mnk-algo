#include "SkipList.h"
#include "BST.h"

#include <random>
#include <chrono>
#include <iostream>
#include <functional>
#include <stdint.h>

using namespace std;
using namespace chrono;

void timeTest(function<void()> what, int samples) {
    typedef high_resolution_clock::rep rep;

    duration<rep, high_resolution_clock::period> time;

    for (int c = 0; c < samples; ++c) {
        auto start = chrono::high_resolution_clock::now();
        what();
        time_point<high_resolution_clock, high_resolution_clock::duration> end = chrono::high_resolution_clock::now();
        time += end - start;
    }
    rep total = duration_cast<milliseconds>(time).count();
    rep perSample = duration_cast<milliseconds>(time / samples).count();
    cout << samples << " samples for " << total << "ms, " << perSample << "ms per sample." << endl;
}


void timeTest(const char * testName, function<void()> bstTest, function<void()> slTest, int samples = 1) {
    cout << testName << endl;
    cout << "BST:" << endl;
    timeTest(bstTest, samples);
    cout << "SkipList:" << endl;
    timeTest(slTest, samples);
    cout << endl << endl;
}


const int n10k = 10000, n5k = 5000, samples = 10;

int main() {
    srand(10);

    BST<int> intTree;
    SkipList<int> intList(numeric_limits<int>::min(), numeric_limits<int>::max());

    timeTest("Ascending sequence of 10k ints", [intTree]() mutable {
        for (int c = 0; c < n10k; ++c) {
            intTree.Insert(c);
        }
    }, [intList]() mutable {
        for (int c = 0; c < n10k; ++c) {
            intList.Insert(c);
        }
    }, samples);

    timeTest("Ascending sequence of 5k ints deleted one by one from the first", [intTree]() mutable {
        for (int c = 0; c < n5k; ++c) {
            intTree.Insert(c);
        }
        for (int c = 0; c < n5k; ++c) {
            intTree.Delete(c);
        }
    }, [intList]() mutable {
        for (int c = 0; c < n5k; ++c) {
            intList.Insert(c);
        }
        for (int c = 0; c < n5k; ++c) {
            intList.Delete(c);
        }
    }, samples);

    timeTest("Ascending sequence of 5k ints deleted one by one from the last", [intTree]() mutable {
        for (int c = 0; c < n5k; ++c) {
            intTree.Insert(c);
        }
        for (int c = n5k; c >= 0; --c) {
            intTree.Delete(c);
        }
    }, [intList]() mutable {
        for (int c = 0; c < n5k; ++c) {
            intList.Insert(c);
        }
        for (int c = n5k; c >= 0; --c) {
            intList.Delete(c);
        }
    }, samples);

    timeTest("Descending sequence of 10k ints", [intTree]() mutable {
        for (int c = n10k; c >= 0; --c) {
            intTree.Insert(c);
        }
    }, [intList]() mutable {
        for (int c = n10k; c >= 0; --c) {
            intList.Insert(c);
        }
    }, samples);

    timeTest("Descending sequence of 5k ints deleted one by one from the first", [intTree]() mutable {
        for (int c = n5k; c >= 0; --c) {
            intTree.Insert(c);
        }
        for (int c = n5k; c >= 0; --c) {
            intTree.Delete(c);
        }
    }, [intList]() mutable {
        for (int c = n5k; c >= 0; --c) {
            intList.Insert(c);
        }
        for (int c = n5k; c >= 0; --c) {
            intList.Delete(c);
        }
    }, samples);

    timeTest("Descending sequence of 5k ints deleted one by one from the first", [intTree]() mutable {
        for (int c = n5k; c >= 0; --c) {
            intTree.Insert(c);
        }
        for (int c = 0; c < n5k; ++c) {
            intTree.Delete(c);
        }
    }, [intList]() mutable {
        for (int c = n5k; c >= 0; --c) {
            intList.Insert(c);
        }
        for (int c = 0; c < n5k; ++c) {
            intList.Delete(c);
        }
    }, samples);

    mt19937 gen;
    uniform_int_distribution<int> dist(numeric_limits<int>::min(), numeric_limits<int>::max());
    timeTest("Insert 100k random ints with uniform distribution", [intTree, &dist, &gen]() mutable {
        for (int c = 0; c < 10 * n10k; ++c) {
            intTree.Insert(dist(gen));
        }
    }, [intList, &dist, &gen]() mutable {
        for (int c = 0; c < 10 * n10k; ++c) {
            intList.Insert(dist(gen));
        }
    }, samples);

    return 0;
};