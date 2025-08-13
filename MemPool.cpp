#include "MemPool.h"

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <sys/mman.h>


namespace MemPool {
    void *MEM_POOL = nullptr;
    static void *AVAIL_PTR = nullptr; // next available

    void memInit() {
        assert((MEM_POOL == 0) && "reinit mem pool");
        std::cout << "CHUNK_SIZE = " << CHUNK_SIZE << std::endl;
        MEM_POOL = mmap((void*)MEM_POOL_START, MEM_POOL_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        assert(MEM_POOL == MEM_POOL_START);
        if (MEM_POOL == MAP_FAILED) {
            perror("mmap");
            exit(EXIT_FAILURE);
        }
        AVAIL_PTR = MEM_POOL;
    }

    void *malloc() {
        // assert((uint64_t)AVAIL_PTR < (uint64_t)MEM_POOL + MEM_POOL_SIZE);
        auto p = AVAIL_PTR;
        AVAIL_PTR = reinterpret_cast<void *>(reinterpret_cast<uint64_t>(AVAIL_PTR) + CHUNK_SIZE);
        return p;
    }

    void memDestroy() {
        assert(MEM_POOL && "mem pool not init yet");
        if (munmap(MEM_POOL, MEM_POOL_SIZE)) {
            perror("munmap");
        }
    }
} // namespace MemPool
