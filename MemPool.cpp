#include "MemPool.h"

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <sys/mman.h>

#define MEM_POOL_SIZE (1024 * 1024 * 1024) // 1G

static void *MEM_POOL = nullptr;
static size_t CHUNK_SIZE = 0;
static void *AVAIL_PTR = nullptr; // next available

static uint64_t align_up(uint64_t n, uint64_t align) {
  return (n + align - 1) & ~(align - 1);
}

namespace MemPool {
void memInit(size_t chunkSize) {
  assert((MEM_POOL == 0) && "reinit mem pool");
  CHUNK_SIZE = align_up(chunkSize, 64);
  std::cout << "CHUNK_SIZE = " << CHUNK_SIZE << std::endl;
  MEM_POOL = mmap(nullptr, MEM_POOL_SIZE, PROT_READ | PROT_WRITE,
                  MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (MEM_POOL == MAP_FAILED) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }
  AVAIL_PTR = MEM_POOL;
}
void *malloc() {
  assert((uint64_t)AVAIL_PTR < (uint64_t)MEM_POOL + MEM_POOL_SIZE);
  auto p = AVAIL_PTR;
  AVAIL_PTR = reinterpret_cast<void *>(reinterpret_cast<uint64_t>(AVAIL_PTR) +
                                       CHUNK_SIZE);
  return p;
}
size_t indexOf(void *ptr) {
  auto offset =
      reinterpret_cast<uintptr_t>(ptr) - reinterpret_cast<uintptr_t>(MEM_POOL);
  return offset / CHUNK_SIZE;
}

void memDestroy() {
  assert(MEM_POOL && "mem pool not init yet");
  if (munmap(MEM_POOL, MEM_POOL_SIZE)) {
    perror("munmap");
  }
}

} // namespace MemPool