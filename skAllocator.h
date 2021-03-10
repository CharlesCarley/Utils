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
#ifndef _skAllocator_h_
#define _skAllocator_h_

#include "Config/skConfig.h"
#include "skMemoryUtils.h"
#include "skMinMax.h"
#include "skTraits.h"

template <typename T, typename UnsignedSizeType, const UnsignedSizeType AllocLimit>
class skAllocBase
{
public:
    SK_DECLARE_TYPE(T)

    typedef UnsignedSizeType SizeType;

    static const SizeType npos;
    static const SizeType limit;

    void fill(PointerType dst, ConstPointerType src, const SizeType capacity)
    {
        if (capacity > 0 && capacity < limit && capacity != npos)
        {
            SKsize i = 0;
            do
                dst[i] = src[i];
            while (++i < capacity);
        }
    }

    static void construct(PointerType base, ConstReferenceType v)
    {
        new (base) T(v);
    }

    static void construct(PointerType base)
    {
        new (base) T();
    }

    template <typename A0>
    static void construct_arg(PointerType base, const A0& v)
    {
        new (base) T(v);
    }

    static void destroy(PointerType base)
    {
        if (base)
            base->~T();
    }

    static void destroy(PointerType beg, PointerType end)
    {
        while (beg && beg != end)
        {
            beg->~T();
            ++beg;
        }
    }

protected:
    static void construct(PointerType beg, ConstPointerType end, ConstReferenceType value)
    {
        while (beg != end)
        {
            new (beg) T(value);
            ++beg;
        }
    }

    static void construct(PointerType beg, ConstPointerType end)
    {
        while (beg != end)
        {
            new (beg) T();
            ++beg;
        }
    }

    void enforce_limit(SizeType capacity)
    {
        if (capacity > limit)
            throw(SizeType)(capacity - limit);
    }
};

template <typename T, typename SizeType, const SizeType AllocLimit>
const SizeType skAllocBase<T, SizeType, AllocLimit>::limit = AllocLimit;
template <typename T, typename SizeType, const SizeType AllocLimit>
const SizeType skAllocBase<T, SizeType, AllocLimit>::npos = SK_MKNPOS(SizeType);

template <typename T,
          typename SizeType         = SKsize,
          const SizeType AllocLimit = SK_MKMX(SizeType)>
class skMallocAllocator : public skAllocBase<T, SizeType, AllocLimit>
{
public:
    SK_DECLARE_TYPE(T)

public:
    typedef skMallocAllocator<T, SizeType, AllocLimit> SelfType;

public:
    explicit skMallocAllocator()
    {
    }

    explicit skMallocAllocator(const SelfType&)
    {
    }

    ~skMallocAllocator()
    {
    }

    PointerType allocate(void)
    {
        PointerType base = static_cast<PointerType>(skMalloc(sizeof(T)));
        this->construct(base);
        return base;
    }

    void deallocate(PointerType base)
    {
        this->destroy(base);
        skFree(base);
    }

    static PointerType allocate_base(void)
    {
        return static_cast<PointerType>(skMalloc(sizeof(T)));
    }

    static void deallocate_base(PointerType& base)
    {
        skFree(base);
    }

    PointerType array_allocate(SizeType capacity)
    {
        this->enforce_limit(capacity);

        PointerType base = static_cast<PointerType>(skMalloc(sizeof(T) * capacity));
        this->construct(base, base + capacity);
        return base;
    }

    PointerType array_allocate(SizeType capacity, ConstReferenceType val)
    {
        this->enforce_limit(capacity);

        PointerType base = static_cast<PointerType>(
            skMalloc(sizeof(T) * capacity));

        this->construct(base, base + capacity, val);
        return base;
    }

    PointerType array_reallocate(PointerType oldPtr, SizeType capacity, SizeType os)
    {
        this->enforce_limit(capacity);

        PointerType base = static_cast<PointerType>(
            skRealloc(oldPtr, sizeof(T) * capacity));

        if (oldPtr)
            this->construct(base + os, base + capacity);
        else
            this->construct(base, base + capacity);
        return base;
    }

    void array_deallocate(PointerType base, SizeType capacity)
    {
        capacity = skMin<SizeType>(capacity, SelfType::limit);
        this->destroy(base, base + capacity);
        skFree(base);
    }

    static void array_deallocate(PointerType base)
    {
        skFree(base);
    }
};

template <typename T,
          typename SizeType          = SKsize,
          const SizeType alloc_limit = SK_MKMX(SizeType)>
class skNewAllocator : public skAllocBase<T, SizeType, alloc_limit>
{
public:
    SK_DECLARE_TYPE(T)

public:
    typedef skNewAllocator<T, SizeType, alloc_limit> SelfType;

public:
    explicit skNewAllocator() = default;

    explicit skNewAllocator(const SelfType&)
    {
    }

    ~skNewAllocator() = default;

    static PointerType allocate(void)
    {
        return new T;
    }

    void deallocate(PointerType base)
    {
        this->destroy(base);
        operator delete(base);
    }

    PointerType array_allocate(SizeType capacity)
    {
        this->enforce_limit(capacity);
        return new T[capacity];
    }

    PointerType array_reallocate(PointerType old, SizeType capacity, SizeType old_nr)
    {
        this->enforce_limit(capacity);

        PointerType base = new T[capacity];
        if (old)
        {
            this->fill(base, old, old_nr);
            delete[] old;
        }
        return base;
    }

    static void array_deallocate(PointerType base, SizeType)
    {
        delete[] base;
    }
};

#if SK_ALLOCATOR == 1
#define skAllocator skMallocAllocator
#else
#define skAllocator skNewAllocator
#endif

#endif  //_skAllocator_h_
