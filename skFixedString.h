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
#ifndef _skFixedString_h_
#define _skFixedString_h_

#include "Utils/Config/skConfig.h"
#include "Utils/skArray.h"
#include "Utils/skChar.h"
#include "Utils/skHash.h"

template <const SKuint16 L>
class skFixedString
{
public:
    typedef char Pointer[L + 1];

public:
    skFixedString() :
        m_size(0),
        m_hash(-1)
    {
        m_buffer[m_size] = 0;
    }

    skFixedString(const skFixedString& rhs) :
        m_size(0),
        m_hash(-1)
    {
        if (rhs.size())
            copy(rhs.c_str(), rhs.size(), rhs.size());
        else
            m_buffer[m_size] = 0;
    }

    skFixedString(const char* rhs) :
        m_size(0),
        m_hash(-1)
    {
        if (rhs)
            copy(rhs, SK_NPOS16, SK_NPOS16);
        else
            m_buffer[m_size] = 0;
    }

    skFixedString(const char* rhs, SKuint16 size) :
        m_size(0),
        m_hash(-1)
    {
        if (rhs)
            copy(rhs, size, size);
        else
            m_buffer[m_size] = 0;
    }

    void push_back(char ch)
    {
        if (m_size >= L)
            return;

        m_buffer[m_size++] = ch;
        m_buffer[m_size]   = 0;
    }

    void resize(SKuint16 ns)
    {
        if (ns <= L)
        {
            m_size           = ns;
            m_buffer[m_size] = 0;
        }
    }

    void split(skArray<skFixedString<L> >& destination, char condition1, char condition2 = '\0') const
    {
        SKuint16 i, p = 0, t;
        for (i = 0; i < L && i < m_size; ++i)
        {
            if (m_buffer[i] == condition1 || m_buffer[i] == condition2)
            {
                skFixedString<L> copy;
                for (t = p; t < i; ++t)
                    copy.push_back(m_buffer[t]);
                destination.push_back(copy);
                p = i + 1;
            }
        }

        if (p != i)
        {
            skFixedString<L> copy;
            for (t = p; t < i; ++t)
                copy.push_back(m_buffer[t]);
            destination.push_back(copy);
        }
    }

    void append(const char* str)
    {
        const SKuint16 len = skClamp<SKuint16>((SKuint16)skChar::length(str), 0, SK_MAX16);
        SKuint16       a   = 0;
        while (a < len)
            push_back(str[a++]);
    }

    void append(const skFixedString& str)
    {
        const int len = str.m_size;
        int       a   = 0;
        while (a < len)
            push_back(str.m_buffer[a++]);
    }

    skFixedString& operator=(const skFixedString& rhs)
    {
        if (this != &rhs && rhs.m_size > 0)
            copy(rhs.c_str(), rhs.size(), rhs.size());
        return *this;
    }

    template <const SKuint16 OL>
    skFixedString<L>& operator=(const skFixedString<OL>& o)
    {
        if (o.size() > 0)
            copy(o.c_str(), o.size(), OL);
        return *this;
    }

    skFixedString operator+(const skFixedString& rhs)
    {
        skFixedString lhs = *this;
        lhs.append(rhs);
        return lhs;
    }

    skFixedString operator+=(const skFixedString& rhs)
    {
        append(rhs);
        return *this;
    }

    skFixedString operator+(const char* str)
    {
        skFixedString lhs = *this;
        lhs.append(str);
        return lhs;
    }

    skFixedString operator+=(const char* str)
    {
        append(str);
        return *this;
    }

    const char* c_str(void) const
    {
        return m_buffer;
    }

    char* ptr(void)
    {
        return m_buffer;
    }

    const char* ptr(void) const
    {
        return m_buffer;
    }

    void clear(void)
    {
        m_buffer[0] = 0;
        m_size      = 0;
    }

    bool empty(void) const
    {
        return m_size == 0;
    }

    SKuint16 size(void) const
    {
        return m_size;
    }

    static SKuint16 capacity(void)
    {
        return L;
    }

    char operator[](SKuint16 i) const
    {
        SK_ASSERT(i < m_size && i < L);
        return m_buffer[i];
    }

    char at(SKuint16 i) const
    {
        SK_ASSERT(i < m_size && i < L);
        return m_buffer[i];
    }

    char& operator[](SKuint16 i)
    {
        SK_ASSERT(i < m_size && i < L);
        return m_buffer[i];
    }

    char* at(SKuint16 i)
    {
        SK_ASSERT(i < m_size && i < L);
        return m_buffer[i];
    }

    SKsize hash(void) const
    {
        if (m_hash != -1)
            return m_hash;

        if (m_size == 0 || !m_buffer)
            return -1;

        m_hash = skHash(m_buffer);
        return m_hash;
    }

    bool operator==(const skFixedString& str) const
    {
        return hash() == str.hash();
    }

    bool operator!=(const skFixedString& str) const
    {
        return hash() != str.hash();
    }

private:
    Pointer        m_buffer;
    SKuint16       m_size;
    mutable SKhash m_hash;

    void copy(const char* src, SKuint16 size, SKuint16 other = SK_NPOS16)
    {
        const SKuint16 val = skMin(size, skMin(L, other));

        m_size = 0;
        m_hash = -1;

        while (m_size < val && src[m_size])
        {
            m_buffer[m_size] = src[m_size];
            ++m_size;
        }

        m_buffer[m_size] = 0;
    }
};

#endif  //_skFixedString_h_
