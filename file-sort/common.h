#include <stdint.h>

const int chunk_size = 1024 * 1024 * 64;

inline bool custom_cmp(const uint64_t left, const uint64_t right)
{
    return left < right;
}

inline uint64_t custom_min(const uint64_t left, const uint64_t right) {
    return custom_cmp(left, right) ? left : right;
}
