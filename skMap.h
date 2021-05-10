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
#ifndef _skMap_h_
#define _skMap_h_

#include "skArray.h"
#include "skHash.h"

template <typename T, typename SizeType = SKsize>
class skHashTableIncrementIterator : public skPointerIncrementIterator<T, SizeType>
{
public:
    typedef typename T::PairValueType               PairValueType;
    typedef typename T::PairKeyType                 PairKeyType;
    typedef typename T::PointerType                 PointerType;
    typedef skPointerIncrementIterator<T, SizeType> BaseType;

public:
    skHashTableIncrementIterator()
    {
    }

    skHashTableIncrementIterator(PointerType begin, SizeType size) :
        BaseType(begin, size)
    {
    }

    explicit skHashTableIncrementIterator(T& v) :
        BaseType(v)
    {
    }

    SK_INLINE PairKeyType& peekNextKey(void)
    {
        SK_ITER_DEBUG(hasMoreElements());
        return (*BaseType::m_beg).first;
    }

    SK_INLINE PairValueType& peekNextValue(void)
    {
        SK_ITER_DEBUG(hasMoreElements());
        return (*BaseType::m_beg).second;
    }

    SK_INLINE const PairKeyType& peekNextKey(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        return (*BaseType::m_beg).first;
    }

    SK_INLINE const PairValueType& peekNextValue(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        return (*BaseType::m_beg).second;
    }
};

template <typename T, typename SizeType = SKsize>
class skHashTableDecrementIterator : public skPointerDecrementIterator<T, SizeType>
{
public:
    typedef typename T::PairValueType               PairValueType;
    typedef typename T::PairKeyType                 PairKeyType;
    typedef typename T::PointerType                 PointerType;
    typedef skPointerDecrementIterator<T, SizeType> BaseType;

public:
    skHashTableDecrementIterator()
    {
    }

    skHashTableDecrementIterator(PointerType begin, SizeType size) :
        BaseType(begin, size)
    {
    }

    explicit skHashTableDecrementIterator(T& v) :
        BaseType(v)
    {
    }

    SK_INLINE PairKeyType& peekNextKey(void)
    {
        SK_ITER_DEBUG(hasMoreElements());
        return (*BaseType::m_beg).first;
    }

    SK_INLINE PairValueType& peekNextValue(void)
    {
        SK_ITER_DEBUG(hasMoreElements());
        return (*BaseType::m_beg).second;
    }

    SK_INLINE const PairKeyType& peekNextKey(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        return (*BaseType::m_beg).first;
    }

    SK_INLINE const PairValueType& peekNextValue(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        return (*BaseType::m_beg).second;
    }
};

#define SK_HASHTABLE_POW2(x) \
    --(x);                   \
    (x) |= (x) >> 16;        \
    (x) |= (x) >> 8;         \
    (x) |= (x) >> 4;         \
    (x) |= (x) >> 2;         \
    (x) |= (x) >> 1;         \
    ++(x);
#define SK_HASHTABLE_IS_POW2(x) ((x) && !(((x)-1) & (x)))

template <typename Key, typename Value>
class skEntry
{
public:
    Key    first;
    Value  second;
    SKhash hash;

    skEntry() :
        first(),
        second(),
        hash(SK_NPOS)
    {
    }

    skEntry(const Key& k, const Value& v, SKhash hk) :
        first(k),
        second(v),
        hash(hk)
    {
    }

    explicit skEntry(const skEntry& oth) :
        first(oth.first),
        second(oth.second),
        hash(oth.hash)
    {
    }

    ~skEntry()
    {
    }
};

// Derived from btHashTable
// https://github.com/bulletphysics/bullet3/blob/master/src/LinearMath/btHashMap.h
template <typename Key,
          typename Value,
          typename Allocator = skAllocator<skEntry<Key, Value>, SKsize> >
class skHashTable
{
public:
    typedef skAllocator<SKsize>                IndexAllocator;
    typedef skHashTable<Key, Value, Allocator> SelfType;

public:
    typedef skEntry<Key, Value> Pair;
    SK_DECLARE_TYPE(Pair)

