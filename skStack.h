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
    SK_DECLARE_TYPE(T);

public:
    typedef skStack<T, Allocator>     SelfType;
    typedef skArrayBase<T, Allocator> BaseType;

    typedef skPointerDecrementIterator<SelfType>       Iterator;
    typedef const skPointerDecrementIterator<SelfType> ConstIterator;

public:
    skStack() :
        m_top(0)
    {
    }

    skStack(SKuint32 initialCapacity) :
        m_top(0)
    {
        reserve(initialCapacity);
    }

    skStack(const SelfType& o) :
        skArrayBase(o)
    {
        m_top = m_size;
    }

    ~skStack()
    {
        clear();
    }


    void clear(void)
    {
        destroy();
        m_top = 0;
    }

    void push(ConstReferenceType v)
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

            m_top = m_size;
        }
        else
            throw m_alloc.limit;
    }

    void pop(void)
    {
        if (m_size > 0)
        {
            m_alloc.destroy(&m_data[m_size]);
            m_size--;
            m_top = m_size;
        }
    }


    void pop(SKuint32 nr)
    {
        while (nr-- && !empty())
            pop();
        m_top = m_size;
    }


    SK_INLINE ReferenceType top(void)
    {
        SK_ASSERT(m_top != 0);
        return m_data[itop()];
    }

    SK_INLINE ConstReferenceType top(void) const
    {
        SK_ASSERT(m_top != 0);
        return m_data[itop()];
    }

    SK_INLINE ReferenceType peek(SKuint32 offs)
    {
        SK_ASSERT(m_top != 0 && (m_top - 1 - offs) != SK_NPOS32);
        return m_data[m_top - 1 - offs];
    }

    SK_INLINE ConstReferenceType peek(SKuint32 offs) const
    {
        return peek(offs);
    }

    SK_INLINE SKuint32 itop(void) const
    {
        return empty() ? 0 : m_top - 1;
    }

    SK_INLINE ConstPointerType ptr(void) const
    {
        return m_data;
    }

    SK_INLINE PointerType ptr(void)
    {
        return m_data;
    }

    SK_INLINE Iterator iterator(void)
    {
        return Iterator(m_data, m_size);
    }

    SK_INLINE ConstIterator iterator(void) const
    {
        return ConstIterator(m_data, m_size);
    }

    SelfType& operator=(const SelfType& rhs)
    {
        replicate(rhs);
        m_top = m_size;
        return *this;
    }

private:
    SKuint32    m_top;
};

#endif  //_skStack_h_
