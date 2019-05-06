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
#include "Utils/skTypes.h"

template <const SKuint16 L>
class skFixedString
{
public:
    typedef char Pointer[L + 1];

public:
    skFixedString()
        :   m_size(0),  m_hash(SK_NPOS)
    {
        m_buffer[m_size] = 0;
    }

    skFixedString(const skFixedString& rhs)
        :   m_size(0), m_hash(SK_NPOS)
    {
        if (rhs.size())
        {
            SKuint16 i, os = rhs.size();
            const char* cp = rhs.c_str();

            for (i = 0; i < L && i < os; ++i, ++m_size) m_buffer[i] = cp[i];
        }
        m_buffer[m_size] = 0;
    }


    skFixedString(const char* rhs)
        :   m_size(0), m_hash(SK_NPOS)
    {
        if (rhs)
        {
            SKuint16 i;

            for (i = 0; i < L && rhs[i]; ++i, ++m_size) m_buffer[i] = rhs[i];
        }

        m_buffer[m_size] = 0;
    }

    skFixedString(const char* rhs, SKuint16 size)
        :   m_size(0), m_hash(SK_NPOS)
    {
        if (rhs)
        {
            SKuint16 i;
            for (i = 0; i < L && i < size && rhs[i]; ++i, ++m_size) m_buffer[i] = rhs[i];
        }
        m_buffer[m_size] = 0;
    }

    void push_back(char ch)
    {
        if (m_size >= L)
            return;

        m_buffer[m_size++] = ch;
        m_buffer[m_size] = 0;
    }

    void resize(SKuint16 ns)
    {
        if (ns <= L)
        {
            //if (ns < m_size)
            //    for (SKuint16 i = ns; i < m_size; i++) m_buffer[i] = 0;
            m_size = ns;
            m_buffer[m_size] = 0;
        }
    }

    skFixedString& operator = (const skFixedString& rhs)
    {
        if (this != &rhs && rhs.m_size > 0)
        {
            SKuint16 i;
            m_size = 0;
            m_hash = SK_NPOS;
            for (i = 0; i < L && i < rhs.m_size; ++i, ++m_size)
                m_buffer[i] = rhs.m_buffer[i];
            m_buffer[m_size] = 0;
        }

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
        m_buffer[0] = 0; m_size = 0;
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

    SK_INLINE const char operator [](SKuint16 i) const
    {
        SK_ASSERT(i < m_size && i < L);
        return m_buffer[i];
    }

    SK_INLINE const char at(SKuint16 i) const
    {
        SK_ASSERT(i < m_size && i < L);
        return m_buffer[i];
    }

    SK_INLINE char& operator [](SKuint16 i)
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
        if (m_hash != SK_NPOS)
            return m_hash;

        if (m_size == 0 || !m_buffer)
            return SK_NPOS;

        m_hash = skHash(m_buffer);
        return m_hash;
    }

    SK_INLINE bool operator == (const skFixedString& str) const
    {
        return hash() == str.hash();
    }
    SK_INLINE bool operator != (const skFixedString& str) const
    {
        return hash() != str.hash();
    }

private:

    Pointer m_buffer;
    SKuint16 m_size;
    mutable SKhash m_hash;
};


#endif//_skFixedString_h_
