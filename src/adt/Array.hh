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
    ALLOC& allocator;

    Array(size_t _capacity = SIZE_MIN, ALLOC& _allocator = g::StdAllocator);
    ~Array();

    T& operator[](size_t i) { return this->pData[i]; }

    T* push(const T& data);
    T* back();
    T* front();
};

template<typename T, typename ALLOC>
Array<T, ALLOC>::Array(size_t _capacity, ALLOC& _allocator)
    : capacity(_capacity), allocator(_allocator)
{
    pData = static_cast<T*>(this->allocator.alloc(this->capacity, 1));
}

template<typename T, typename ALLOC>
Array<T, ALLOC>::~Array()
{
    this->allocator.free(this->pData);
}

template<typename T, typename ALLOC>
T*
Array<T, ALLOC>::push(const T& data)
{
    this->pData[this->size++] = data;

    if (this->size >= this->capacity)
        this->pData = static_cast<T*>(this->allocator.realloc(this->pData, sizeof(T) * (this->capacity *= 2)));

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

} /* namespace adt */
