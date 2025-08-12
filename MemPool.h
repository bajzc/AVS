#ifndef MEMPOOL_H
#define MEMPOOL_H
#include <cstddef>

namespace MemPool {
void memInit(std::size_t chunkSize);
void* malloc();
std::size_t indexOf(void* ptr);
void memDestroy();
}

#endif //MEMPOOL_H
