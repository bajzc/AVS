#ifndef DATA_H
#define DATA_H

#include <bitset>
#include <boost/dynamic_bitset/dynamic_bitset.hpp>
#include <random>

#include "Colour.h"
#include "Rectangle.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

template<std::size_t N>
struct PlaceHolder {
    char garbage[N];
    inline void r() {
        uint8_t sum = 0;
        for (int i = 0; i < N; i++) {
            sum += garbage[i];
        }
        memset(garbage, sum, sizeof(garbage));
    }
};

#define CACHE_SIZE 512
#define ELEMENT_SIZE 32
#define VECTOR_SIZE (CACHE_SIZE * 4 / ELEMENT_SIZE)
#define LIKELY(x) __builtin_expect(!!(x), 1)

struct alignas(64) AlignedBitset512 {
    std::bitset<CACHE_SIZE> data;
};

struct RenderData {
    Colour colour;
    PlaceHolder<1024> g1;
    Rectangle<int> selfRect;
#if AOS
    int intersectAreaWithChild;
    bool hasOverlap;
    bool hasChild;
    bool hasOverlapWithChild;
    bool hasOverlapWithParent;
    bool childOverlapsEachOtherAndThis; // children And This
#endif
    bool param1, param2, param3, param4;
    PlaceHolder<1024> g2;
    PlaceHolder<1024*8> g3;
};

struct GlobalData {
    boost::dynamic_bitset<> hasOverlap;
    boost::dynamic_bitset<> hasChild;
    boost::dynamic_bitset<> hasOverlapWithChild;
    boost::dynamic_bitset<> hasOverlapWithParent;
    boost::dynamic_bitset<> childOverlapsEachOtherAndThis;
    boost::dynamic_bitset<> params;

    std::vector<int> intersectAreaWithChild;
#define DEFINE_BITSET_ACCESSORS(NAME)                                                                                  \
    inline bool r_##NAME(int i) { return NAME[i]; }                                                                    \
    inline void w_##NAME(int i, bool v) { NAME[i] = v; }

    DEFINE_BITSET_ACCESSORS(hasOverlap)
    DEFINE_BITSET_ACCESSORS(hasChild)
    DEFINE_BITSET_ACCESSORS(hasOverlapWithChild)
    DEFINE_BITSET_ACCESSORS(hasOverlapWithParent)
    DEFINE_BITSET_ACCESSORS(childOverlapsEachOtherAndThis)
    inline void pinCache() {
        __builtin_prefetch(&hasOverlap);
        __builtin_prefetch(&hasChild);
        __builtin_prefetch(&hasOverlapWithChild);
        __builtin_prefetch(&hasOverlapWithParent);
        __builtin_prefetch(&childOverlapsEachOtherAndThis);
    }
};

#endif // DATA_H
