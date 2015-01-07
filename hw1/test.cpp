#include <cstdio>
#include <stdint.h>
#include <random>
#include <memory>
#include <iostream>
#include "common.h"


using namespace std;

int main()
{
    FILE * test_file = fopen("generated.dat", "rb");
    if(!test_file) {
        puts("Failed to open generated.dat");
        return -1;
    }
    setvbuf(test_file, NULL, _IONBF, 0);
    bool has_prev = false;

    unique_ptr<uint64_t[]> data(new uint64_t[chunk_size]);
    uint64_t prev_item;

    int read_count;
    do {
        read_count = fread(&data[0], sizeof(uint64_t), chunk_size, test_file);
        if (!read_count) {
            break;
        }

        if(has_prev) {
            if (!custom_cmp(prev_item, data[0])) {
                puts("Not sorted");
                break;
            }
        }
        for (int c = 0; c < read_count - 1; ++c) {
            if (!custom_cmp(data[c], data[c+1])) {
                puts("Not sorted");
                fclose(test_file);
                return 0;
            }
        }

        has_prev = true;
        prev_item = data[chunk_size - 1];
    } while(read_count == chunk_size);

    fclose(test_file);
    puts("Sorted");
    return 0;
}
