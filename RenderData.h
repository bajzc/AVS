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
    void r() {
        uint8_t sum = 0;
        for (int i = 0; i < N; i++) {
            sum += garbage[i];
        }
        for (int i = 0; i < N; i++) {
            garbage[i] = sum;
        }
    }
};

#define CACHE_SIZE 512
#define ELEMENT_SIZE 32
#define VECTOR_SIZE (CACHE_SIZE * 4 / ELEMENT_SIZE)
#define LIKELY(x) __builtin_expect(!!(x), 1)

struct RenderData {
    Colour colour;
    PlaceHolder<1024> g1;
    Rectangle<int> selfRect;
#if AOS
    int intersectAreaWithChild;
    bool hasOverlap;
    bool hasChild;
    PlaceHolder<512> g5;
    bool hasOverlapWithChild;
    bool hasOverlapWithParent;
    bool childOverlapsEachOtherAndThis; // children And This
#endif
    PlaceHolder<1024 * 128> g2;
    bool param1, param2, param3, param4;
    PlaceHolder<1024 * 1024> g3;
};

struct alignas(64) GlobalData {
    std::bitset<512> hasOverlap;
    std::bitset<512> hasChild;
    std::bitset<512> hasOverlapWithChild;
    std::bitset<512> hasOverlapWithParent;
    std::bitset<512> childOverlapsEachOtherAndThis;

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
