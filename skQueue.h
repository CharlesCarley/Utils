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
#include "skArray.h"
#include "skTraits.h"

template <typename T>
class skQueueIncrementIterator
{
public:
    SK_DECLARE_REF_TYPE(T)

protected:
    mutable PointerType m_beg;
    mutable PointerType m_end;

    void swap(skQueueIncrementIterator& rhs)
    {
        skSwap(m_beg, rhs.m_beg);
        skSwap(m_end, rhs.m_end);
    }

public:
    skQueueIncrementIterator() :
        m_beg(0),
        m_end(0)
    {
    }

    skQueueIncrementIterator(PointerType begin, SKsize size, SKsize front) :
        m_beg(begin + front),
        m_end(begin + size)
    {
    }

    explicit skQueueIncrementIterator(T& v) :
        m_beg(v.ptr() + v.front()),
        m_end(v.ptr() + v.size())
    {
    }

    skQueueIncrementIterator(const skQueueIncrementIterator& rhs) :
        m_beg(rhs.m_beg),
        m_end(rhs.m_end)
    {
    }

    ~skQueueIncrementIterator()
    {
    }

    skQueueIncrementIterator& operator=(const skQueueIncrementIterator& rhs)
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
        return *m_beg++;
    }

    SK_INLINE ConstReferenceType getNext(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        return *m_beg++;
    }

    SK_INLINE void next(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        ++m_beg;
    }

    SK_INLINE ReferenceType peekNext(void)
    {
        SK_ITER_DEBUG(hasMoreElements());
        return *m_beg;
    }

    SK_INLINE ConstReferenceType peekNext(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        return *m_beg;
    }
};

template <typename T>
class skQueueDecrementIterator
{
public:
    SK_DECLARE_REF_TYPE(T)

protected:
    mutable PointerType m_beg;
    mutable PointerType m_end;

    void swap(skQueueDecrementIterator& rhs)
    {
        skSwap(m_beg, rhs.m_beg);
        skSwap(m_end, rhs.m_end);
    }

public:
    skQueueDecrementIterator() :
        m_beg(0),
        m_end(0)
    {
    }

    skQueueDecrementIterator(PointerType begin, SKsize size, SKsize front) :
        m_beg(begin + (size - 1)),
        m_end(begin + front)
    {
    }

    explicit skQueueDecrementIterator(T& v) :
        m_beg(v.ptr() + (v.size() - 1)),
        m_end(v.ptr() + v.front())
    {
    }

    skQueueDecrementIterator(const skQueueDecrementIterator& rhs) :
        m_beg(rhs.m_beg),
        m_end(rhs.m_end)
    {
    }

    ~skQueueDecrementIterator()
    {
    }

    skQueueDecrementIterator& operator=(const skQueueDecrementIterator& rhs)
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
        return *m_beg--;
    }

    SK_INLINE ConstReferenceType getNext(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        return *m_beg--;
    }

    SK_INLINE void next(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        --m_beg;
    }

    SK_INLINE ReferenceType peekNext(void)
    {
        SK_ITER_DEBUG(hasMoreElements());
        return *m_beg;
    }

    SK_INLINE ConstReferenceType peekNext(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        return *m_beg;
    }
};



template <typename T, typename Allocator = skAllocator<T, SKuint32> >
class skQueue : public skArrayBase<T, Allocator>
{
public:
    SK_DECLARE_TYPE(T)

    typedef skQueue<T, Allocator>                    SelfType;
    typedef skQueueIncrementIterator<SelfType>       Iterator;
    typedef skArrayBase<T, Allocator>                BaseType;
    typedef const skQueueIncrementIterator<SelfType> ConstIterator;
    typedef skQueueDecrementIterator<SelfType>       ReverseIterator;
    typedef const skQueueDecrementIterator<SelfType> ConstReverseIterator;
    typedef typename SelfType::SizeType              SizeType;

private:
    SizeType m_front, m_back;

public:
    skQueue() :
        BaseType(),
        m_front(0),
        m_back(0)
    {
    }


    skQueue(const skQueue& q) :
        BaseType(q),
        m_front(0),
        m_back(0)
    {
    }

    ~skQueue()
    {
        clear();
    }

    void clear(void)
    {
        this->destroy();
        m_front = 0;
        m_back  = 0;
    }


    void resize(SKsize nr)
    {
        this->resize(nr);
        m_front = 0;
        m_back  = nr;
    }

    void enqueue(ConstReferenceType value)
    {
        if (this->m_size > this->m_alloc.limit)  // provide an upper limit
            return;

        if (this->m_size + 1 > this->m_capacity)
        {
            this->reserve(this->m_size == 0 ? 8 : this->m_size * 2);
            m_back = this->m_size;
        }

        this->m_data[m_back] = value;
        ++this->m_size;

        m_back = (m_back + 1) % this->m_capacity;
    }

    ReferenceType pop_front(void)
    {
        return dequeue();
    }

    ReferenceType dequeue(void)
    {
        if (m_front >= this->m_capacity)
            throw m_front;


        ReferenceType returnValue = this->m_data[m_front];

        m_front = (m_front + 1) % this->m_capacity;
        --this->m_size;
        return returnValue;
    }

    SK_INLINE ReferenceType at(SKsize idx)
    {
        return (*this)[(m_front + idx) % (this->m_capacity - 1)];
    }

    SK_INLINE ConstReferenceType at(SKsize idx) const
    {
        return (*this)[(m_front + idx) % (this->m_capacity - 1)];
    }

    SK_INLINE ReferenceType operator[](SKsize idx)
    {
        SK_ASSERT(this->m_data);
        SK_ASSERT(idx != this->npos && idx < this->m_capacity);
        return this->m_data[(m_front + idx) % (this->m_capacity - 1)];
    }

    SK_INLINE ConstReferenceType operator[](SKsize idx) const
    {
        SK_ASSERT(this->m_data);
        SK_ASSERT(idx != this->npos && idx < this->m_capacity);
        return this->m_data[(m_front + idx) % (this->m_capacity - 1)];
    }

    SK_INLINE SKsize front(void) const
    {
        return m_front;
    }

    SK_INLINE SKsize back(void) const
    {
        return m_back;
    }

    SK_INLINE Iterator iterator(void)
    {
        return this->m_data && this->m_size > 0 ? 
            Iterator(this->m_data, this->m_size, m_front) : Iterator();
    }

    SK_INLINE ConstIterator iterator(void) const
    {
        return this->m_data && this->m_size > 0 ? 
            ConstIterator(this->m_data, this->m_size, m_front) : ConstIterator();
    }

    SK_INLINE ReverseIterator reverseIterator(void)
    {
        return this->m_data && this->m_size > 0 ? 
            ReverseIterator(this->m_data, this->m_size, m_front) : ReverseIterator();
    }

    SK_INLINE ConstReverseIterator reverseIterator(void) const
    {
        return this->m_data && this->m_size > 0 ? 
            ConstReverseIterator(this->m_data, this->m_size, m_front) : ConstReverseIterator();
    }
};

#endif  //_skQueue_h_
