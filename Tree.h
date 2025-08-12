#ifndef TREE_H
#define TREE_H

#include <vector>

#include "RenderData.h"

struct Tree {
    explicit Tree(int _id) : id(_id) {
    }

    ~Tree() = default;

    int id;
    bool visible = true;

    std::vector<Tree *> children;
    RenderData data;

#ifdef AOS

    void printTree(int depth = 0);

#endif

    void printTree(RenderData &globalData, int depth = 0);
};

#ifdef AOS

Tree *createRandomTree(const std::vector<int> &nodePerLevel);

#else
Tree* createRandomTree(const std::vector<int> &nodePerLevel, GlobalData& globalData);
#endif

#endif //TREE_H
