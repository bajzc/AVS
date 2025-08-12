#include <random>
#include <iostream>
#include <queue>

#include "Tree.h"
#include "RenderData.h"

#define SEED 1145 // time(nullptr)

std::mt19937 rng(SEED);

inline int randInt(int Min, int Max) {
    std::uniform_int_distribution<int> dist(Min, Max);
    return dist(rng);
}

#ifdef AOS

Tree *createRandomTree(const std::vector<int> &nodePerLevel) {
    if (nodePerLevel.empty()) return nullptr;

    std::uniform_real_distribution<float> colourDist(0, 1);
    std::uniform_int_distribution<> widthDist(0, SCREEN_WIDTH);
    std::uniform_int_distribution<> heightDist(0, SCREEN_HEIGHT);

    int id_counter = 0;
    Tree *root = new Tree(id_counter++);
    root->data.colour = {colourDist(rng), colourDist(rng), colourDist(rng)};
    root->data.selfRect = Rectangle<int>(widthDist(rng), heightDist(rng), widthDist(rng), heightDist(rng));

    std::queue<Tree *> q;
    q.push(root);

    for (size_t level = 0; level < nodePerLevel.size(); ++level) {
        int num_children_per_node = nodePerLevel[level];
        int level_size = q.size(); // nodes at current level

        for (int i = 0; i < level_size; ++i) {
            Tree *parent = q.front();
            q.pop();

            for (int c = 0; c < num_children_per_node; ++c) {
                Tree *child = new Tree(id_counter++);

                child->data.colour = {colourDist(rng), colourDist(rng), colourDist(rng)};
                child->data.selfRect = Rectangle<int>(widthDist(rng), heightDist(rng), widthDist(rng), heightDist(rng));

                parent->children.push_back(child);
                q.push(child);
            }
        }
    }

    return root;
}

#else
Tree* createRandomTree(const std::vector<int> &nodePerLevel, GlobalData& globalData) {
    if (nodePerLevel.empty()) return nullptr;

    std::uniform_real_distribution<float> colourDist(0, 1);
    std::uniform_int_distribution<> widthDist(0, SCREEN_WIDTH);
    std::uniform_int_distribution<> heightDist(0, SCREEN_HEIGHT);

    int id_counter = 0;
    Tree* root = new Tree(id_counter++);

    root->data.colour = {colourDist(rng), colourDist(rng), colourDist(rng)};
    root->data.selfRect = Rectangle<int>(widthDist(rng), heightDist(rng), widthDist(rng), heightDist(rng));
    globalData.address2id[reinterpret_cast<uint64_t>(root)] = root->id;

    std::queue<Tree*> q;
    q.push(root);

    for (size_t level = 0; level < nodePerLevel.size(); ++level) {
        int num_children_per_node = nodePerLevel[level];
        int level_size = q.size(); // nodes at current level

        for (int i = 0; i < level_size; ++i) {
            Tree* parent = q.front(); q.pop();

            for (int c = 0; c < num_children_per_node; ++c) {
                Tree* child = new Tree(id_counter++);

                child->data.colour = {colourDist(rng), colourDist(rng), colourDist(rng)};
                child->data.selfRect = Rectangle<int>(widthDist(rng), heightDist(rng), widthDist(rng), heightDist(rng));
                globalData.address2id[reinterpret_cast<uint64_t>(child)] = child->id;

                parent->children.push_back(child);
                q.push(child);
            }
        }
    }

    return root;
}
#endif

// Print the tree (DFS)
#if AOS

void Tree::printTree(int depth) {
    std::cout << std::string(depth * 2, ' ') << "Node " << this->id;
//    std::cout << std::format(", visible={}, colour=({},{},{})", this->visible, this->data.colour.red,
//                             this->data.colour.green, this->data.colour.blue);
    auto &rect = this->data.selfRect;
//    std::cout << std::format(", selfRect=({}, {}, {}, {})\n", rect.x, rect.y, rect.width, rect.height);
    for (Tree *child: this->children) {
        child->printTree(depth + 1);
    }
}

#else
void Tree::printTree(RenderData& globalData, int depth) {
    std::cout << std::string(depth * 2, ' ') << "Node " << this->id;
    auto &colour = this->data.colour;
//    std::cout << std::format(", visible={}, colour=({},{},{})", this->visible, colour.red, colour.green, colour.blue);
    auto &rect = this->data.selfRect;
//    std::cout << std::format(", selfRect=({}, {}, {}, {})\n", rect.x, rect.y, rect.width, rect.height);
    for (Tree* child : this->children) {
        child->printTree(globalData, depth + 1);
    }
}
#endif
