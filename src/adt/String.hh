#pragma once

#include <string.h>

#include "Common.hh"
#include "Utils.hh"

namespace adt
{

template<typename ALLOC = DefaultAllocator>
struct String
{
    char* pData = nullptr;
    size_t size = 0;
    ALLOC* allocator;

    String(ALLOC* _allocator = &g::StdAllocator) : allocator(_allocator) {}
    String(const char* sNullTerminated, ALLOC* _allocator = &g::StdAllocator);
    String(const String& other);

    char& operator[](size_t i) { return this->pData[i]; }
    const char& operator[](size_t i) const { return this->pData[i]; }
    void free() { this->allocator->free(this->pData); }
};

template<typename ALLOC>
String<ALLOC>::String(const char* sNullTerminated, ALLOC* _allocator)
    : size(strlen(sNullTerminated)), allocator(_allocator)
{
    this->pData = static_cast<char*>(this->allocator->alloc(this->size + 1, sizeof(char)));
    strncpy(pData, sNullTerminated, this->size);
}

template<typename ALLOC>
String<ALLOC>::String(const String& other)
    : size(other.size), allocator(other.allocator)
{
    this->pData = static_cast<char*>(this->allocator->alloc(this->size + 1, sizeof(char)));
    strncpy(this->pData, other.pData, this->size);
}

template<typename CHAR_T, typename ALLOC>
String<ALLOC>
operator+(const String<ALLOC>& sL, const String<ALLOC>& sR)
{
    String<ALLOC> ret(sL.allocator);

    ret.size = sL.size + sR.size + 1;
    ret.pData = static_cast<CHAR_T*>(ret.allocator->alloc(ret.size, sizeof(CHAR_T)));
    strncpy(ret.pData, sL.pData, sL.size);
    strncpy(&ret.pData[sL.size], sR.pData, sR.size);

    return ret;
}

template<typename ALLOC>
String<ALLOC>&
operator+=(String<ALLOC>& sL, const String<ALLOC>& sR)
{
    size_t newSize = sL.size + sR.size;
    sL.pData = static_cast<char*>(sL.allocator->realloc(sL.pData, sizeof(char) * (newSize + 1)));
    sL.pData[newSize] = '\0';
    strncpy(&sL.pData[sL.size], sR.pData, sR.size);
    sL.size = newSize;

    return sL;
}

template<typename ALLOC>
bool
operator==(const String<ALLOC>& sL, const String<ALLOC>& sR)
{
    return strncmp(sL.pData, sR.pData, min(sL.size, sR.size)) == 0;
}

struct StringView
{
    char* pData = nullptr;
    size_t size = 0;

    StringView() = default;
    StringView(char* sNullTerminated) : pData(sNullTerminated), size(strlen(sNullTerminated)) {}
    StringView(const char* sNullTerminated) : pData(const_cast<char*>(sNullTerminated)), size(strlen(sNullTerminated)) {}
    StringView(char* sNullTerminated, size_t len) : pData(sNullTerminated), size(len) {}
    template<typename ALLOC> StringView(const String<ALLOC>& s) : pData(s.pData), size(s.size) {}

    char& operator[](size_t i) { return this->pData[i]; }
    const char& operator[](size_t i) const { return this->pData[i]; }
};

static inline bool
operator==(const StringView& sL, const StringView& sR)
{
    return strncmp(sL.pData, sR.pData, min(sL.size, sR.size)) == 0;
}

} /* namespace adt */
