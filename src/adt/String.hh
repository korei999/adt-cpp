#pragma once

#include <string.h>

#include "Common.hh"
#include "Utils.hh"

namespace adt
{

template<typename CHAR_T = char, typename ALLOC = DefaultAllocator>
struct String
{
    CHAR_T* pData = nullptr;
    size_t size = 0;
    ALLOC* allocator;

    String(ALLOC* _allocator = &g::StdAllocator) : allocator(_allocator) {}
    String(const CHAR_T* sNullTerminated, ALLOC* _allocator = &g::StdAllocator);
    String(const String& other);

    CHAR_T& operator[](size_t i) { return this->pData[i]; }
    const CHAR_T& operator[](size_t i) const { return this->pData[i]; }
    String& operator=(const String& other);
    void free() { this->allocator->free(this->pData); }
};

template<typename CHAR_T, typename ALLOC>
String<CHAR_T, ALLOC>::String(const CHAR_T* sNullTerminated, ALLOC* _allocator)
    : size(strlen(sNullTerminated)), allocator(_allocator)
{
    this->pData = static_cast<CHAR_T*>(this->allocator->alloc(this->size + 1, sizeof(CHAR_T)));
    strncpy(pData, sNullTerminated, this->size);
}

template<typename CHAR_T, typename ALLOC>
String<CHAR_T, ALLOC>::String(const String& other)
    : size(other.size), allocator(other.allocator)
{
    this->pData = static_cast<CHAR_T*>(this->allocator->alloc(this->size + 1, sizeof(CHAR_T)));
    strncpy(this->pData, other.pData, this->size);
}

template<typename CHAR_T, typename ALLOC>
String<CHAR_T, ALLOC>
operator+(const String<CHAR_T, ALLOC>& sL, const String<CHAR_T, ALLOC>& sR)
{
    String<CHAR_T, ALLOC> ret(sL.allocator);

    ret.size = sL.size + sR.size + 1;
    ret.pData = static_cast<CHAR_T*>(ret.allocator->alloc(ret.size, sizeof(CHAR_T)));
    strncpy(ret.pData, sL.pData, sL.size);
    strncpy(&ret.pData[sL.size], sR.pData, sR.size);

    return ret;
}

template<typename CHAR_T, typename ALLOC>
String<CHAR_T, ALLOC>&
operator+=(String<CHAR_T, ALLOC>& sL, const String<CHAR_T, ALLOC>& sR)
{
    size_t newSize = sL.size + sR.size;
    sL.pData = static_cast<CHAR_T*>(sL.allocator->realloc(sL.pData, sizeof(CHAR_T) * (newSize + 1)));
    sL.pData[newSize] = static_cast<CHAR_T>('\0');
    strncpy(&sL.pData[sL.size], sR.pData, sR.size);
    sL.size = newSize;

    return sL;
}

template<typename CHAR_T, typename ALLOC>
bool
operator==(const String<CHAR_T, ALLOC>& sL, const String<CHAR_T, ALLOC>& sR)
{
    return strncmp(sL.pData, sR.pData, min(sL.size, sR.size)) == 0;
}

template<typename CHAR_T, typename ALLOC>
String<CHAR_T, ALLOC>&
String<CHAR_T, ALLOC>::operator=(const String<CHAR_T, ALLOC>& other)
{
    this->allocator = other.allocator;
    this->pData = static_cast<CHAR_T*>(this->allocator->realloc(this->pData, (other.size + 1) * sizeof(CHAR_T)));
    strncpy(pData, other.pData, other.size);
    this->size = other.size;
    this->pData[this->size] = static_cast<CHAR_T>('\0');

    return *this;
}

template<typename CHAR_T>
struct StringView
{
    CHAR_T* pData = nullptr;
    size_t size = 0;

    StringView() = default;
    StringView(CHAR_T* sNullTerminated) : pData(sNullTerminated), size(strlen(sNullTerminated)) {}
    StringView(CHAR_T* sNullTerminated, size_t len) : pData(sNullTerminated), size(len) {}
    StringView(const String<CHAR_T>& s) : pData(s.pData), size(s.size) {}

    CHAR_T& operator[](size_t i) { return this->pData[i]; }
    const CHAR_T& operator[](size_t i) const { return this->pData[i]; }
};

template<typename CHAR_T>
bool
operator==(const StringView<CHAR_T>& sL, const StringView<CHAR_T>& sR)
{
    return strncmp(sL.pData, sR.pData, min(sL.size, sR.size)) == 0;
}

} /* namespace adt */
