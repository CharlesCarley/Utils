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
#ifndef _skArray_h_
#define _skArray_h_

#include "skAllocator.h"
#include "skArrayBase.h"
#include "skMinMax.h"
#include "skSort.h"


template <typename T, typename Allocator = skAllocator<T, SKuint32> >
class skArray : public skArrayBase<T, Allocator>
{
public:
    SK_DECLARE_TYPE(T);

    typedef skArray<T, Allocator>     SelfType;
    typedef skArrayBase<T, Allocator> BaseType;

    typedef typename Allocator::SizeType SizeType;

    typedef skPointerIncrementIterator<SelfType, SizeType>       Iterator;
    typedef const skPointerIncrementIterator<SelfType, SizeType> ConstIterator;
    typedef skPointerDecrementIterator<SelfType, SizeType>       ReverseIterator;
    typedef const skPointerDecrementIterator<SelfType, SizeType> ConstReverseIterator;

    SK_IMPLEMENT_QSORT(T, skArray, SizeType);

public:
    skArray()
    {
    }

    skArray(const skArray& o) :
        skArrayBase(o)
    {
    }

    ~skArray()
    {
        clear();
    }

    void clear(void)
    {
        destroy();
    }

    SizeType find(ConstReferenceType v) const
    {
        SizeType i;
        for (i = 0; i < m_size; i++)
        {
            if (m_data[i] == v)
                return i;
        }
        return m_alloc.npos;
    }

    void push_back(ConstReferenceType v)
    {
        if (m_size + 1 <= m_alloc.limit)
        {
            if (m_size + 1 > m_capacity)
            {
                reserve(m_size == 0 ? 8 : m_size * 2);
                m_data[m_size++] = v;
            }
            else
                m_data[m_size++] = v;
        }
        else
            throw m_alloc.limit;
    }

    void pop_back(void)
    {
        if (m_size > 0)
        {
            m_alloc.destroy(&m_data[m_size]);
            m_size--;
        }
    }

    void erase(ConstReferenceType v)
    {
        remove(find(v));
    }

    void remove(SizeType pos)
    {
        if (m_size > 0)
        {
            if (pos != m_alloc.npos)
            {
                skSwap(m_data[pos], m_data[m_size - 1]);
                m_alloc.destroy(&m_data[--m_size]);
            }
        }
    }

    SK_INLINE ReferenceType operator[](SizeType idx)
    {
        SK_ASSERT(idx >= 0 && idx < m_capacity);
        return m_data[idx];
    }

    SK_INLINE ConstReferenceType operator[](SizeType idx) const
    {
        SK_ASSERT(idx >= 0 && idx < m_capacity);
        return m_data[idx];
    }

    SK_INLINE ReferenceType at(SizeType idx)
    {
        SK_ASSERT(idx >= 0 && idx < m_capacity);
        return m_data[idx];
    }

    SK_INLINE ConstReferenceType at(SizeType idx) const
    {
        SK_ASSERT(idx >= 0 && idx < m_capacity);
        return m_data[idx];
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

    SK_INLINE ConstPointerType ptr(void) const
    {
        return m_data;
    }

    SK_INLINE PointerType ptr(void)
    {
        return m_data;
    }

    SK_INLINE bool valid(void) const
    {
        return m_data != 0;
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

    skArray& operator=(const skArray& rhs)
    {
        replicate(rhs);
        return *this;
    }

};

#endif  //_skArray_h_
