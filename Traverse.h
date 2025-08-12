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
                for (auto *child: t->children) {
                    if (t->data.selfRect.intersectArea(child->data.selfRect))
                        child->data.hasOverlapWithParent = true;
                }
                auto overlapCounter = 0;
                for (auto *child: t->children) {
                    if (child->data.hasOverlapWithParent) {
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
        t->data.intersectAreaWithChild= rect.area();
        totalIntersectArea += t->data.intersectAreaWithChild;
    }

    void finish() override {
        std::cout << "total intersect area = " << totalIntersectArea << "\n";
        std::cout << "total overlap = " << totalOverlap << "\n";
    }

    void preCheck(Tree *t) override {
        data.w_hasChild(t->id, t->children.size());
        if (data.r_hasChild(t->id)) {
            data.w_hasOverlapWithChild(t->id, t->data.intersectAreaWithChild> 0);
            if (data.r_hasOverlapWithChild(t->id))
                for (auto *child: t->children) {
                    if (t->data.selfRect.intersectArea(child->data.selfRect))
                        data.w_hasOverlapWithParent(child->id, true);
                }
            auto overlapCounter = 0;
            for (auto *child: t->children) {
                // auto child_id = child->id;
                auto child_id = MemPool::indexOf(child);
                if (data.r_hasOverlapWithParent(child_id)) {
                    overlapCounter += 1;
                }
            }
            data.w_childOverlapsEachOtherAndThis(t->id, overlapCounter > 1);
        }
    }


    void postCheck(Tree *t) override {
        data.w_hasOverlap(t->id, data.r_hasOverlapWithParent(t->id) | data.r_hasOverlapWithChild(t->id));
        if (data.r_hasOverlap(t->id))
            totalOverlap += 1;
        if (data.r_childOverlapsEachOtherAndThis(t->id))
            totalOverlap += 2;
    }
};
#endif
#endif // TRAVERSE_H
