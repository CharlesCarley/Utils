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
#include "skSort.h"

template <typename T, typename Allocator = skAllocator<T, SKuint32> >
class skArray : public skArrayBase<T, Allocator>
{
public:
    SK_DECLARE_TYPE(T)

    typedef skArray<T, Allocator>        SelfType;
    typedef skArrayBase<T, Allocator>    BaseType;
    typedef typename Allocator::SizeType SizeType;

    typedef skPointerIncrementIterator<SelfType, SizeType>       Iterator;
    typedef const skPointerIncrementIterator<SelfType, SizeType> ConstIterator;
    typedef skPointerDecrementIterator<SelfType, SizeType>       ReverseIterator;
    typedef const skPointerDecrementIterator<SelfType, SizeType> ConstReverseIterator;

    SK_IMPLEMENT_QSORT(T, skArray, SizeType)

public:
    skArray() :
        BaseType()
    {
    }

    skArray(const skArray& o) :
        BaseType(o)
    {
    }

    skArray(const SizeType& initialCapacity) :
        BaseType(initialCapacity)
    {
    }


    ~skArray()
    {
        clear();
    }

    void clear(void)
    {
        this->destroy();
    }

    void push_back(ConstReferenceType v)
    {
        if (this->m_size + 1 <= this->m_alloc.limit)
        {
            // If the size of the array is known ahead of time
            // and the data is reserved before pushing any elements.
            // The reserved size should be plus one.
            // This needs to know that the next element to
            // push will not overflow the array.
            if (this->m_size + 1 > this->m_capacity)
            {
                this->reserve(this->m_size == 0 ? SKInitalCap : this->m_size * 2);
                this->m_data[this->m_size++] = v;
            }
            else
                this->m_data[this->m_size++] = v;
        }
        else
            throw this->m_alloc.limit;
    }

    void pop_back(void)
    {
        if (this->m_size > 0)
        {
            this->m_alloc.destroy(&this->m_data[this->m_size]);
            --this->m_size;
        }
    }

    void erase(ConstReferenceType v)
    {
        remove(this->find(v));
    }

    void remove(SizeType pos)
    {
        if (this->m_size > 0)
        {
            if (pos != this->m_alloc.npos)
            {
                skSwap(this->m_data[pos], this->m_data[this->m_size - 1]);
                this->m_alloc.destroy(&this->m_data[--this->m_size]);
            }
        }
    }

    SK_INLINE ReferenceType operator[](SizeType idx)
    {
        SK_ASSERT(idx >= 0 && idx < this->m_capacity);
        return this->m_data[idx];
    }

    SK_INLINE ConstReferenceType operator[](SizeType idx) const
    {
        SK_ASSERT(idx >= 0 && idx < this->m_capacity);
        return this->m_data[idx];
    }

    SK_INLINE ReferenceType at(SizeType idx)
    {
        SK_ASSERT(idx >= 0 && idx < this->m_capacity);
        return this->m_data[idx];
    }

    SK_INLINE ConstReferenceType at(SizeType idx) const
    {
        SK_ASSERT(idx >= 0 && idx < this->m_capacity);
        return this->m_data[idx];
    }

    SK_INLINE ReferenceType front(void)
    {
        SK_ASSERT(this->m_size > 0);
        return this->m_data[0];
    }

    SK_INLINE ReferenceType back(void)
    {
        SK_ASSERT(this->m_size > 0);
        return this->m_data[this->m_size - 1];
    }

    SK_INLINE ConstReferenceType front(void) const
    {
        SK_ASSERT(this->m_size > 0);
        return this->m_data[0];
    }

    SK_INLINE ConstReferenceType back(void) const
    {
        SK_ASSERT(this->m_size > 0);
        return this->m_data[this->m_size - 1];
    }

    SK_INLINE PointerType begin(void)
    {
        if (this->m_size > 0)
            return &this->m_data[0];
        return nullptr;
    } 

    SK_INLINE PointerType end(void)
    {
        if (this->m_size > 0)
            return &this->m_data[this->m_size];
        return nullptr;
    } 


    SK_INLINE ConstPointerType begin(void) const
    {
        if (this->m_size > 0)
            return &this->m_data[0];
        return nullptr;
    } 

    SK_INLINE ConstPointerType end(void) const
    {
        if (this->m_size > 0)
            return &this->m_data[this->m_size];
        return nullptr;
    } 


    SK_INLINE Iterator iterator(void)
    {
        return this->m_data && this->m_size > 0 ? Iterator(this->m_data, this->m_size) : Iterator();
    }

    SK_INLINE ConstIterator iterator(void) const
    {
        return this->m_data && this->m_size > 0 ? ConstIterator(this->m_data, this->m_size) : ConstIterator();
    }

    SK_INLINE ReverseIterator reverseIterator(void)
    {
        return this->m_data && this->m_size > 0 ? ReverseIterator(this->m_data, this->m_size) : ReverseIterator();
    }

    SK_INLINE ConstReverseIterator reverseIterator(void) const
    {
        return this->m_data && this->m_size > 0 ? ConstReverseIterator(this->m_data, this->m_size) : ConstReverseIterator();
    }

    skArray& operator=(const skArray& rhs)
    {
        this->replicate(rhs);
        return *this;
    }
};

#endif  //_skArray_h_
