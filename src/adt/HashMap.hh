#pragma once

#include "Array.hh"
#include "String.hh"

namespace adt
{

constexpr f64 HASHMAP_DEFAULT_LOAD_FACTOR = 0.5;

constexpr size_t
fnHash(const size_t& value)
{
    return value;
}

/* FNV */
template<typename CHAR_T, typename ALLOC>
size_t
fnHash(const String<CHAR_T, ALLOC>& str)
{
    size_t hash = 0xCBF29CE484222325;
    for (size_t i = 0; i < str.size; i++)
        hash = (hash ^ static_cast<size_t>(str[i])) * 0x100000001B3;
    return hash;
}

template<typename T>
struct Bucket
{
    T data;
    bool bOccupied = false;
    bool bDeleted = false;
};

template <typename T>
struct HashMapIt
{
    T* pData;
    size_t hash;
    size_t idx;
    bool bInserted;
};

/* simple linear probing */
template<typename T, typename ALLOC = DefaultAllocator>
struct HashMap
{
    f64 maxLoadFactor;
    Array<Bucket<T>, ALLOC> aBuckets;
    size_t bucketCount = 0;

    HashMap(size_t capacity = SIZE_MIN,
            f64 _maxLoadFactor = HASHMAP_DEFAULT_LOAD_FACTOR,
            ALLOC* allocator = &g::StdAllocator)
        : maxLoadFactor(_maxLoadFactor), aBuckets(capacity, allocator) {}

    f64 loadFactor() const { return static_cast<f64>(this->bucketCount) / static_cast<f64>(this->aBuckets.capacity); }
    size_t capacity() const { return this->aBuckets.capacity; }
    HashMapIt<T> insert(const T& value);
    HashMapIt<T> search(const T& value);
    void remove(size_t i);
    HashMapIt<T> tryInsert(const T& value);
    void free() { this->aBuckets.free(); }
};

template<typename T, typename ALLOC>
HashMapIt<T>
HashMap<T, ALLOC>::insert(const T& value)
{
    if (this->loadFactor() >= this->maxLoadFactor)
        this->aBuckets.reallocate(this->capacity() * 2);

    size_t hash = fnHash(value);
    size_t idx = hash % this->capacity();

    while (this->aBuckets[idx].bOccupied)
        if (++idx > this->capacity())
            idx = 0;

    this->aBuckets[idx].data = value;
    this->aBuckets[idx].bOccupied = true;
    this->aBuckets[idx].bDeleted = false;
    this->bucketCount++;

    return {
        .pData = &this->aBuckets[idx].data,
        .hash = hash,
        .idx = idx,
        .bInserted = true
    };
}

template<typename T, typename ALLOC>
HashMapIt<T>
HashMap<T, ALLOC>::search(const T& value)
{
    size_t hash = fnHash(value);
    size_t idx = hash % this->capacity();

    HashMapIt<T> ret;
    ret.hash = hash;
    ret.pData = nullptr;
    ret.bInserted = false;

    while (this->aBuckets[idx].bOccupied || this->aBuckets[idx].bDeleted)
    {
        if (this->aBuckets[idx].data == value)
        {
            ret.pData = &this->aBuckets[idx].data;
            break;
        }

        if (++idx >= this->capacity())
            idx = 0;
    }

    ret.idx = idx;
    return ret;
}

template<typename T, typename ALLOC>
void
HashMap<T, ALLOC>::remove(size_t i)
{
    this->aBuckets[i].bDeleted = true;
    this->aBuckets[i].bOccupied = false;
}

template<typename T, typename ALLOC>
HashMapIt<T>
HashMap<T, ALLOC>::tryInsert(const T& value)
{
    auto f = this->search(value);
    if (f.pData) return f;
    else return this->insert(value);
}

} /* namespace adt */