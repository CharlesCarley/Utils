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
#include "skSort.h"

template <typename T>
class skPointerIncrementIterator
{
public:
    SK_DECLARE_REF_TYPE(T);

protected:
    mutable PointerType m_beg;
    mutable PointerType m_end;

    void swap(skPointerIncrementIterator& rhs)
    {
        skSwap(m_beg, rhs.m_beg);
        skSwap(m_end, rhs.m_end);
    }

public:
    skPointerIncrementIterator() :
        m_beg(0),
        m_end(0)
    {
    }

    skPointerIncrementIterator(PointerType begin, SKsize size) :
        m_beg(begin),
        m_end(begin + size)
    {
    }

    explicit skPointerIncrementIterator(T& v) :
        m_beg(v.ptr()),
        m_end(v.ptr() + v.size())
    {
    }

    skPointerIncrementIterator(const skPointerIncrementIterator& rhs) :
        m_beg(rhs.m_beg),
        m_end(rhs.m_end)
    {
    }

    ~skPointerIncrementIterator()
    {
    }

    skPointerIncrementIterator& operator=(const skPointerIncrementIterator& rhs)
    {
        if (this != &rhs)
            skPointerIncrementIterator(rhs).swap(*this);
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
class skPointerDecrementIterator
{
public:
    SK_DECLARE_REF_TYPE(T);

protected:
    mutable PointerType m_beg;
    mutable PointerType m_end;

    void swap(skPointerDecrementIterator& rhs)
    {
        skSwap(m_beg, rhs.m_beg);
        skSwap(m_end, rhs.m_end);
    }

public:
    skPointerDecrementIterator() :
        m_beg(0),
        m_end(0)
    {
    }

    skPointerDecrementIterator(PointerType begin, SKsize size) :
        m_beg(begin + (size - 1)),
        m_end(begin)
    {
    }

    explicit skPointerDecrementIterator(T& v) :
        m_beg(v.ptr() + (v.size() - 1)),
        m_end(v.ptr())
    {
    }

    skPointerDecrementIterator(const skPointerDecrementIterator& rhs) :
        m_beg(rhs.m_beg),
        m_end(rhs.m_end)
    {
    }

    ~skPointerDecrementIterator()
    {
    }

    skPointerDecrementIterator& operator=(const skPointerDecrementIterator& rhs)
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

template <typename T, typename Allocator = skAllocator<T> >
class skArray
{
public:
    SK_DECLARE_TYPE(T);

public:
    typedef skArray<T, Allocator>                      SelfType;
    typedef skPointerIncrementIterator<SelfType>       Iterator;
    typedef const skPointerIncrementIterator<SelfType> ConstIterator;
    typedef skPointerDecrementIterator<SelfType>       ReverseIterator;
    typedef const skPointerDecrementIterator<SelfType> ConstReverseIterator;

    SK_IMPLEMENT_QSORT(T, skArray);

public:
    skArray() :
        m_data(0),
        m_size(0),
        m_capacity(0)
    {
    }

    skArray(const skArray& o) :
        m_data(0),
        m_size(0),
        m_capacity(0)
    {
        if (o.m_data)
        {
            m_size = o.size();
            reserve(m_size);
            skFill(m_data, o.m_data, m_size);
        }
    }

    ~skArray()
    {
        clear();
    }

    void clear(void)
    {
        if (m_data)
            m_alloc.array_deallocate(m_data, m_capacity);

        m_data     = 0;
        m_capacity = 0;
        m_size     = 0;
    }

    const SKsize find(ConstReferenceType v) const
    {
        SKsize i;
        for (i = 0; i < m_size; i++)
        {
            if (m_data[i] == v)
                return i;
        }
        return SK_NPOS;
    }

    void push_back(ConstReferenceType v)
    {
        if (m_size > m_alloc.max_size())
        {
            // do nothing
        }
        else if (m_size == m_capacity)
        {
            reserve(m_size == 0 ? 8 : m_size * 2);
            m_data[m_size++] = v;
        }
        else
            m_data[m_size++] = v;
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

    void remove(SKsize pos)
    {
        if (m_size > 0)
        {
            if (pos != SK_NPOS)
            {
                skSwap(m_data[pos], m_data[m_size - 1]);
                m_alloc.destroy(&m_data[--m_size]);
            }
        }
    }

    void resize(SKsize nr)
    {
        if (nr < m_size)
        {
            for (SKsize i = nr; i < m_size; i++)
                m_alloc.destroy(&m_data[i]);
        }
        else
        {
            if (nr > m_size)
                reserve(nr);
        }
        m_size = nr;
    }

    void resize(SKsize nr, ConstReferenceType fill)
    {
        SKsize i;

        if (nr < m_size)
        {
            for (i = nr; i < m_size; i++)
                m_data[i].~T();
        }
        else
        {
            if (nr > m_size)
                reserve(nr);
            skFill(m_data + m_size, fill, nr);
        }
        m_size = nr;
    }

    void reserve(SKsize nr)
    {
        if (m_capacity > nr)
            ;
        else if (m_data)
        {
            m_data     = m_alloc.array_reallocate(m_data, nr, m_size);
            m_capacity = nr;
        }
        else
        {
            m_data     = m_alloc.array_allocate(nr);
            m_capacity = nr;
        }
    }

    SK_INLINE ReferenceType operator[](SKsize idx)
    {
        SK_ASSERT(idx >= 0 && idx < m_capacity);
        return m_data[idx];
    }

    SK_INLINE ConstReferenceType operator[](SKsize idx) const
    {
        SK_ASSERT(idx >= 0 && idx < m_capacity);
        return m_data[idx];
    }

    SK_INLINE ReferenceType at(SKsize idx)
    {
        SK_ASSERT(idx >= 0 && idx < m_capacity);
        return m_data[idx];
    }

    SK_INLINE ConstReferenceType at(SKsize idx) const
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
    SK_INLINE SKsize capacity(void) const
    {
        return m_capacity;
    }
    SK_INLINE SKsize size(void) const
    {
        return m_size;
    }
    SK_INLINE bool empty(void) const
    {
        return m_size == 0;
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
        if (this != &rhs)
        {
            if (rhs.m_size > 0 && rhs.m_data)
            {
                clear();
                resize(rhs.m_size);
                skFill(m_data, rhs.m_data, rhs.m_size);
            }
        }
        return *this;
    }

private:
    PointerType m_data;
    SKsize      m_size, m_capacity;
    Allocator   m_alloc;
};

#endif  //_skArray_h_
