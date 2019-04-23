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
#ifndef _skQueue_h_
#define _skQueue_h_


#include "Config/skConfig.h"
#include "skTraits.h"
#include "skArray.h"


template <typename T>
class skQueueIncrementIterator
{
public:
    SK_DECLARE_REF_TYPE(T);

protected:
    mutable PointerType m_beg;
    mutable PointerType m_end;

    void swap(skQueueIncrementIterator& rhs)
    {
        skSwap(m_beg, rhs.m_beg);
        skSwap(m_end, rhs.m_end);
    }


public:
    skQueueIncrementIterator()
        : m_beg(0), m_end(0)
    {
    }

    skQueueIncrementIterator(PointerType begin, 
        SKsize size, SKsize front) : 
        m_beg(begin + front), 
        m_end(begin + size)
    {
    }

    explicit skQueueIncrementIterator(T& v)
        : m_beg(v.ptr() + v.front()), m_end(v.ptr() + v.size())
    {
    }

    skQueueIncrementIterator(const skQueueIncrementIterator& rhs)
        : m_beg(rhs.m_beg), m_end(rhs.m_end)
    {
    }


    ~skQueueIncrementIterator() {}

    skQueueIncrementIterator& operator = (const skQueueIncrementIterator& rhs)
    {
        if (this != &rhs)
            skQueueIncrementIterator(rhs).swap(*this);

        return *this;
    }


    SK_INLINE bool hasMoreElements(void) const
    {
        return m_beg < m_end;
    }

    SK_INLINE ReferenceType getNext(void)
    {
        SK_ITER_DEBUG(hasMoreElements());
        return (*m_beg++);
    }

    SK_INLINE ConstReferenceType getNext(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        return (*m_beg++);
    }


    SK_INLINE void next(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        ++m_beg;
    }


    SK_INLINE ReferenceType peekNext(void)
    {
        SK_ITER_DEBUG(hasMoreElements());
        return (*m_beg);
    }

    SK_INLINE ConstReferenceType peekNext(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        return (*m_beg);
    }

};



template <typename T>
class skQueueDecrementIterator
{
public:
    SK_DECLARE_REF_TYPE(T);

protected:
    mutable PointerType m_beg;
    mutable PointerType m_end;

    void swap(skQueueDecrementIterator& rhs)
    {
        skSwap(m_beg, rhs.m_beg);
        skSwap(m_end, rhs.m_end);
    }

public:


    skQueueDecrementIterator()
        : m_beg(0), m_end(0)
    {
    }


    skQueueDecrementIterator(PointerType begin, SKsize size, SKsize front)
        : m_beg(begin + (size - 1)), m_end(begin + front)
    {
    }

    explicit skQueueDecrementIterator(T& v)
        : m_beg(v.ptr() + (v.size() - 1)), m_end(v.ptr() + v.front())
    {
    }

    skQueueDecrementIterator(const skQueueDecrementIterator& rhs)
        : m_beg(rhs.m_beg), m_end(rhs.m_end)
    {
    }

    ~skQueueDecrementIterator() {}


    skQueueDecrementIterator& operator = (const skQueueDecrementIterator& rhs)
    {
        if (this != &rhs)
            skPointerIterator(*this).swap(rhs);
        return *this;
    }

    SK_INLINE bool hasMoreElements(void) const
    {
        return m_beg && m_beg >= m_end;
    }


    SK_INLINE ReferenceType getNext(void)
    {
        SK_ITER_DEBUG(hasMoreElements());
        return (*m_beg--);
    }

    SK_INLINE ConstReferenceType getNext(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        return (*m_beg--);
    }

    SK_INLINE void next(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        --m_beg;
    }

    SK_INLINE ReferenceType peekNext(void)
    {
        SK_ITER_DEBUG(hasMoreElements());
        return (*m_beg);
    }

    SK_INLINE ConstReferenceType peekNext(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        return (*m_beg);
    }

};




template < typename T, typename Allocator = skAllocator<T> >
class skQueue
{
public:
    SK_DECLARE_TYPE(T);

    typedef skQueue<T, Allocator> SelfType;
    typedef skQueueIncrementIterator<SelfType>          Iterator;
    typedef const skQueueIncrementIterator<SelfType>    ConstIterator;
    typedef skQueueDecrementIterator<SelfType>          ReverseIterator;
    typedef const skQueueDecrementIterator<SelfType>    ConstReverseIterator;

    SK_IMPLEMENT_QSORT(T, SelfType);

private:
    Allocator m_alloc;

    mutable PointerType m_data;
    SKsize m_front, m_back;
    SKsize m_size,  m_capacity;


public:

    skQueue() :
        m_data(0), 
        m_front(0), 
        m_back(0), 
        m_size(0), 
        m_capacity(0)
    {
    }


    skQueue(const skQueue& o) :
        m_data(0),
        m_front(0),
        m_back(0),
        m_size(0),
        m_capacity(0)
    {
        if (o.m_data) {
            m_size = o.size();
            m_back = m_size;

            reserve(m_size);
            skFill(m_data, o.m_data, m_size);
        }
    }

    ~skQueue() { clear(); }

    void clear(void)
    {
        if (m_data)
            m_alloc.array_deallocate(m_data, m_capacity);
        m_size = 0;
        m_front = 0;
        m_back = 0;
        m_capacity = 0;
    }



    void resize(SKsize nr)
    {
        if (nr < m_size) {
            for (SKsize i = m_size; i < nr; i++)
                m_alloc.destroy(&m_data[i]);

        }
        else {
            if (nr > m_size)
                reserve(nr);
        }

        m_front = 0;
        m_back =  nr;
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
        {
            reserve(m_size == 0 ? 8 : m_size * 2);
            m_back = m_size;
        }

        m_data[m_back] = value;

        m_back = (m_back + 1) % m_capacity;
        m_size++;
    }

    ReferenceType pop_front(void)
    {
        return dequeue();
    }

    ReferenceType dequeue(void)
    {
        ReferenceType returnValue = m_data[m_front];
        m_front =  (m_front + 1) % m_capacity;
        m_size--;
        return returnValue;
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
        SK_ASSERT(idx != SK_NPOS && idx < m_capacity);
        return m_data[(m_front + idx) % m_capacity];
    }

    SK_INLINE ConstReferenceType operator[](SKsize idx) const    
    {
        SK_ASSERT(m_data);
        SK_ASSERT(idx != SK_NPOS && idx < m_capacity);
        return m_data[(m_front + idx) % m_capacity];
    }

    SK_INLINE bool      empty(void) const { return m_size == 0; }
    SK_INLINE SKsize    size(void)  const { return m_size; }
    SK_INLINE SKsize    front(void) const { return m_front; }
    SK_INLINE SKsize    back(void)  const { return m_back; }

    SK_INLINE Iterator iterator(void)
    {
        return m_data && m_size > 0 ? 
            Iterator(m_data, m_size, m_front) : Iterator();
    }

    SK_INLINE ConstIterator iterator(void) const
    {
        return m_data && m_size > 0 ? 
            ConstIterator(m_data, m_size, m_front) : ConstIterator();
    }

    SK_INLINE ReverseIterator reverseIterator(void)
    {
        return m_data && m_size > 0 ? 
            ReverseIterator(m_data, m_size, m_front) : ReverseIterator();
    }


    SK_INLINE ConstReverseIterator reverseIterator(void) const
    {
        return m_data && m_size > 0 ? 
            ConstReverseIterator(m_data, m_size, m_front) : ConstReverseIterator();
    }

    skQueue& operator= (const skQueue& rhs)
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
};





#endif//_skQueue_h_
