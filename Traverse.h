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
        // cur->data.g3.r();
        for (auto *child: cur->children) {
            check(child);
        }
        postCheck(cur);
        cur->data.g2.r();
    }

    void go(Tree *cur = nullptr) {
        if (cur == nullptr) {
            cur = root;
        }
        preVisit(cur);
        for (auto *child: cur->children) {
            go(child);
        }
        postVisit(cur);
        cur->data.g3.r();
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
        t->data.hasOverlap = t->data.hasOverlapWithParent | t->data.hasOverlapWithChild;
    }

    void postCheck(Tree *t) override {
        if(t->data.hasOverlap){
            t->data.hasOverlap = false;
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
        // auto id = t->id;
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
        data.w_hasOverlap(id, data.r_hasOverlapWithParent(id) | data.r_hasOverlapWithChild(id));
    }

    void postCheck(Tree *t) override {
        __builtin_prefetch(&data.hasOverlap, 1, 3);
        auto id = MemPool::indexOf(t);
        if(data.r_hasOverlap(id)){
            data.w_hasOverlap(id, false);
            
        }
    }
};
#endif
#endif // TRAVERSE_H