    typedef SKsize*                  IndexArray;
    typedef Key                      PairKeyType;
    typedef Value                    PairValueType;
    typedef IndexAllocator::SizeType SizeType;

    typedef skHashTableIncrementIterator<SelfType>       Iterator;
    typedef const skHashTableIncrementIterator<SelfType> ConstIterator;

    typedef skHashTableDecrementIterator<SelfType>       ReverseIterator;
    typedef const skHashTableDecrementIterator<SelfType> ConstReverseIterator;

    // TODO: this needs to use the type defined via template parameters.
    const SKsize npos = SK_NPOS;

public:
    skHashTable() :
        m_size(0),
        m_capacity(0),
        m_iPtr(nullptr),
        m_nPtr(nullptr),
        m_bPtr(nullptr)
    {
    }

    skHashTable(SKsize initialCapacity) :
        m_size(0),
        m_capacity(0),
        m_iPtr(nullptr),
        m_nPtr(nullptr),
        m_bPtr(nullptr)
    {
        reserve(initialCapacity);
    }

    skHashTable(const skHashTable& rhs) :
        m_size(0),
        m_capacity(0),
        m_iPtr(nullptr),
        m_nPtr(nullptr),
        m_bPtr(nullptr)
    {
        copy(rhs);
    }

    ~skHashTable()
    {
        clear();
    }

    skHashTable<Key, Value, Allocator>& operator=(const SelfType& rhs)
    {
        if (this != &rhs)
            copy(rhs);
        return *this;
    }

    void clear(void)
    {
        m_alloc.array_deallocate(m_bPtr, m_capacity);
        m_iAlloc.array_deallocate(m_iPtr, m_capacity);
        m_iAlloc.array_deallocate(m_nPtr, m_capacity);

        m_bPtr = 0;
        m_iPtr = nullptr;
        m_nPtr = nullptr;
        m_size = m_capacity = 0;
    }

    SK_INLINE Value& at(SKsize i)
    {
        SK_ASSERT(m_bPtr && i < m_size);
        return m_bPtr[i].second;
    }

    SK_INLINE Value& operator[](SKsize i)
    {
        SK_ASSERT(m_bPtr && i < m_size);
        return m_bPtr[i].second;
    }

    SK_INLINE const Value& at(SKsize i) const
    {
        SK_ASSERT(m_bPtr && i < m_size);
        return m_bPtr[i].second;
    }

    SK_INLINE const Value& operator[](SKsize i) const
    {
        SK_ASSERT(m_bPtr && i < m_size);
        return m_bPtr[i].second;
    }

    SK_INLINE Key& keyAt(SKsize i)
    {
        SK_ASSERT(m_bPtr && i < m_size);
        return m_bPtr[i].first;
    }

    SK_INLINE const Key& keyAt(SKsize i) const
    {
        SK_ASSERT(m_bPtr && i < m_size);
        return m_bPtr[i].first;
    }

    Value* get(const Key& key)
    {
        SKsize i = find(key);
        if (i == npos)
            return nullptr;
        return &m_bPtr[i].second;
    }

    Value* operator[](const Key& key)
    {
        return get(key);
    }

    const Value* operator[](const Key& key) const
    {
        return get(key);
    }

    SKsize find(const Key& key) const
    {
        if (empty())
            return npos;

        SKhash       hk = skHash(key);
        const SKhash hr = hk & m_capacity - 1;
        SKsize       fh = m_iPtr[hr];

        while (fh != npos && hk != m_bPtr[fh].hash)
            fh = m_nPtr[fh];

        return fh;
    }

    bool insert(const Key& key, const Value& val)
    {
        if (!empty())
        {
            if (find(key) != npos)
                return false;
        }

        if (m_size == m_capacity)
            reserve(m_size == 0 ? 32 : m_size * 2);

        SKhash       hk = skHash(key);
        const SKhash hr = hk & m_capacity - 1;

        m_bPtr[m_size] = Pair(key, val, hk);
        m_nPtr[m_size] = m_iPtr[hr];
        m_iPtr[hr]     = m_size++;
        return true;
    }

