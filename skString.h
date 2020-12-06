/*
-------------------------------------------------------------------------------

    Copyright (c) Charles Carley.

    Contributor(s): none yet.

-------------------------------------------------------------------------------
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#ifndef _skString_h_
#define _skString_h_

#include "Utils/skAllocator.h"
#include "Utils/skArray.h"
#include "Utils/skMap.h"
#include "Utils/skMemoryUtils.h"

namespace skStringUtils
{
    extern SKsize length(const char* in);
    extern void   copy(char* dest, const char* src);
    extern void   copyn(char* dest, const char* src, SKsize n);
    extern SKsize equals(const char* a, const char* b);
    extern SKsize equalsn(const char* a, const char* b, SKsize n);

    extern bool   toBool(const char* in);
    extern int    toInt(const char* in);
    extern long   toLong(const char* in);
    extern float  toFloat(const char* in);
    extern double toDouble(const char* in);

    extern const SKuint8  HexTable[16];
    extern const SKuint16 BinaryTable[9];
};  // namespace skStringUtils

#define sk_strncpy(dest, size, source, count) skStringUtils::copyn(dest, source, size)

class skString
{
public:
    SK_DECLARE_TYPE(char)

    typedef skPointerIncrementIterator<skString, SKsize>       Iterator;
    typedef const skPointerIncrementIterator<skString, SKsize> ConstIterator;
    typedef skPointerDecrementIterator<skString, SKsize>       ReverseIterator;
    typedef const skPointerDecrementIterator<skString, SKsize> ConstReverseIterator;

    SK_IMPLEMENT_SORT(ValueType, skString, SKsize)

    const SKsize npos = -1;

public:
    skString() :
        m_data(0),
        m_size(0),
        m_capacity(0)
    {
    }

    skString(const ValueType* str, SKsize len = 0) :
        m_data(0),
        m_size(0),
        m_capacity(0)
    {
        alloc(str, len);
    }

    skString(const skString& str) :
        m_data(0),
        m_size(0),
        m_capacity(0)
    {
        alloc(str.c_str(), str.size());
    }

    skString(const char ch, SKsize nr) :
        m_data(0),
        m_size(0),
        m_capacity(0)
    {
        alloc(ch, nr);
    }

    ~skString()
    {
        clear();
    }

    SK_INLINE bool operator==(const skString& rhs) const
    {
        return !skStringUtils::equals(m_data, rhs.m_data);
    }

    SK_INLINE bool operator!=(const skString& rhs) const
    {
        return skStringUtils::equals(m_data, rhs.m_data) != 0;
    }

    SK_INLINE bool operator==(const ValueType* rhs) const
    {
        return !skStringUtils::equals(m_data, rhs);
    }

    SK_INLINE bool operator!=(const ValueType* rhs) const
    {
        return skStringUtils::equals(m_data, rhs) != 0;
    }

    SK_INLINE skString operator+(const char ch) const
    {
        return skString(*this).append(ch);
    }

    SK_INLINE skString operator+(const ValueType* rhs) const
    {
        return skString(*this).append(rhs);
    }

    SK_INLINE skString operator+(const skString& rhs) const
    {
        return skString(*this).append(rhs);
    }

    SK_INLINE skString& operator+=(const char* rhs)
    {
        return append(rhs);
    }

    SK_INLINE skString& operator+=(char rhs)
    {
        return append(rhs);
    }

    SK_INLINE skString& operator+=(const skString& rhs)
    {
        return this->append(rhs);
    }

    SK_INLINE char operator[](SKsize idx) const
    {
        SK_ASSERT(idx != npos);
        return m_data ? m_data[idx] : '\0';
    }

    skString& operator=(const skString& rhs);

    SKsize    find(char ch) const;
    SKsize    find(const char* ch, SKsize offs = 0) const;
    skString& erase(SKsize pos, SKsize nr = 0);
    skString& append(char ch);
    skString& append(const char* rhs, SKsize rhsLen = 0);
    skString  substr(SKsize pos, SKsize nr) const;
    void      substr(skString& dest, SKsize pos, SKsize nr = 0) const;
    void      swap(skString& rhs);
    SKsize    copy(char* arr, SKsize nr, SKsize offs = 0) const;
    void      split(skArray<skString>& dst, const char* op) const;
    void      toBinary(void);
    void      fromBinary(void);
    void      toHex(void);
    void      fromHex(void);
    void      encrypt(const SKbyte* lb, int b1, const SKuint16* ub, int b2);
    void      decrypt(const SKbyte* lb, int b1, const SKuint16* ub, int b2);
    void      encrypt(const char* password);
    void      decrypt(const char* password);
    void      encrypt(void);
    void      decrypt(void);
    void      clear(void);
    void      reserve(SKsize nr);
    void      resize(SKsize nr);
    void      assign(const skString& rhs);
    
    static skString format(const char* fmt, ...);
    static void     format(skString& dst, const char* fmt, ...);

    skString& asHex(void)
    {
        this->toHex();
        return *this;
    }

    skString& append(const skString& rhs)
    {
        return append(rhs.c_str(), rhs.size());
    }

    SK_INLINE ConstPointerType c_str(void) const
    {
        return m_data;
    }

    SK_INLINE PointerType ptr(void)
    {
        return m_data;
    }

    SK_INLINE SKsize size(void) const
    {
        return m_size;
    }

    SK_INLINE SKsize capacity(void) const
    {
        return m_capacity;
    }

    SK_INLINE bool empty(void) const
    {
        return m_size == 0 || !m_data || m_data[0] == 0;
    }

    SK_INLINE char at(SKsize idx) const
    {
        SK_ASSERT(idx != npos);
        return m_data ? m_data[idx] : '\0';
    }

    SK_INLINE Iterator iterator(void)
    {
        return m_data && m_size > 0 ? Iterator(m_data, m_size) : Iterator();
    }

    SK_INLINE ConstIterator iterator(void) const
    {
        return m_data && m_size > 0 ? ConstIterator(m_data, m_size) : ConstIterator();
    }

    SK_INLINE ReverseIterator reverseIterator(void)
    {
        return m_data && m_size > 0 ? ReverseIterator(m_data, m_size) : ReverseIterator();
    }

    SK_INLINE ConstReverseIterator reverseIterator(void) const
    {
        return m_data && m_size > 0 ? ConstReverseIterator(m_data, m_size) : ConstReverseIterator();
    }

    bool toBoolean(void) const
    {
        return skStringUtils::toBool(m_data);
    }

    SKint32 toInteger(void) const
    {
        return skStringUtils::toInt(m_data);
    }

    float toFloat(void) const
    {
        return skStringUtils::toFloat(m_data);
    }

    double toDouble(void) const
    {
        return skStringUtils::toDouble(m_data);
    }

protected:
    PointerType m_data;
    SKsize      m_size, m_capacity;

    void alloc(const char* str, SKsize len);
    void alloc(char p, SKsize len);
    void alloc(SKsize len, const void* p);
};

typedef skArray<skString> skStringArray;

SK_INLINE SKhash skHash(const skString& key)
{
    return skHash(key.c_str());
}

extern int skSprintf(char* dst, int max_size, const char* fmt, ...);

#endif  //_skString_h_
