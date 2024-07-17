#pragma once

#include "Common.hh"

namespace adt
{

template<typename T, typename ALLOC = DefaultAllocator>
struct Array
{
    T* pData = nullptr;
    size_t size = 0;
    size_t capacity = 0;
    ALLOC* allocator;

    Array(size_t _capacity = SIZE_MIN, ALLOC* _allocator = &g::StdAllocator);

    T& operator[](size_t i) { return this->pData[i]; }

    T* push(const T& data);
    T* back();
    T* front();
    void reallocate(size_t _size);
    void free() { this->allocator->free(this->pData); }
};

template<typename T, typename ALLOC>
Array<T, ALLOC>::Array(size_t _capacity, ALLOC* _allocator)
    : capacity(_capacity), allocator(_allocator)
{
    pData = static_cast<T*>(this->allocator->alloc(this->capacity, sizeof(T)));
}

template<typename T, typename ALLOC>
T*
Array<T, ALLOC>::push(const T& data)
{
    this->pData[this->size++] = data;

    if (this->size >= this->capacity)
        this->reallocate(this->capacity * 2);

    memset(&this->pData[this->size], 0, sizeof(T) * (this->capacity - this->size));

    return this->back();
}

template<typename T, typename ALLOC>
T*
Array<T, ALLOC>::back()
{
    return &this->pData[this->size - 1];
}

template<typename T, typename ALLOC>
T*
Array<T, ALLOC>::front()
{
    return &this->pData[0];
}

template<typename T, typename ALLOC>
void
Array<T, ALLOC>::reallocate(size_t _size)
{
    this->capacity = _size;
    this->pData = static_cast<T*>(this->allocator->realloc(this->pData, sizeof(T) * _size));
}

} /* namespace adt */