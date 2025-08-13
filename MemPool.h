#ifndef MEMPOOL_H
#define MEMPOOL_H
#include <cstddef>
#include <cstdint>
#include "RenderData.h"
#define MEM_POOL_SIZE (1024 * 1024 * 1024) // 1G
#define MEM_POOL_START 0x1145000000
constexpr uint64_t align_up(uint64_t n, uint64_t align) { return (n + align - 1) & ~(align - 1); }

namespace MemPool {
    constexpr size_t CHUNK_SIZE = align_up(sizeof(struct RenderData), 64);
    extern void *MEM_POOL;
    void memInit();
    void *malloc();

    inline size_t indexOf(void *ptr) {
        auto offset = reinterpret_cast<uint64_t>(ptr) - MEM_POOL_START;
        return offset / CHUNK_SIZE;
    }

    void memDestroy();
} // namespace MemPool

#endif // MEMPOOL_H
