#include <algorithm>
#include <cstdio>
#include <array>
#include <iostream>
#include <string>

#include "common.h"

typedef std::array<FILE *, 4> file_group;
using namespace std;

struct {
    const char * value;
} names[4] = {
    { "left_up.dat" }, { "left_down.dat" }, { "right_up.dat" }, { "right_down.dat" }
};


class swap_files {
    file_group files;
    bool init;
public:
    swap_files(): init(false) {
        for (int c = 0; c < 4; ++c) {
            open(c);
        }

        if (any_of(files.begin(), files.end(), [](FILE * f) {
            return f == NULL;
        })) {
            init = false;
        }
    }

    void swap(int a, int b) {
        std::swap(files[a], files[b]);
        std::swap(names[a].value, names[b].value);
    }

    void reset(int idx) {
        fclose(files[idx]);
        open(idx);
    }

    FILE * operator[](int idx) {
        return files[idx];
    }

    void unlink(int idx) {
        fclose(files[idx]);
        std::remove(names[idx].value);
    }

    void rename(int idx, const char * name) {
        fclose(files[idx]);
        std::rename(names[idx].value, name);
    }
private:
    void open(int idx) {
        files[idx] = fopen(names[idx].value, "wb+");
        setvbuf(files[idx], NULL, _IONBF, 0);
    }

};

const int stream_buff_size = chunk_size / 2;


class file_iterator {
    FILE * file;
    const int block_size, capacity;
    uint64_t * buff;
    int offset, size, total_read;
public:
    file_iterator(FILE * f, int block_size): buff(NULL), block_size(block_size), file(f), capacity(stream_buff_size) {
        buff = new uint64_t[capacity];
        reset();
    }

    void reset() {
        offset = size = total_read = 0;
        size = fread(buff, sizeof(uint64_t), capacity, file);
    }

    uint64_t peek() const {
        return buff[offset];
    }

    bool done() const {
        return total_read == block_size || eof();
    }

    bool eof() const {
        return feof(file) != 0;
    }

    uint64_t get() {
        uint64_t val = peek();
        ++offset;
        ++total_read;
        if (offset == size && total_read < block_size) {
            size = fread(buff, sizeof(uint64_t), capacity, file);
            offset = 0;
        }
        return val;
    }
};

bool done(const file_iterator & left, const file_iterator & right) {
    return left.done() && right.done();
}

uint64_t get_min(file_iterator & left, file_iterator & right) {
    if (!left.done() && !right.done()) {
        if (custom_cmp(left.peek(), right.peek())) {
            return left.get();
        } else {
            return right.get();
        }
    } else if (!left.done()) {
        return left.get();
    } else {
        return right.get();
    }
}

int merge_files(swap_files & files, const int block_size, uint64_t * reuse_buff, int buff_size)
{
    rewind(files[0]);
    rewind(files[1]);
    files.reset(2);
    files.reset(3);

    file_iterator left(files[0], block_size), right(files[1], block_size);
    int output = 0, total_write = 0;
    int c;
    while(!done(left, right)) {
        
        for (c = 0; c < buff_size && !done(left, right); ++c) {
            reuse_buff[c] = get_min(left, right);
        }
        
        int wrote = fwrite(reuse_buff, sizeof(uint64_t), c, files[2 + (output & 1)]);

        total_write += c;
        
        if (total_write == block_size * 2 ||
            ((!left.eof() || !right.eof()) && done(left, right)) // done with one of the files but more in the other !does not work
        ) {
            total_write = 0;
            ++output;
            left.reset();
            right.reset();
        }
    }
    
    return output > 0;
}

int main ()
{
    FILE * target = fopen("generated.dat", "rb");
    if(!target) {
        cerr << "Failed to open file";
        return -1;
    }

    swap_files files;

    uint64_t * chunk = new uint64_t[chunk_size];
    int read_count = 0, chunk_id = 0;
    do {
        read_count = fread(chunk, sizeof(uint64_t), chunk_size, target);
        sort(chunk, chunk + read_count, custom_cmp);
        fwrite(chunk, sizeof(uint64_t), chunk_size, files[chunk_id++ & 1]);
    }
    while(read_count == chunk_size);
    fclose(target);
    remove("generated.dat");

    int block_size = chunk_size;
    while(merge_files(files, block_size, chunk, chunk_size)) {
        block_size *= 2;
        files.swap(0, 2);
        files.swap(1, 3);
    }

    for (int c = 1; c < 4; ++c) {
        files.unlink(c);
    }
    files.rename(0, "generated.dat");
    return 0;
}
