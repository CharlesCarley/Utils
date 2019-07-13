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
#ifndef _skStackQueue_h_
#define _skStackQueue_h_

#include "Config/skConfig.h"
#include "skArray.h"
#include "skTraits.h"

template <typename T, typename Allocator = skAllocator<T> >
class skStackQueue
{
public:
    SK_DECLARE_TYPE(T);
    typedef skStackQueue<T, Allocator> SelfType;

    typedef skPointerIncrementIterator<SelfType>       Iterator;
    typedef const skPointerIncrementIterator<SelfType> ConstIterator;
    typedef skPointerDecrementIterator<SelfType>       ReverseIterator;
    typedef const skPointerDecrementIterator<SelfType> ConstReverseIterator;

    SK_IMPLEMENT_QSORT(T, SelfType);

private:
    Allocator           m_alloc;
    mutable PointerType m_data;
    SKsize              m_size, m_capacity;

public:
    skStackQueue() : m_data(0), m_size(0), m_capacity(0)
    {
    }

    skStackQueue(const skStackQueue& o) : m_data(0), m_size(0), m_capacity(0)
    {
        if (o.m_data)
        {
            m_size = o.size();
            reserve(m_size);
            skFill(m_data, o.m_data, m_size);
        }
    }

    ~skStackQueue()
    {
        clear();
    }

    void clear(void)
    {
        if (m_data)
            m_alloc.array_deallocate(m_data, m_capacity);
        m_size = 0;
        m_capacity = 0;
    }

    void resize(SKsize nr)
    {
        if (nr < m_size)
        {
            for (SKsize i = m_size; i < nr; i++)
                m_alloc.destroy(&m_data[i]);
        }
        else
        {
            if (nr > m_size)
                reserve(nr);
        }
        m_size = nr;
    }

    void reserve(SKsize nr)
    {
        if (m_capacity > nr || nr > m_alloc.max_size())
            return;
        if (m_data && m_size > 0)
            m_data = m_alloc.array_reallocate(m_data, nr, m_size);
        else
            m_data = m_alloc.array_allocate(nr);
        m_capacity = nr;
    }

    void push_back(ConstReferenceType value)
    {
        enqueue(value);
    }

    void enqueue(ConstReferenceType value)
    {
        if (m_size > m_alloc.max_size())
            return;
        if (m_size == m_capacity)
            reserve(m_size == 0 ? 8 : m_size * 2);

        m_data[m_size++] = value;
    }

    ReferenceType pop_front(void)
    {
        return dequeue();
    }

    ReferenceType dequeue(void)
    {
        return stackRecurse();
    }

    SK_INLINE ReferenceType at(SKsize idx)
    {
        return (*this)[idx];
    }

    SK_INLINE ConstReferenceType at(SKsize idx) const
    {
        return (*this)[idx];
    }

    SK_INLINE ReferenceType operator[](SKsize idx)
    {
        SK_ASSERT(m_data);
        SK_ASSERT(idx != SK_NPOS && idx < m_size);
        return m_data[(m_size - 1) - idx];
    }

    SK_INLINE ConstReferenceType operator[](SKsize idx) const
    {
        SK_ASSERT(m_data);
        SK_ASSERT(idx != SK_NPOS && idx < m_size);
        return m_data[(m_size - 1) - idx];
    }

    SK_INLINE bool empty(void) const
    {
        return m_size == 0;
    }
    SK_INLINE SKsize size(void) const
    {
        return m_size;
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

private:
    ReferenceType pop(void)
    {
        SK_ASSERT(m_size >= 0);
        return m_data[--m_size];
    }

    ReferenceType stackRecurse(void)
    {
        if (m_size == 1)
            return pop();
        else
        {
            ReferenceType val = pop();
            ReferenceType rval = stackRecurse();
            enqueue(val);
            return rval;
        }
    }
};

#endif //_skStackQueue_h_
