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
#ifndef _skArrayBase_h_
#define _skArrayBase_h_

#include "skAllocator.h"
#include "skMinMax.h"
#include "skSort.h"

template <typename T, typename SizeType = SKuint32>
class skPointerIncrementIterator
{
public:
    SK_DECLARE_REF_TYPE(T)

protected:
    mutable PointerType m_beg;
    mutable PointerType m_end;

    void swap(skPointerIncrementIterator& rhs) noexcept
    {
        skSwap(m_beg, rhs.m_beg);
        skSwap(m_end, rhs.m_end);
    }

public:
    skPointerIncrementIterator() :
        m_beg(nullptr),
        m_end(nullptr)
    {
    }

    skPointerIncrementIterator(PointerType begin, SizeType size) :
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

    ~skPointerIncrementIterator() = default;

    skPointerIncrementIterator& operator=(const skPointerIncrementIterator& rhs)
    {
        if (this != &rhs)
            skPointerIncrementIterator(rhs).swap(*this);
        return *this;
    }

    bool hasMoreElements(void) const
    {
        return m_beg < m_end;
    }

    ReferenceType getNext(void)
    {
        SK_ITER_DEBUG(hasMoreElements());
        return *m_beg++;
    }

    ConstReferenceType getNext(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        return *m_beg++;
    }

    void next(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        ++m_beg;
    }

    ReferenceType peekNext(void)
    {
        SK_ITER_DEBUG(hasMoreElements());
        return *m_beg;
    }

    ConstReferenceType peekNext(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        return *m_beg;
    }
};

template <typename T, typename SizeType = SKuint32>
class skPointerDecrementIterator
{
public:
    SK_DECLARE_REF_TYPE(T)

protected:
    mutable PointerType m_beg;
    mutable PointerType m_end;

    void swap(skPointerDecrementIterator& rhs) noexcept
    {
        skSwap(m_beg, rhs.m_beg);
        skSwap(m_end, rhs.m_end);
    }

public:
    skPointerDecrementIterator() :
        m_beg(nullptr),
        m_end(nullptr)
    {
    }

    skPointerDecrementIterator(PointerType begin, SizeType size) :
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

    ~skPointerDecrementIterator() = default;

    skPointerDecrementIterator& operator=(const skPointerDecrementIterator& rhs)
    {
        if (this != &rhs)
            skPointerIterator(*this).swap(rhs);
        return *this;
    }

    bool hasMoreElements(void) const
    {
        return m_beg && m_beg >= m_end;
    }

    ReferenceType getNext(void)
    {
        SK_ITER_DEBUG(hasMoreElements());
        return *m_beg--;
    }

    ConstReferenceType getNext(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        return *m_beg--;
    }

    void next(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        --m_beg;
    }

    ReferenceType peekNext(void)
    {
        SK_ITER_DEBUG(hasMoreElements());
        return *m_beg;
    }

    ConstReferenceType peekNext(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        return *m_beg;
    }
};

template <typename T, typename Allocator = skAllocator<T, SKuint32> >
class skArrayBase
{
public:
    SK_DECLARE_TYPE(T)
    typedef skArrayBase<T, Allocator>    SelfType;
    typedef typename Allocator::SizeType SizeType;

    const SizeType npos = Allocator::npos;

public:
    SizeType find(ConstReferenceType v) const
    {
        SizeType i;
        for (i = 0; i < m_size; ++i)
        {
            if (m_data[i] == v)
                return i;
        }
        return m_alloc.npos;
    }

    // This assumes that the array was previously sorted by the 'Param' type.
    // Param should be some common member of the type T.
    // Returns NPOS if not found or the index of the found array element.
    template <typename Param>
    SizeType findBinary(int (*cmpFunc)(ValueType a, Param b), Param param) const
    {
        if (!cmpFunc)
            return m_alloc.npos;

        SizeType f = 0, l = m_size, m, cmp;
        while (f <= l)
        {
            m   = (f + l) / 2;
            cmp = cmpFunc(m_data[m], param);
            if (cmp == 0)
                return m;
            if (cmp > 0)
                l = m - 1;
            else
                f = m + 1;
        }
        return m_alloc.npos;
    }

