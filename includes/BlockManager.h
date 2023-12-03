#ifndef BLOCK_MANAGER_H
#define BLOCK_MANAGER_H

#include <vector>

#include "Block.h"


/* Class Block manager used to manage the creation and the deletion of the Block objects */
class BlockManager {
public:
    BlockManager() = default;
    ~BlockManager() { clear_memory(); }

    /* Deletion of the copy constructor and the assignment operator to avoid pointers ownership errors */
    BlockManager(const BlockManager& other) = delete;
    BlockManager& operator=(const BlockManager& other) = delete;

    Block* makeBlock(std::vector<Statement*> stmts) {
        Block* blk = new Block(stmts);
        blks_allocated.push_back(blk);
        return blk;
    }

    void clear_memory() {
        auto i = blks_allocated.begin();
        for (; i != blks_allocated.end(); i++) {
            delete(*i);
        }
        blks_allocated.resize(0);
    }

private:
    std::vector<Block*> blks_allocated;
};

#endif /* BLOCK_MANAGER_H */