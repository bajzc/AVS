#ifndef TRAVERSE_H
#define TRAVERSE_H

#include <iostream>

#include "Rectangle.h"
#include "Tree.h"

struct Traverse {
    explicit Traverse(Tree *root) : root(root) {}

    Tree *root;

    virtual void preVisit(Tree *t) {}

    virtual void postVisit(Tree *t) {}

    virtual void preCheck(Tree *t) {}

    virtual void postCheck(Tree *t) {}

    virtual void finish() {}

    void check(Tree *cur = nullptr) {
        if (cur == nullptr) {
            cur = root;
        }
        preCheck(cur);
        memset(cur->data.g1.garbage, 2, sizeof(cur->data.g1.garbage));
        memset(cur->data.g2.garbage, 4, sizeof(cur->data.g2.garbage));
        for (auto *child: cur->children) {
            check(child);
        }
        postCheck(cur);
    }

    void go(Tree *cur = nullptr) {
        if (cur == nullptr) {
            cur = root;
        }
        preVisit(cur);
        memset(cur->data.g1.garbage, 2, sizeof(cur->data.g1.garbage));
        memset(cur->data.g2.garbage, 4, sizeof(cur->data.g2.garbage));
        for (auto *child: cur->children) {
            go(child);
        }
        postVisit(cur);
    }
};

#if AOS

struct TraverseAOS : Traverse {
    TraverseAOS(Tree *root) : Traverse(root) {}

    uint64_t totalIntersectArea = 0;
    uint64_t totalOverlap = 0;

    void preVisit(Tree *t) override {
        std::swap(t->data.colour.blue, t->data.colour.green);
        std::swap(t->data.colour.green, t->data.colour.red);
    }

    void postVisit(Tree *t) override {
        auto rect = t->data.selfRect;
        for (auto *child: t->children) {
            rect = rect.intersectRect(child->data.selfRect);
        }
        t->data.intersectAreaWithChild = rect.area();
        totalIntersectArea += t->data.intersectAreaWithChild;
    }

    void finish() override {
        std::cout << "total intersect area = " << totalIntersectArea << "\n";
        std::cout << "total overlap = " << totalOverlap << "\n";
    }

    void preCheck(Tree *t) override {
        t->data.hasChild = t->children.size();
        if (t->data.hasChild) {
            t->data.hasOverlapWithChild = t->data.intersectAreaWithChild;
            if (t->data.hasOverlapWithChild) {
                auto overlapCounter = 0;
                for (auto *child: t->children) {
                    if (t->data.selfRect.intersectArea(child->data.selfRect)) {
                        child->data.hasOverlapWithParent = true;
                        overlapCounter += 1;
                    }
                }
                t->data.childOverlapsEachOtherAndThis = overlapCounter > 1;
            }
        }
    }

    void postCheck(Tree *t) override {
        t->data.hasOverlap = t->data.hasOverlapWithParent | t->data.hasOverlapWithChild;
        if (t->data.hasOverlap)
            totalOverlap += 1;
        if (t->data.childOverlapsEachOtherAndThis)
            totalOverlap += 2;
        if (t->data.hasOverlap) {
            t->data.param1 = true;
            t->data.param2 = true;
            t->data.param3 = false;
            t->data.param4 = false;
        }else {
            t->data.param1 = false;
            t->data.param2 = false;
            t->data.param3 = true;
            t->data.param4 = true;
        }

    }
};

#else
struct TraverseSOA : Traverse {
    TraverseSOA(Tree *root, GlobalData &data) : Traverse(root), data(data) {}

    GlobalData &data;
    uint64_t totalIntersectArea = 0;
    uint64_t totalOverlap = 0;

    void preVisit(Tree *t) override {
        auto &[red, green, blue] = t->data.colour;
        std::swap(blue, green);
        std::swap(green, red);
    }

    void postVisit(Tree *t) override {
        auto rect = t->data.selfRect;
        for (auto *child: t->children) {
            rect = rect.intersectRect(child->data.selfRect);
        }
        auto area = rect.area();
        data.intersectAreaWithChild[MemPool::indexOf(t)] = area;
        totalIntersectArea += area;
    }

    void finish() override {
        std::cout << "total intersect area = " << totalIntersectArea << "\n";
        std::cout << "total overlap = " << totalOverlap << "\n";
    }

    void preCheck(Tree *t) override {
        auto id = MemPool::indexOf(t);
        data.w_hasChild(id, t->children.size());
        if (data.r_hasChild(id)) {
            data.w_hasOverlapWithChild(id, data.intersectAreaWithChild[id] > 0);
            if (data.r_hasOverlapWithChild(id)) {
                auto overlapCounter = 0;

                for (auto *child: t->children) {
                    auto childId = MemPool::indexOf(child);
                    if (t->data.selfRect.intersectArea(child->data.selfRect)) {
                        data.w_hasOverlapWithParent(childId, true);
                        overlapCounter += 1;
                    }
                }
                data.w_childOverlapsEachOtherAndThis(id, overlapCounter > 1);
            }
        }
    }

    void postCheck(Tree *t) override {
        auto id = MemPool::indexOf(t);
        data.w_hasOverlap(id, data.r_hasOverlapWithParent(id) | data.r_hasOverlapWithChild(id));
        if (data.r_hasOverlap(id))
            totalOverlap += 1;
        if (data.r_childOverlapsEachOtherAndThis(id))
            totalOverlap += 2;
        if (data.hasOverlap[id]) {
            data.param1[id] = true;
            data.param2[id] = true;
            data.param3[id] = false;
            data.param4[id] = false;
        }else {
            data.param1[id] = false;
            data.param2[id] = false;
            data.param3[id] = true;
            data.param4[id] = true;
        }
    }
};
#endif
#endif // TRAVERSE_H
