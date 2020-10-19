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
#ifndef _skStack_h_
#define _skStack_h_

#include "Config/skConfig.h"
#include "skArrayBase.h"
#include "skTraits.h"

template <typename T, typename Allocator = skAllocator<T, SKuint32> >
class skStack : public skArrayBase<T, Allocator>
{
public:
    SK_DECLARE_TYPE(T)

    typedef skStack<T, Allocator>       SelfType;
    typedef skArrayBase<T, Allocator>   BaseType;
    typedef typename BaseType::SizeType SizeType;

    typedef skPointerDecrementIterator<SelfType>       Iterator;
    typedef const skPointerDecrementIterator<SelfType> ConstIterator;

public:
    skStack() :
        BaseType()
    {
    }

    skStack(SizeType initialCapacity) :
        BaseType()
    {
        this->reserve(initialCapacity);
    }

    skStack(const SelfType& o) :
        BaseType(o)
    {
    }

    ~skStack()
    {
        clear();
    }

    void clear(void)
    {
        this->destroy();
    }

    void push(ConstReferenceType v)
    {
        if (this->m_size + 1 > this->m_capacity)
            this->reserve(this->m_capacity == 0 ? SKInitalCap : this->m_capacity * 2);

        if (this->m_data)
            this->m_data[this->m_size++] = v;
    }

    void pop(void)
    {
        if (this->m_size > 0)
        {
            this->m_alloc.destroy(&this->m_data[this->m_size]);
            --this->m_size;
        }
    }

    void pop(SizeType nr)
    {
        while (nr-- && !this->empty())
            pop();
    }

    SK_INLINE ReferenceType top(void)
    {
        SK_ASSERT(this->m_size != 0);
        return this->m_data[itop()];
    }

    SK_INLINE ConstReferenceType top(void) const
    {
        SK_ASSERT(this->m_size != 0);
        return this->m_data[itop()];
    }

    SK_INLINE ReferenceType peek(SizeType offs)
    {
        SK_ASSERT(this->m_size != 0 && (this->m_size - 1 - offs) != this->m_alloc.npos);
        return this->m_data[this->m_size - 1 - offs];
    }

    SK_INLINE ConstReferenceType peek(SizeType offs) const
    {
        return peek(offs);
    }

    SK_INLINE SizeType itop(void) const
    {
        return this->empty() ? 0 : this->m_size - 1;
    }

    SK_INLINE Iterator iterator(void)
    {
        return Iterator(this->m_data, this->m_size);
    }

    SK_INLINE ConstIterator iterator(void) const
    {
        return ConstIterator(this->m_data, this->m_size);
    }

    skStack<T, Allocator>& operator=(const SelfType& rhs)
    {
        if (this != &rhs)
        {
            this->replicate(rhs);
        }
        return *this;
    }
};

#endif  //_skStack_h_
