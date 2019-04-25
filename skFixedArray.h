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
#ifndef _skFixedArray_h_
#define _skFixedArray_h_


#include "Config/skConfig.h"
#include "Utils/skTypes.h"



template <typename T, const SKuint16 L>
class skFixedArray
{
public:
    typedef skFixedArray<T, L> SelfType;
    typedef skPointerIncrementIterator<SelfType>          Iterator;
    typedef const skPointerIncrementIterator<SelfType>    ConstIterator;
    typedef skPointerDecrementIterator<SelfType>          ReverseIterator;
    typedef const skPointerDecrementIterator<SelfType>    ConstReverseIterator;

    SK_DECLARE_TYPE(T);
    SK_IMPLEMENT_QSORT(T, SelfType);

    const SKuint16 LIMIT = L;

public:
    skFixedArray() :
        m_size(0)
    {
    }

    skFixedArray(const skFixedArray& rhs)
        : m_size(0)
    {
        if (rhs.size())
        {
            SKuint16 i, os = rhs.size();
            ConstPointerType cp = rhs.ptr();
            for (i = 0; i < L && i < os; ++i, ++m_size)
                m_data[i] = cp[i];
        }
    }

    template <const SKuint16 R>
    skFixedArray(const skFixedArray<T, R>& rhs)
        : m_size(0)
    {
        if (rhs.size())
        {
            SKuint16 i, os = rhs.size();
            ConstPointerType cp = rhs.ptr();
            for (i = 0; i < L && i < os; ++i, ++m_size)
                m_data[i] = cp[i];
        }
    }


    skFixedArray(ConstPointerType rhs)
        : m_size(0)
    {
        if (rhs)
        {
            SKuint16 i;
            for (i = 0; i < L && rhs[i]; ++i, ++m_size)
                m_data[i] = rhs[i];
        }
    }

    skFixedArray(ConstPointerType rhs, SKuint16 size)
        : m_size(0)
    {
        if (rhs)
        {
            SKuint16 i;
            for (i = 0; i < L && i < size && rhs[i]; ++i, ++m_size)
                m_data[i] = rhs[i];
        }
    }

    void push_back(T rhs)
    {
        if (m_size >= L)
            return;

        m_data[m_size++] = rhs;
    }

    template <const SKuint16 A>
    void push_back(T rhs[A])
    {
        if (m_size >= L)
            return;

        SKuint16 l;
        for (l = m_size; l < A && l < L; ++l)
            m_data[m_size++] = rhs[l];
    }

    void pop_back(void)
    {
        if (m_size > 0)
        {
            m_data[m_size].~T();
            m_size--;
        }
    }

    SKuint16 find(ConstReferenceType v)
    {
        SKuint16 i;
        for (i = 0; i < m_size; i++)
        {
            if (m_data[i] == v)
                return i;
        }
        return SK_NPOS16;
    }

    void remove(SKuint16 pos)
    {
        if (m_size > 0)
        {
            if (pos < SK_NPOS16)
            {
                skSwap(m_data[pos], m_data[m_size - 1]);
                m_data[m_size - 1].~T();
                m_size--;
            }
        }
    }

    SK_INLINE ReferenceType front(void)
    {
        SK_ASSERT(m_size > 0);
        return m_data[0];
    }

    SK_INLINE ReferenceType back(void)
    {
        SK_ASSERT(m_size > 0);
        return m_data[m_size - 1];
    }

    SK_INLINE ConstReferenceType front(void) const
    {
        SK_ASSERT(m_size > 0);
        return m_data[0];
    }

    SK_INLINE ConstReferenceType back(void) const
    {
        SK_ASSERT(m_size > 0);
        return m_data[m_size - 1];
    }

    void resize(SKuint16 ns)
    {
        if (ns <= L)
        {
            if (ns < m_size)
            {
                SKuint16 i;
                for (i = ns; i < m_size; i++)
                    m_data[i].~T();
            }
            m_size = ns;
        }
    }


    skFixedArray& operator = (const skFixedArray& rhs)
    {
        if (this != &rhs && rhs.m_size > 0)
        {
            SKuint16 i;
            m_size = 0;
            for (i = 0;  i < rhs.m_size && i < L; ++i, ++m_size)
                m_data[i] = rhs.m_data[i];
        }
        return *this;
    }

    void clear(void)
    {
        SKuint16 i;
        for (i = 0; i < m_size; i++)
            m_data[i].~T();
        m_size = 0;
    }


    void fill(ConstReferenceType v)
    {
        skFill<T>(m_data, v, L);
    }

    SK_INLINE PointerType       ptr(void)               { return &m_data[0]; }
    SK_INLINE ConstPointerType  ptr(void) const         { return &m_data[0]; }
    SK_INLINE bool              empty(void) const       { return m_size == 0; }
    SK_INLINE SKuint16          size(void) const        { return m_size; }
    SK_INLINE SKuint16          capacity(void) const    { return L; }

    SK_INLINE ConstReferenceType operator [](SKuint16 i) const
    {
        SK_ASSERT(i < m_size && i < L);
        return m_data[i];
    }

    SK_INLINE ConstReferenceType at(SKuint16 i) const
    {
        return (*this)[idx];
    }

    SK_INLINE ReferenceType operator [](SKuint16 i)
    {
        SK_ASSERT(i < m_size && i < L);
        return m_data[i];
    }

    SK_INLINE ReferenceType at(SKuint16 i)
    {
        return (*this)[idx];
    }


    SK_INLINE Iterator iterator(void)
    {
        return m_size > 0 ? Iterator((PointerType)ptr(), m_size) : Iterator();
    }

    SK_INLINE ConstIterator iterator(void) const
    {
        return m_size > 0 ? ConstIterator((PointerType)ptr(), m_size) : ConstIterator();
    }

    SK_INLINE ReverseIterator reverseIterator(void)
    {
        return m_size > 0 ? ReverseIterator((PointerType)ptr(), m_size) : ReverseIterator();
    }

    SK_INLINE ConstReverseIterator reverseIterator(void) const
    {
        return m_size > 0 ? ConstReverseIterator((PointerType)ptr(), m_size) : ConstReverseIterator();
    }

protected:
    mutable ValueType   m_data[L];
    SKuint16            m_size;
};


#endif//_skFixedArray_h_
