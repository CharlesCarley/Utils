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

#include "Config/skConfig.h"
#include "Utils/skArray.h"

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
        {
            SKuint16    i;
            const SKuint16 os = rhs.size();
            const char* cp = rhs.c_str();

            for (i = 0; i < L && i < os; ++i, ++m_size)
                m_buffer[i] = cp[i];
        }
        m_buffer[m_size] = 0;
    }

    skFixedString(const char* rhs) :
        m_size(0),
        m_hash(-1)
    {
        if (rhs)
        {
            SKuint16 i;

            for (i = 0; i < L && rhs[i]; ++i, ++m_size)
                m_buffer[i] = rhs[i];
        }

        m_buffer[m_size] = 0;
    }

    skFixedString(const char* rhs, SKuint16 size) :
        m_size(0),
        m_hash(-1)
    {
        if (rhs)
        {
            SKuint16 i;
            for (i = 0; i < L && i < size && rhs[i]; ++i, ++m_size)
                m_buffer[i] = rhs[i];
        }
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


    
    void split(skArray<skFixedString<L> >& dest, char c, char e = '\0') const
    {
        SKuint16 i, p = 0, t;
        for (i = 0; i < L && i < m_size; ++i)
        {
            if (m_buffer[i] == c || m_buffer[i] == e)
            {
                skFixedString<L> cpy;
                for (t = p; t < i; ++t)
                    cpy.push_back(m_buffer[t]);
                dest.push_back(cpy);
                p = i + 1;
            }
        }

        if (p != i)
        {
            skFixedString<L> cpy;
            for (t = p; t < i; ++t)
                cpy.push_back(m_buffer[t]);
            dest.push_back(cpy);
        }
    }


    void append(const char* str)
    {
        int len = strlen(str);
        int a   = 0;
        while (a < len)
            push_back(str[a++]);
    }


    void append(const skFixedString& str)
    {
        int len = str.m_size;
        int a   = 0;
        while (a < len)
            push_back(str.m_buffer[a++]);
    }


    skFixedString& operator=(const skFixedString& rhs)
    {
        if (this != &rhs && rhs.m_size > 0)
        {
            SKuint16 i;
            m_size = 0;
            m_hash = -1;
            for (i = 0; i < L && i < rhs.m_size; ++i, ++m_size)
                m_buffer[i] = rhs.m_buffer[i];
            m_buffer[m_size] = 0;
        }

        return *this;
    }

    template <const SKuint16 OL>
    skFixedString<L>& operator=(const skFixedString<OL>& o)
    {
        if (o.size() > 0)
        {
            SKuint16 i;
            m_size = 0;
            for (i = 0; (i < L && i < OL) && i < o.size(); ++i, ++m_size)
                m_buffer[i] = o.c_str()[i];
            m_buffer[m_size] = 0;
        }
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

    SK_INLINE const char* c_str(void) const
    {
        return m_buffer;
    }

    SK_INLINE char* ptr(void)
    {
        return m_buffer;
    }

    SK_INLINE const char* ptr(void) const
    {
        return m_buffer;
    }

    SK_INLINE void clear(void)
    {
        m_buffer[0] = 0;
        m_size      = 0;
    }

    SK_INLINE bool empty(void) const
    {
        return m_size == 0;
    }

    SK_INLINE SKuint16 size(void) const
    {
        return m_size;
    }
    SK_INLINE SKuint16 capacity(void) const
    {
        return L;
    }

    SK_INLINE const char operator[](SKuint16 i) const
    {
        SK_ASSERT(i < m_size && i < L);
        return m_buffer[i];
    }

    SK_INLINE const char at(SKuint16 i) const
    {
        SK_ASSERT(i < m_size && i < L);
        return m_buffer[i];
    }

    SK_INLINE char& operator[](SKuint16 i)
    {
        SK_ASSERT(i < m_size && i < L);
        return m_buffer[i];
    }

    SK_INLINE char* at(SKuint16 i)
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

    SK_INLINE bool operator==(const skFixedString& str) const
    {
        return hash() == str.hash();
    }
    SK_INLINE bool operator!=(const skFixedString& str) const
    {
        return hash() != str.hash();
    }

private:
    Pointer        m_buffer;
    SKuint16       m_size;
    mutable SKhash m_hash;
};

#endif  //_skFixedString_h_
