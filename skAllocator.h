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
#include "skTraits.h"

#if SK_ALLOCATOR == 1
#define SK_DECLARE_ALLOC                              \
public:                                               \
    inline void* operator new(size_t size)            \
    {                                                 \
        return skMalloc(size);                        \
    }                                                 \
    inline void operator delete(void* ptr)            \
    {                                                 \
        skFree(ptr);                                  \
    }                                                 \
    inline void* operator new[](size_t size)          \
    {                                                 \
        return skMalloc(size);                        \
    }                                                 \
    inline void operator delete[](void* ptr)          \
    {                                                 \
        skFree(ptr);                                  \
    }                                                 \
    inline void* operator new(size_t size, void* ptr) \
    {                                                 \
        return ptr;                                   \
    }                                                 \
    inline void operator delete(void* ptr, void*)     \
    {                                                 \
    }

#else
#define SK_DECLARE_ALLOC
#endif

class skAllocObject
{
public:
    virtual ~skAllocObject()
    {
    }

    SK_DECLARE_ALLOC;
};


template <typename T, typename UnsignedSizeType, const UnsignedSizeType alloc_limit>
class skAllocBase
{
public:
    SK_DECLARE_TYPE(T);
    typedef UnsignedSizeType SizeType;

    static const SizeType npos;
    static const SizeType limit;


    void fill(PointerType dst, ConstPointerType src, const SizeType nr)
    {
        if (nr > 0 && nr <= limit && nr != npos)
        {
            SKsize i = 0;
            do
                dst[i] = src[i];
            while (++i < nr);
        }
    }
};

template <typename T, typename SizeType, const SizeType alloc_limit>
const SizeType skAllocBase<T, SizeType, alloc_limit>::limit = alloc_limit;

template <typename T, typename SizeType, const SizeType alloc_limit>
const SizeType skAllocBase<T, SizeType, alloc_limit>::npos = SK_MKNPOS(SizeType);



template <typename T,
          typename SizeType   = SKsize,
          const SizeType alloc_limit = SK_MKMX(SizeType)>
class skMallocAllocator : public skAllocBase<T, SizeType, alloc_limit>
{
public:
    SK_DECLARE_TYPE(T);

public:
    typedef skMallocAllocator<T, SizeType, alloc_limit> SelfType;

    template <typename U>
    struct rebind
    {
        typedef skMallocAllocator<U, SizeType, alloc_limit> other;
    };

public:
    explicit skMallocAllocator()
    {
    }

    ~skMallocAllocator()
    {
    }

    explicit skMallocAllocator(const SelfType&)
    {
    }

    template <typename U>
    explicit skMallocAllocator(const skMallocAllocator<U>&)
    {
    }

    SK_INLINE PointerType address(ReferenceType v)
    {
        return &v;
    }
    SK_INLINE ConstPointerType address(ConstReferenceType v) const
    {
        return &v;
    }

    SK_INLINE void construct(PointerType p, ConstReferenceType v)
    {
        new (p) T(v);
    }

    template <typename A0>
    void construct_arg(PointerType p, const A0& v)
    {
        new (p) T(v);
    }

    PointerType allocate(SKsize nr)
    {
        PointerType p = reinterpret_cast<PointerType>(skMalloc(sizeof(T) * nr));
        skConstruct(p, p + nr, T());
        return p;
    }

    void deallocate(PointerType p)
    {
        destroy(p);
        skFree(p);
    }

    PointerType array_allocate(SKsize nr)
    {
        PointerType p = reinterpret_cast<PointerType>(skMalloc(sizeof(T) * nr));
        skConstructDefault(p, p + nr);
        return p;
    }

    PointerType array_reallocate(PointerType op, SKsize nr, SKsize os)
    {
        PointerType p = reinterpret_cast<PointerType>(skRealloc(op, sizeof(T) * nr));

        if (op)
            skConstructDefault(p + os, p + nr);
        else
            skConstructDefault(p, p + nr);

        return p;
    }

    void array_deallocate(PointerType p, SKsize nr)
    {
        skDestruct(p, p + nr);
        skFree(p);
    }

    SK_INLINE void destroy(PointerType p)
    {
        if (p)
            p->~T();
    }

    void destroy_range(PointerType beg, PointerType end)
    {
        skDestruct(beg, end);
    }

};

template <typename T,
          typename SizeType   = SKsize,
          const SizeType alloc_limit = SK_MKMX(SizeType)>
class skNewAllocator : public skAllocBase<T, SizeType, alloc_limit>
{
public:
    SK_DECLARE_TYPE(T);

public:
    typedef skNewAllocator<T, SizeType, alloc_limit> SelfType;

    template <typename U>
    struct rebind
    {
        typedef skNewAllocator<U, SizeType, alloc_limit> other;
    };

public:
 
    explicit skNewAllocator()
    {
    }

    ~skNewAllocator()
    {
    }

    explicit skNewAllocator(const SelfType&)
    {
    }

    template <typename U>
    explicit skNewAllocator(const skNewAllocator<U, SizeType, alloc_limit>&)
    {
    }

    SK_INLINE PointerType address(ReferenceType v)
    {
        return &v;
    }

    SK_INLINE ConstPointerType address(ConstReferenceType v) const
    {
        return &v;
    }

    void construct(PointerType p, ConstReferenceType v)
    {
        new (p) T(v);
    }

    template <typename A0>
    void construct_arg(PointerType p, const A0& v)
    {
        new (p) T(v);
    }

    PointerType allocate(SKsize nr)
    {
        const PointerType p = reinterpret_cast<PointerType>(new T(sizeof(T) * nr));
        new (p) T();
        return p;
    }

    void deallocate(PointerType p)
    {
        destroy(p);
        operator delete(p);
    }

    PointerType array_allocate(SKsize nr)
    {
        return new T[nr];
    }

    PointerType array_reallocate(ConstPointerType op, SKsize nr, SKsize os)
    {
        PointerType p = new T[nr];
        if (op)
        {
            SelfType::fill(p, op, os);
            delete[] op;
        }

        return p;
    }

    void array_deallocate(PointerType p, SKsize)
    {
        delete[] p;
    }

    void destroy(PointerType p)
    {
        if (p) p->~T();
    }

    void destroy_range(PointerType beg, PointerType end)
    {
        skDestruct(beg, end);
    }
};


#if SK_ALLOCATOR == 1
#define skAllocator skMallocAllocator
#else
#define skAllocator skNewAllocator
#endif

#endif  //_skAllocator_h_