    SizeType findBinary(ConstReferenceType key) const
    {
        if (!key)
            return m_alloc.npos;

        SizeType f = 0, l = m_size, m;
        while (f <= l)
        {
            m = (f + l) / 2;
            if (m_data[m] == key)
                return m;
            if (m_data > key)
                l = m - 1;
            else
                f = m + 1;
        }
        return m_alloc.npos;
    }

    void resizeFast(SizeType nr)
    {
        // Do not call this with a type T that has
        // cleanup code in ~T(). This is intended
        // for atomic and simple class types. 
        if (nr > m_size)
            reserve(nr);
        m_size = nr;
    }

    void resize(SizeType nr)
    {
        if (nr < m_size)
        {
            for (SizeType i = nr; i < m_size; ++i)
                m_alloc.destroy(&m_data[i]);
        }
        else
        {
            if (nr > m_size)
                reserve(nr);
        }
        m_size = nr;
    }

    void resize(SizeType nr, ConstReferenceType fill)
    {
        SizeType i;

        if (nr < m_size)
        {
            for (i = nr; i < m_size; ++i)
                m_data[i].~T();
        }
        else
        {
            if (nr > m_size)
                reserve(nr);

            m_alloc.fill(m_data + m_size, fill, nr);
        }
        m_size = nr;
    }

    void reserve(SizeType capacity)
    {
        if (m_capacity < capacity)
        {
            // This is using capacity plus one in order to reserve
            // and then push up to the max without expanding the
            // data. One more element after that will cause the expansion.
            capacity = skMin<SKuint32>(capacity + 1, m_alloc.limit);
            if (m_data)
            {
                m_data     = m_alloc.array_reallocate(m_data, capacity, m_size);
                m_capacity = capacity;
            }
            else
            {
                m_data     = m_alloc.array_allocate(capacity);
                m_capacity = capacity;
            }
        }
    }

    void write(PointerType writeData, SizeType writeDataCount)
    {
        if (m_capacity < writeDataCount)
            reserve(writeDataCount);

        skFill(m_data, writeData, writeDataCount);
        m_size = writeDataCount;
    }

    ConstPointerType ptr(void) const
    {
        return m_data;
    }

    PointerType ptr(void)
    {
        return m_data;
    }

    bool valid(void) const
    {
        return m_data != nullptr;
    }

    SizeType capacity(void) const
    {
        return m_capacity;
    }

    SizeType size(void) const
    {
        return m_size;
    }

    bool empty(void) const
    {
        return m_size == 0;
    }

protected:
    skArrayBase() :
        m_data(nullptr),
        m_size(0),
        m_capacity(0)
    {
    }

    skArrayBase(const skArrayBase& o) :
        m_data(nullptr),
        m_size(0),
        m_capacity(0)
    {
        replicate(o);
    }

    explicit skArrayBase(const SizeType& initialCapacity) :
        m_data(nullptr),
        m_size(0),
        m_capacity(0)
    {
        reserve(initialCapacity);
    }

    ~skArrayBase() = default;

    void replicate(const skArrayBase& rhs)
    {
        if (this != &rhs)
        {
            destroy();

            if (rhs.m_capacity > 0 && rhs.m_capacity < m_alloc.limit)
            {
                reserve(rhs.m_capacity);
                ConstPointerType pt = rhs.ptr();
                for (m_size = 0; m_size < rhs.m_size && m_data; ++m_size)
                    m_data[m_size] = pt[m_size];
            }
            else
            {
                m_data = 0;
                m_size = m_capacity = 0;
            }
        }
    }

    void destroy(void)
    {
        if (m_data)
            m_alloc.array_deallocate(m_data, m_capacity);
        m_data     = nullptr;
        m_capacity = 0;
        m_size     = 0;
    }

    PointerType m_data;
    SizeType    m_size, m_capacity;
    Allocator   m_alloc;
};

#endif  //_skArrayBase_h_