    void erase(const Key& key)
    {
        remove(key);
    }

    void remove(const Key& key)
    {
        SKhash hash, lHash;
        SKsize index, pIndex, fIndex;

        if (empty())
            return;

        fIndex = find(key);
        hash   = skHash(key) & m_capacity - 1;

        index  = m_iPtr[hash];
        pIndex = npos;

        while (index != fIndex)
        {
            pIndex = index;
            index  = m_nPtr[index];
        }

        if (pIndex != npos)
        {
            SK_ASSERT(m_nPtr[pIndex] == fIndex);
            m_nPtr[pIndex] = m_nPtr[fIndex];
        }
        else
            m_iPtr[hash] = m_nPtr[fIndex];

        SKsize lIndex = m_size - 1;
        if (lIndex == fIndex)
        {
            --m_size;
            m_bPtr[m_size].~Pair();
            return;
        }

        lHash = m_bPtr[lIndex].hash & m_capacity - 1;
        index = m_iPtr[lHash];

        pIndex = npos;
        while (index != lIndex)
        {
            pIndex = index;
            index  = m_nPtr[index];
        }

        if (pIndex != npos)
        {
            SK_ASSERT(m_nPtr[pIndex] == lIndex);
            m_nPtr[pIndex] = m_nPtr[lIndex];
        }
        else
            m_iPtr[lHash] = m_nPtr[lIndex];

        m_bPtr[fIndex] = m_bPtr[lIndex];
        m_nPtr[fIndex] = m_iPtr[lHash];
        m_iPtr[lHash]  = fIndex;

        --m_size;
        m_bPtr[m_size].~Pair();
    }

    SK_INLINE PointerType ptr(void)
    {
        return m_bPtr;
    }

    SK_INLINE ConstPointerType ptr(void) const
    {
        return m_bPtr;
    }

    SK_INLINE bool valid(void) const
    {
        return m_bPtr != nullptr;
    }

    SK_INLINE SKsize size(void) const
    {
        return m_size;
    }

    SK_INLINE SKsize capacity(void) const
    {
        return m_capacity;
    }

    SK_INLINE bool empty(void) const
    {
        return m_size == 0;
    }

    Iterator iterator(void)
    {
        return m_bPtr && m_size > 0 ? Iterator(m_bPtr, m_size) : Iterator();
    }

    ConstIterator iterator(void) const
    {
        return m_bPtr && m_size > 0 ? ConstIterator(m_bPtr, m_size) : ConstIterator();
    }

    ReverseIterator reverseIterator(void)
    {
        return m_bPtr && m_size > 0 ? ReverseIterator(m_bPtr, m_size) : ReverseIterator();
    }

    ConstReverseIterator reverseIterator(void) const
    {
        return m_bPtr && m_size > 0 ? ConstReverseIterator(m_bPtr, m_size) : ConstReverseIterator();
    }

    void reserve(SKsize nr)
    {
        if (m_capacity < nr && nr != npos)
            rehash(nr);
    }

private:
    void _zeroIndices(SKsize from, SKsize to) const
    {
        if (to <= 0 || from >= to)
            return;

        SKsize i = from;
        do
            m_iPtr[i] = m_nPtr[i] = npos;
        while (++i < to);
    }

    void copy(const SelfType& rhs)
    {
        if (rhs.valid() && !rhs.empty())
        {
            reserve(rhs.m_capacity);

            SKsize i, b;
            m_size     = rhs.m_size;
            m_capacity = rhs.m_capacity;

            b = m_size > 0 ? m_size - 1 : 0;
            _zeroIndices(b, m_capacity);

            for (i = 0; i < m_size; ++i)
            {
                m_bPtr[i] = rhs.m_bPtr[i];
                m_iPtr[i] = rhs.m_iPtr[i];
                m_nPtr[i] = rhs.m_nPtr[i];
            }
        }
    }

