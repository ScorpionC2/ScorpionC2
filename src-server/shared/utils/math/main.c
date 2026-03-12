#include "main.h"

static inline uint32_t rotl(const uint32_t x, const int r) {
    return (x << (r & 31)) | (x >> (32 - (r & 31)));

}

static inline uint32_t rotr(const uint32_t x, const int r) {
    return (x >> (r & 31)) | (x << (32 - (r & 31)));

}