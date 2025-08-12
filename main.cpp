#include "MemPool.h"
#include "Traverse.h"
#include "Tree.h"

int main() {
    std::vector<int> nodePerLevel = {1, 2, 1, 1, 1, 2, 2, 2, 2, 2, 2, 1};

    long N = 1;
    long last = nodePerLevel[0];
    for (auto i: nodePerLevel) {
        N += last * i;
        last *= i;
    }
    std::cout << "total node number = " << N << "\n";

    GlobalData data;
#if AOS
    Tree *root = createRandomTree(nodePerLevel);
//    root->printTree();
    TraverseAOS visitor(root);
#endif
#if SOA
    MemPool::memInit(sizeof(Tree));
    assert(N * MemPool::CHUNK_SIZE < MEM_POOL_SIZE);
    Tree *root = createRandomTree(nodePerLevel, data);
    data.intersectAreaWithChild.resize(N);
    data.hasOverlap.resize(N);
    data.hasChild.resize(N);
    data.hasOverlapWithChild.resize(N);
    data.hasOverlapWithParent.resize(N);
    data.childOverlapsEachOtherAndThis.resize(N);
    data.params.resize(N * 4);

    // root->printTree(data);
    TraverseSOA visitor(root, data);
#endif
    visitor.go();

    visitor.check();

    //    root->printTree();

    visitor.finish();
#if SOA
    MemPool::memDestroy();
#endif
}