    void rehash(SKsize nr)
    {
        if (!SK_HASHTABLE_IS_POW2(nr))
        {
            SK_HASHTABLE_POW2(nr);
        }

        m_bPtr = m_alloc.array_reallocate(m_bPtr, nr, m_capacity);
        m_iPtr = m_iAlloc.array_reallocate(m_iPtr, nr, m_capacity);
        m_nPtr = m_iAlloc.array_reallocate(m_nPtr, nr, m_capacity);

        m_capacity = nr;
        SK_ASSERT(m_bPtr && m_iPtr && m_nPtr);

        SKhash i, h;
        _zeroIndices(0, m_capacity);

        for (i = 0; i < m_size; i++)
        {
            h         = m_bPtr[i].hash & m_capacity - 1;
            m_nPtr[i] = m_iPtr[h];
            m_iPtr[h] = i;
        }
    }

    SKsize         m_size, m_capacity;
    IndexArray     m_iPtr;
    IndexArray     m_nPtr;
    PointerType    m_bPtr;
    Allocator      m_alloc;
    IndexAllocator m_iAlloc;
};

template <typename T, typename Allocator = skAllocator<skEntry<T, bool> > >
class skHashSet
{
public:
    typedef skHashTable<T, bool, Allocator> TableType;
    SK_DECLARE_REF_TYPE(TableType)

    typedef skHashSet<T, Allocator>                            SelfType;
    typedef skPointerIncrementIterator<SelfType, SKsize>       Iterator;
    typedef const skPointerIncrementIterator<SelfType, SKsize> ConstIterator;
    typedef skPointerDecrementIterator<SelfType, SKsize>       ReverseIterator;
    typedef const skPointerDecrementIterator<SelfType, SKsize> ConstReverseIterator;

    const SKsize npos = SK_NPOS;

public:
    skHashSet()
    {
    }

    skHashSet(const skHashSet& oth) :
        m_table(oth)
    {
    }

    ~skHashSet()
    {
    }

    void clear(void)
    {
        m_table.clear();
    }

    bool insert(const T& v)
    {
        return m_table.insert(v, true);
    }

    void erase(const T& v)
    {
        m_table.remove(v);
    }

    SKsize find(const T& v)
    {
        return m_table.find(v);
    }

    SK_INLINE T& operator[](SKsize idx)
    {
        SK_ASSERT(idx < size());
        return m_table.at(idx);
    }

    SK_INLINE const T& operator[](SKsize idx) const
    {
        SK_ASSERT(idx < size());
        return m_table.at(idx);
    }

    SK_INLINE T& at(SKsize idx)
    {
        SK_ASSERT(idx < size());
        return m_table.at(idx);
    }

    SK_INLINE const T& at(SKsize idx) const
    {
        SK_ASSERT(idx < size());
        return m_table.at(idx);
    }

    SK_INLINE SKsize size(void) const
    {
        return m_table.size();
    }
    SK_INLINE bool empty(void) const
    {
        return m_table.empty();
    }
    SK_INLINE PointerType ptr(void)
    {
        return m_table.ptr();
    }
    SK_INLINE ConstPointerType ptr(void) const
    {
        return m_table.ptr();
    }

    SK_INLINE Iterator iterator(void)
    {
        return !empty() ? Iterator(ptr(), size()) : Iterator();
    }

    SK_INLINE ConstIterator iterator(void) const
    {
        return !empty() ? ConstIterator(const_cast<PointerType>(ptr()), size()) : ConstIterator();
    }

    SK_INLINE ReverseIterator reverseIterator(void)
    {
        return !empty() ? ReverseIterator(ptr(), size()) : ReverseIterator();
    }

    SK_INLINE ConstReverseIterator reverseIterator(void) const
    {
        return !empty() ? ConstReverseIterator(const_cast<PointerType>(ptr()), size()) : ConstReverseIterator();
    }

    skHashSet& operator=(const skHashSet& rhs)
    {
        if (this != &rhs)
            m_table = rhs.m_table;
        return *this;
    }

private:
    TableType m_table;
};

#endif  //_skMap_h_
