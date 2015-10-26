#include <cstdio>
#include <stdint.h>
#include <random>
#include <memory>
#include "common.h"

#include <chrono>
using namespace std;
using namespace std::chrono;

int main()
{
    mt19937_64 generator;
    uniform_int_distribution<uint64_t> dist(0, numeric_limits<uint64_t>::max());

    FILE * output = fopen("generated.dat", "wb");
    setvbuf(output , NULL , _IONBF , 0);

    int buff_index = 0;
    uint64_t count = 250000000, r = 0;
    unique_ptr<uint64_t[]> buff(new uint64_t[chunk_size]);

    for(uint64_t c = 0; c < count; ++c) {
        buff[buff_index++] = dist(generator);
        if (buff_index == chunk_size) {
            if (fwrite(&buff[0], sizeof(uint64_t), buff_index, output) != buff_index) {
                puts("Failed to write all elements in file");
                break;
            }
            buff_index = 0;
        }
    }
    if (buff_index) {
        if (fwrite(&buff[0], sizeof(uint64_t), buff_index, output) != buff_index) {
            puts("Failed to write all elements in file");
        }
    }
    
    fclose(output);

    return 0;
}
