#include <iostream>
#include "vector.hpp"

using namespace std;

struct ff {
    char s;

    ff(const ff &&) {

    }

    ff(int, int) {}

private:

    ff();

    ff & operator=(const ff &);
    
};

int main() {
    ::vector<int> v;

    for (int c = 0; c < 10; ++c) {
        v.push_back(c);
    }
    cout << 5;
    cin.get();
}
