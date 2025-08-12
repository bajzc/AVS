#ifndef MEMPOOL_H
#define MEMPOOL_H
#include <cstddef>
#include <cstdint>
#define MEM_POOL_SIZE (1024 * 1024 * 1024) // 1G

namespace MemPool {
    extern void *MEM_POOL;
    extern size_t CHUNK_SIZE;
    void memInit(std::size_t chunkSize);
    void *malloc();

    inline size_t indexOf(void *ptr) {
        auto offset = reinterpret_cast<uint64_t>(ptr) - reinterpret_cast<uintptr_t>(MEM_POOL);
        return offset / CHUNK_SIZE;
    }

    void memDestroy();
} // namespace MemPool

#endif // MEMPOOL_H
