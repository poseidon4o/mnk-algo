#include "SkipList.h"
#include <iostream>
using namespace std;

int main() {
    srand(10);
    SkipList sl(100, 999);
    for(int c = 101; c < 900; ++c) {
        sl.Insert(c);
    }
    cout << sl;
    cin.get();
    return 0;
};