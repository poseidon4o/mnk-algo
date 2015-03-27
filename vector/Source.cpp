#include <iostream>
#include "vector.hpp"
#include <string>
using namespace std;


int main() {
    ::vector<std::string> v;

    for (int c = 0; c < 3; ++c) {
        v.push_back(std::string("test"));
    }

    ::vector<std::string> q(v);

    q = v;


    cout << v.size();
    cin.get();
}
