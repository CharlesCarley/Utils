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
#include "skArray.h"
#include "skTraits.h"

template <typename T, 
    typename Allocator = skAllocator<T> >
class skStack
{
public:
    SK_DECLARE_TYPE(T);

public:
    typedef skArray<T, Allocator> StackType;
    typedef skStack<T, Allocator>    SelfType;

    typedef SKuint32 SizeType;

    typedef skPointerDecrementIterator<StackType>       Iterator;
    typedef const skPointerDecrementIterator<StackType> ConstIterator;

    typedef skPointerIncrementIterator<StackType>       ReverseIterator;
    typedef const skPointerIncrementIterator<StackType> ConstReverseIterator;

    const SizeType npos = -1;

    SK_IMPLEMENT_SORT(T, skStack);

public:
    skStack() :
        m_top(0)
    {
    }

    skStack(SizeType rsp) :
        m_top(0)
    {
        m_stack.reserve(rsp);
    }

    skStack(const SelfType& o) :
        m_top(o.m_top),
        m_stack(o.m_stack)
    {
    }

    ~skStack()
    {
        m_stack.clear();
    }

    SK_INLINE void clear(void)
    {
        m_stack.clear();
        m_top = 0;
    }

    SK_INLINE void push(ConstReferenceType v)
    {
        m_stack.push_back(v);
        m_top = m_stack.size();
    }

    SK_INLINE void pop(void)
    {
        m_stack.pop_back();
        m_top = m_stack.size();
    }

    SK_INLINE void pop(SizeType nr)
    {
        while (nr-- && !m_stack.empty())
            m_stack.pop_back();

        m_top = m_stack.size();
    }

    SK_INLINE ReferenceType top(void)
    {
        SK_ASSERT(m_top != 0);
        return m_stack.at(m_top - 1);
    }

    SK_INLINE ConstReferenceType top(void) const
    {
        SK_ASSERT(m_top != 0);
        return m_stack.at(m_top - 1);
    }

    SK_INLINE ReferenceType peek(SizeType offs)
    {
        SK_ASSERT(m_top != 0 && ((m_top - 1) - offs) != npos);
        return m_stack.at((m_top - 1) - offs);
    }

    SK_INLINE ConstReferenceType peek(SizeType offs) const
    {
        SK_ASSERT(m_top != 0 && ((m_top - 1) - offs) != npos);
        return m_stack.at((m_top - 1) - offs);
    }

    void reserve(SizeType nr)
    {
        m_stack.reserve(nr);
    }

    SK_INLINE SizeType capacity(void) const
    {
        return m_stack.capacity();
    }
    SK_INLINE SizeType size(void) const
    {
        return m_stack.size();
    }
    SK_INLINE SizeType itop(void) const
    {
        return m_top - 1;
    }
    SK_INLINE bool empty(void) const
    {
        return m_stack.empty();
    }
    SK_INLINE ConstPointerType ptr(void) const
    {
        return m_stack.ptr();
    }
    SK_INLINE PointerType ptr(void)
    {
        return m_stack.ptr();
    }

    SK_INLINE Iterator iterator(void)
    {
        return m_stack.reverseIterator();
    }

    SK_INLINE ConstIterator iterator(void) const
    {
        return m_stack.reverseIterator();
    }

    SK_INLINE ReverseIterator reverseIterator(void)
    {
        return m_stack.iterator();
    }

    SK_INLINE ConstReverseIterator reverseIterator(void) const
    {
        return m_stack.iterator();
    }

    SelfType& operator=(const SelfType& rhs)
    {
        if (this != &rhs)
        {
            m_stack = rhs.m_stack;
            m_top   = rhs.m_top;
        }

        return *this;
    }

private:
    mutable StackType m_stack;
    SizeType          m_top;
};

#endif  //_skStack_h_
