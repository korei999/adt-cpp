#pragma once

#include <math.h>
#include <string.h>
#include <stddef.h>

#include "Common.hh"

#define ARENA_FIRST(A) ((A)->pBlocks)
#define ARENA_NEXT(AB) ((AB)->pNext)
#define ARENA_FOREACH(A, IT) for (ArenaBlock* (IT) = ARENA_FIRST(A); (IT); (IT) = ARENA_NEXT(IT))
#define ARENA_FOREACH_SAFE(A, IT, TMP) for (ArenaBlock* (IT) = ARENA_FIRST(A), * (TMP) = nullptr; (IT) && ((TMP) = ARENA_NEXT(IT), true); (IT) = (TMP))

#define ARENA_NODE_GET_FROM_DATA(PDATA) (reinterpret_cast<adt::ArenaNode*>(reinterpret_cast<u8*>(PDATA) - offsetof(adt::ArenaNode, pData)))
#define ARENA_NODE_GET_FROM_BLOCK(PBLOCK) (reinterpret_cast<adt::ArenaNode*>(reinterpret_cast<u8*>(PBLOCK) + offsetof(adt::ArenaBlock, pData)))

namespace adt
{

struct ArenaBlock
{
    ArenaBlock* pNext = nullptr;
    u8 pData[];
};

struct ArenaNode
{
    ArenaNode* pNext = nullptr;
    ArenaBlock* pBlock = nullptr;
    size_t size = 0;
    u8 pData[]; /* flexible array member */
};

struct Arena : __BaseAllocator
{
    ArenaBlock* pBlocks = nullptr;
    size_t blockSize = 0;

    Arena(size_t cap);
    ~Arena();

    void reset();
    size_t alignedBytes(size_t bytes);
    void* alloc(size_t memberCount, size_t size);
    void free(void* p);
    void* realloc(void* p, size_t size);

private:
    ArenaBlock* newBlock();
    bool fitsNode(ArenaNode* pNode, size_t size);
    ArenaBlock* getFreeBlock();
};

Arena::Arena(size_t cap)
    : blockSize(alignedBytes(cap + sizeof(ArenaNode))) /* preventively align */
{
    this->newBlock();
}

Arena::~Arena()
{
    ARENA_FOREACH_SAFE(this, it, tmp)
        ::free(it);
}

void
Arena::reset()
{
    ARENA_FOREACH(this, pB)
    {
        ArenaNode* pNode = ARENA_NODE_GET_FROM_BLOCK(pB), * pNext = nullptr;

        while (pNode->pNext)
        {
            pNext = pNode->pNext;
            pNode->size = 0;
            pNode->pNext = nullptr;
            pNode = pNext;
        }
    }
}

ArenaBlock*
Arena::newBlock()
{
    ArenaBlock** ppLastBlock = &this->pBlocks;
    while (*ppLastBlock) ppLastBlock = &((*ppLastBlock)->pNext);

    *ppLastBlock = reinterpret_cast<ArenaBlock*>(malloc(sizeof(ArenaBlock) + this->blockSize));
    memset(*ppLastBlock, 0, sizeof(ArenaBlock) + this->blockSize);

    return *ppLastBlock;
}

bool
Arena::fitsNode(ArenaNode* pNode, size_t size)
{
    /* TODO: get max contiguous space if we get bunch of freed nodes in the row, while not overflowing */

    /*size_t maxSize = 0;*/
    /*auto* pB = ARENA_NODE_GET_FROM_BLOCK(pNode->pBlock);*/

    /*while (pNode->size == 0)*/
    /*{*/
    /*    if (!pNode->pNext)*/
    /*    {*/
    /*        maxSize += this->blockSize - (reinterpret_cast<u8*>(pNode) - reinterpret_cast<u8*>(pB));*/
    /*        break;*/
    /*    }*/
    /*    else*/
    /*    {*/
    /*        maxSize += reinterpret_cast<u8*>(pNode) - reinterpret_cast<u8*>(pB);*/
    /*    }*/
    /*}*/

    return static_cast<size_t>(reinterpret_cast<u8*>(pNode->pNext) - reinterpret_cast<u8*>(pNode)) > size;
}

ArenaBlock*
Arena::getFreeBlock()
{
    ArenaBlock* pCurrBlock = this->pBlocks, * pFreeBlock = this->pBlocks;
    while (pCurrBlock)
    {
        pFreeBlock = pCurrBlock;
        pCurrBlock = pCurrBlock->pNext;
    }

    return pFreeBlock;
}

size_t
Arena::alignedBytes(size_t bytes)
{
    f64 mulOf = static_cast<f64>(bytes) / static_cast<f64>(sizeof(size_t));
    return sizeof(size_t) * ceil(mulOf);
}

void*
Arena::alloc(size_t memberCount, size_t memberSize)
{
    auto* pFreeBlock = this->getFreeBlock();

    auto requested = memberCount * memberSize;
    auto aligned = this->alignedBytes(requested + sizeof(ArenaNode));
    assert(aligned <= this->blockSize && "requested size is > than one block");

repeat:
    if (!pFreeBlock) pFreeBlock = this->newBlock();

    /* skip pNext */
    auto* pFreeBlockOff = ARENA_NODE_GET_FROM_BLOCK(pFreeBlock);

    /* find node with pNext == nullptr, this one is free to allocate */
    ArenaNode* pNode = pFreeBlockOff;
    while (pNode->pNext)
    {
        /* if next node exist but we can fit */
        if (pNode->size == 0 && pNode->pNext && static_cast<size_t>(reinterpret_cast<u8*>(pNode->pNext) - reinterpret_cast<u8*>(pNode)) > requested)
            break;

        /*if (pNode->size == 0 && this->fitsNode(pNode->pNext, aligned))*/
        /*    break;*/

        pNode = pNode->pNext;

    }

    /* cast to u8* to get correct byte offsets */
    size_t nextAligned = (reinterpret_cast<u8*>(pNode) + aligned) - reinterpret_cast<u8*>(pFreeBlockOff);

    /* heap overflow */
    if (nextAligned >= this->blockSize)
    {
        pFreeBlock = pFreeBlock->pNext;
        goto repeat;
    }

    pNode->pNext = reinterpret_cast<ArenaNode*>(reinterpret_cast<u8*>(pNode) + aligned);
    pNode->size = requested;
    pNode->pBlock = pFreeBlock;

    return &pNode->pData;
}

void
Arena::free(void* p)
{
    ArenaNode* pNode = ARENA_NODE_GET_FROM_DATA(p);
    pNode->size = 0;
}

void*
Arena::realloc(void* p, size_t size)
{
    ArenaNode* pNode = ARENA_NODE_GET_FROM_DATA(p);
    auto aligned = alignedBytes(size);

    if (size <= pNode->size || this->fitsNode(pNode, aligned))
    {
        pNode->size = size;
        return &pNode->pData;
    }
    else
    {
        void* pR = this->alloc(1, size);
        memcpy(pR, p, pNode->size);
        pNode->size = 0;
        return pR;
    }
}

} /* namespace adt */
