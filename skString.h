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
#include "Utils/skChar.h"
#include "Utils/skMap.h"

using skStringConverter = skChar;

class skString
{
public:
    SK_DECLARE_TYPE(char)

    typedef skPointerIncrementIterator<skString, SKsize>       Iterator;
    typedef const skPointerIncrementIterator<skString, SKsize> ConstIterator;
    typedef skPointerDecrementIterator<skString, SKsize>       ReverseIterator;
    typedef const skPointerDecrementIterator<skString, SKsize> ConstReverseIterator;

    SK_IMPLEMENT_SORT(ValueType, skString, SKsize)
    static const SKsize npos;

    [[deprecated("Avoid at exit deconstruction calls.")]] static const skString Blank;

protected:
    PointerType m_data;
    SKsize      m_size;
    SKsize      m_capacity;

    void alloc(const char* str, SKsize len);

    void alloc(char p, SKsize len);

    void alloc(SKsize len, const void* p);

public:
    skString();

    skString(const ValueType* str, SKsize len = 0);
    skString(const skString& str);
    skString(char ch, SKsize nr);

    ~skString();

    bool operator==(const skString& rhs) const
    {
        return !skChar::equals(m_data, rhs.m_data);
    }

    bool operator!=(const skString& rhs) const
    {
        return skChar::equals(m_data, rhs.m_data) != 0;
    }

    bool operator==(const ValueType* rhs) const
    {
        return !skChar::equals(m_data, rhs);
    }

    bool operator!=(const ValueType* rhs) const
    {
        return skChar::equals(m_data, rhs) != 0;
    }

    skString operator+(const char ch) const
    {
        return skString(*this).append(ch);
    }

    skString operator+(const ValueType* rhs) const
    {
        return skString(*this).append(rhs);
    }

    skString operator+(const skString& rhs) const
    {
        return skString(*this).append(rhs);
    }

    skString& operator+=(const char* rhs)
    {
        return append(rhs);
    }

    skString& operator+=(char rhs)
    {
        return append(rhs);
    }

    skString& operator+=(const skString& rhs)
    {
        return this->append(rhs);
    }

    char operator[](const SKsize idx) const
    {
        if (idx < m_size)
            return m_data ? m_data[idx] : '\0';
        return 0;
    }

    skString& operator=(const skString& rhs);

    SKsize find(char ch) const;

    SKsize find(const char* ch, SKsize offs = 0) const;

    skString& erase(SKsize pos, SKsize nr = 0);

    skString& append(char ch);

    skString& append(const char* rhs, SKsize rhsLen = 0);

    skString substr(SKsize pos, SKsize nr) const;

    void substr(skString& dest, SKsize pos, SKsize nr = 0) const;

    void swap(skString& rhs) noexcept;

    SKsize copy(char* arr, SKsize nr, SKsize offs = 0) const;

    void split(skArray<skString>& dst, const char* op) const;

    void split(skArray<skString>& dst, char op) const;

    void toBinary();

    void fromBinary();

    void toHex();

    void fromHex();

    void clear();

    void reserve(SKsize nr);

    void resize(SKsize nr);

    void assign(const skString& rhs);

    bool equals(const skString& rhs) const;

    bool equals(const char* rhs) const;

    bool endsWith(const char* str) const;

    bool startsWith(const char* str) const;

    static skString format(const char* fmt, ...);

    static void format(skString& dst, const char* fmt, ...);

    /// <summary>
    /// Loads the contents of the file into a string.
    /// </summary>
    static skString fromFile(const char* path);

    /// <summary>
    /// Loads the contents of the file into a string.
    /// </summary>
    static void fromFile(skString& dest, const char* path);

    skString& asHex()
    {
        this->toHex();
        return *this;
    }

    skString& append(const skString& rhs)
    {
        return append(rhs.c_str(), rhs.size());
    }

    ConstPointerType c_str() const
    {
        return m_data;
    }

    PointerType ptr() const
    {
        return m_data;
    }

    SKsize size() const
    {
        return m_size;
    }

    SKsize capacity() const
    {
        return m_capacity;
    }

    bool empty() const
    {
        return m_size == 0 || !m_data || m_data[0] == 0;
    }

    char at(const SKsize idx) const
    {
        SK_ASSERT(idx != npos);
        return m_data && idx < m_size ? m_data[idx] : '\0';
    }

    Iterator iterator()
    {
        return m_data && m_size > 0 ? Iterator(m_data, m_size) : Iterator();
    }

    ConstIterator iterator() const
    {
        return m_data && m_size > 0 ? ConstIterator(m_data, m_size) : ConstIterator();
    }

    ReverseIterator reverseIterator()
    {
        return m_data && m_size > 0 ? ReverseIterator(m_data, m_size) : ReverseIterator();
    }

    ConstReverseIterator reverseIterator() const
    {
        return m_data && m_size > 0 ? ConstReverseIterator(m_data, m_size) : ConstReverseIterator();
    }

    bool toBoolean() const
    {
        return skChar::toBool(m_data);
    }

    [[deprecated]] SKint32 toInteger() const
    {
        return skChar::toInt32(m_data);
    }

    SKint16 toInt16(SKint16 def = -1) const
    {
        return skChar::toInt16(m_data, def);
    }

    SKint32 toInt32(SKint32 def = -1) const
    {
        return skChar::toInt32(m_data, def);
    }

    SKint64 toInt64(SKint64 def = -1) const
    {
        return skChar::toInt64(m_data, def);
    }

    SKuint16 toUint16(SKuint16 def = SK_NPOS16) const
    {
        return skChar::toUint16(m_data, def);
    }

    SKuint32 toUint32(SKuint32 def = SK_NPOS32) const
    {
        return skChar::toUint32(m_data, def);
    }

    SKuint64 toUint64(SKuint64 def = (SKuint64)-1) const
    {
        return skChar::toUint64(m_data, def);
    }

    float toFloat(float def = 0.f) const
    {
        return skChar::toFloat(m_data, def);
    }

    double toDouble(double def = 0.0) const
    {
        return skChar::toDouble(m_data, def);
    }
};

typedef skArray<skString> skStringArray;

inline SKhash skHash(const skString& key)
{
    return skHash(key.c_str());
}

extern int skSprintf(char* dst, int maxSize, const char* fmt, ...);

#endif  //_skString_h_
