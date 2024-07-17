#pragma once

#include <string.h>

#include "Common.hh"

namespace adt
{

template<typename CHAR_T = char, typename ALLOC = DefaultAllocator>
struct String
{
    CHAR_T* sData = nullptr;
    size_t size = 0;
    ALLOC& allocator;

    String(ALLOC& _allocator = g::StdAllocator) : allocator(_allocator) {}
    String(const CHAR_T* sNullTerminated, ALLOC& _allocator = g::StdAllocator);
    ~String() { this->allocator.free(this->sData); }

    CHAR_T& operator[](size_t i) { return this->sData[i]; }
};

template<typename CHAR_T, typename ALLOC>
String<CHAR_T, ALLOC>::String(const CHAR_T* sNullTerminated, ALLOC& _allocator)
    : size(strlen(sNullTerminated)), allocator(_allocator)
{
    this->sData = static_cast<CHAR_T*>(this->allocator.alloc(this->size + 1, sizeof(CHAR_T)));
    strncpy(sData, sNullTerminated, this->size);
}

template<typename CHAR_T, typename ALLOC>
String<CHAR_T, ALLOC>
operator+(const String<CHAR_T, ALLOC>& sL, const String<CHAR_T, ALLOC>& sR)
{
    String<CHAR_T, ALLOC> ret(sL.allocator);

    ret.size = sL.size + sR.size + 1;
    ret.sData = static_cast<CHAR_T*>(ret.allocator.alloc(ret.size, sizeof(CHAR_T)));
    strncpy(ret.sData, sL.sData, sL.size);
    strncpy(&ret.sData[sL.size], sR.sData, sR.size);

    return ret;
}

template<typename CHAR_T, typename ALLOC>
String<CHAR_T, ALLOC>&
operator+=(String<CHAR_T, ALLOC>& sL, const String<CHAR_T, ALLOC>& sR)
{
    size_t newSize = sL.size + sR.size + 1;
    sL.sData = static_cast<CHAR_T*>(sL.allocator.realloc(sL.sData, sizeof(CHAR_T) * newSize));
    sL.sData[newSize - 1] = static_cast<CHAR_T>('\0');
    strncpy(&sL.sData[sL.size], sR.sData, sR.size);
    sL.size = newSize;

    return sL;
}

template<typename CHAR_T>
struct StringView
{
    CHAR_T* sData = nullptr;
    size_t size = 0;

    StringView() = default;
    StringView(char* sNullTerminated) : sData(sNullTerminated), size(strlen(sNullTerminated)) {}
    StringView(char* sNullTerminated, size_t len) : sData(sNullTerminated), size(len) {}
    StringView(String<CHAR_T> s) : sData(s.sData), size(s.size) {}
};

} /* namespace adt */
