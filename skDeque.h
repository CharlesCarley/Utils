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
#ifndef _skDeque_h_
#define _skDeque_h_


#include "skSort.h"
#include "skAllocator.h"
#include "skArray.h"


template < typename T, typename Allocator = skAllocator<T> >
class skDeque
{
public:
    SK_DECLARE_TYPE(T);
public:
    typedef skDeque<T, Allocator>                         SelfType;
    typedef skPointerIncrementIterator<SelfType>          Iterator;
    typedef const skPointerIncrementIterator<SelfType>    ConstIterator;
    typedef skPointerDecrementIterator<SelfType>          ReverseIterator;
    typedef const skPointerDecrementIterator<SelfType>    ConstReverseIterator;

    SK_IMPLEMENT_QSORT(T, SelfType);

private:
    PointerType m_data;
    SKsize m_size, m_capacity;
    Allocator m_alloc;
public:


    skDeque()
        : m_data(0), m_size(0), m_capacity(0)
    {
    }

    
    skDeque(const skDeque& o)
        : m_data(0), m_size(0), m_capacity(0)
    {
        if (o.m_data) {
            m_size  = o.size();
            reserve(m_size);
            skFill(m_data, o.m_data, m_size);
        }
    }

    ~skDeque()
    {
        clear();
    }

    void clear(void)
    {
        if (m_data)
            m_alloc.array_deallocate(m_data, m_capacity);

        m_data = 0;
        m_capacity = 0;
        m_size = 0;
    }

    SKsize find(ConstReferenceType v)
    {
        return SK_NPOS;
    }


    SK_INLINE void push_front(ConstReferenceType v)
    {
        if (m_size == 0)
            push_back(v);
        else
            stackRecurse(v);
    }



    void push_back(ConstReferenceType v)
    {
        if (m_size > m_alloc.max_size())
            return;
        else if (m_size == m_capacity)
            reserve(m_size == 0 ? 8 : m_size * 2);

        m_data[m_size++] = v;
    }

    ReferenceType pop_back(void)
    {
        SK_ASSERT(m_size - 1 > 0);
        return m_data[--m_size];
    }

    ReferenceType pop_front(void)
    {
        return stackRecurse();
    }

    void resize(SKsize nr)
    {
        if (nr < m_size) {
            for (SKsize i = m_size; i < nr; i++)
                m_alloc.destroy(&m_data[i]);

        } else {
            if (nr > m_size)
                reserve(nr);
        }

        m_size = nr;
    }

    void resize(SKsize nr, ConstReferenceType fill)
    {
        SKsize i;
        if (nr < m_size) {
            for (i = m_size; i < nr; i++)
                m_data[i].~T();
        } else {
            if (nr > m_size)
                reserve(nr);
            skFill(m_data + m_size, fill, nr);
        }
        m_size = nr;
    }

    void reserve(SKsize nr)
    {
        if (m_capacity > nr);
        else if (m_data) {
            m_data = m_alloc.array_reallocate(m_data, nr, m_size);
            m_capacity = nr;
        } else {
            m_data = m_alloc.array_allocate(nr);
            m_capacity = nr;
        }
    }


    SK_INLINE ReferenceType operator[](SKsize idx)
    {
        SK_ASSERT(idx >= 0 && idx < m_size);
        return m_data[(m_size-1)-idx];
    }

    SK_INLINE ConstReferenceType operator[](SKsize idx) const
    {
        SK_ASSERT(idx >= 0 && idx < m_capacity);
        return m_data[(m_size - 1) - idx];
    }


    SK_INLINE ReferenceType         at(SKsize idx)          { return (*this)[idx]; }
    SK_INLINE ConstReferenceType    at(SKsize idx) const    { return (*this)[idx]; }


    skDeque& operator= (const skDeque& rhs)
    {
        if (this != &rhs) {
            if (rhs.m_size > 0 && rhs.m_data) {
                clear();
                resize(rhs.m_size);
                skFill(m_data, rhs.m_data, rhs.m_size);
            }
        }
        return *this;
    }


    SK_INLINE bool valid(void) const      { return m_data != 0; }
    SK_INLINE SKsize capacity(void) const { return m_capacity; }
    SK_INLINE SKsize size(void) const     { return m_size; }
    SK_INLINE bool empty(void) const      { return m_size == 0; }




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

    void stackRecurse(ConstReferenceType first)
    {
        if (m_size == 1)
        {
            ValueType val = pop();
            push_back(first);
            push_back(val);
        }
        else
        {
            ValueType val = pop();
            stackRecurse(first);
            push_back(val);
        }
    }


    ReferenceType stackRecurse(void)
    {
        if (m_size == 1)
            return pop();
        else
        {
            ReferenceType val = pop();
            ReferenceType rval = stackRecurse();
            push_back(val);
            return rval;
        }
    }

};

#endif//_skDeque_h_
