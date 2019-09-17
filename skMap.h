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

template <typename T>
class skHashTableIncrementIterator : public skPointerIncrementIterator<T>
{
public:
    typedef typename T::PairValueType PairValueType;
    typedef typename T::PairKeyType   PairKeyType;
    typedef typename T::PointerType   PointerType;

public:
    skHashTableIncrementIterator()
    {
    }

    skHashTableIncrementIterator(PointerType begin, SKuint32 size) :
        skPointerIncrementIterator<T>(begin, size)
    {
    }

    explicit skHashTableIncrementIterator(T& v) :
        skPointerIncrementIterator<T>(v)
    {
    }

    SK_INLINE PairKeyType& peekNextKey(void)
    {
        SK_ITER_DEBUG(hasMoreElements());
        return (*skPointerIncrementIterator<T>::m_beg).first;
    }

    SK_INLINE PairValueType& peekNextValue(void)
    {
        SK_ITER_DEBUG(hasMoreElements());
        return (*skPointerIncrementIterator<T>::m_beg).second;
    }

    SK_INLINE const PairKeyType& peekNextKey(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        return (*skPointerIncrementIterator<T>::m_beg).first;
    }

    SK_INLINE const PairValueType& peekNextValue(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        return (*skPointerIncrementIterator<T>::m_beg).second;
    }
};

template <typename T>
class skHashTableDecrementIterator : public skPointerDecrementIterator<T>
{
public:
    typedef typename T::PairValueType PairValueType;
    typedef typename T::PairKeyType   PairKeyType;
    typedef typename T::PointerType   PointerType;

public:
    skHashTableDecrementIterator()
    {
    }

    skHashTableDecrementIterator(PointerType begin, SKuint32 size) :
        skPointerDecrementIterator<T>(begin, size)
    {
    }

    explicit skHashTableDecrementIterator(T& v) :
        skPointerDecrementIterator<T>(v)
    {
    }

    SK_INLINE PairKeyType& peekNextKey(void)
    {
        SK_ITER_DEBUG(hasMoreElements());
        return (*skPointerDecrementIterator<T>::m_beg).first;
    }

    SK_INLINE PairValueType& peekNextValue(void)
    {
        SK_ITER_DEBUG(hasMoreElements());
        return (*skPointerDecrementIterator<T>::m_beg).second;
    }

    SK_INLINE const PairKeyType& peekNextKey(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        return (*skPointerDecrementIterator<T>::m_beg).first;
    }

    SK_INLINE const PairValueType& peekNextValue(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        return (*skPointerDecrementIterator<T>::m_beg).second;
    }
};

#define _SK_HASHTABLE_POW2(x) \
    --x;                      \
    x |= x >> 16;             \
    x |= x >> 8;              \
    x |= x >> 4;              \
    x |= x >> 2;              \
    x |= x >> 1;              \
    ++x;
#define _SK_HASHTABLE_IS_POW2(x) (x && !((x - 1) & x))

template <typename Key, typename Value>
class skEntry
{
public:
    Key      first;
    Value    second;
    SKuint32 hash;

    skEntry(): hash(0)
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
template <typename Key, typename Value, typename Allocator = skAllocator<skEntry<Key, Value> > >
class skHashTable
{
public:
    typedef skAllocator<SKuint32>                IndexAllocator;
    typedef skHashTable<Key, Value, Allocator> SelfType;

public:
    typedef skEntry<Key, Value> Pair;
    SK_DECLARE_TYPE(Pair);

    typedef SKuint32* IndexArray;
    typedef Key       PairKeyType;
    typedef Value     PairValueType;

    typedef skHashTableIncrementIterator<SelfType>       Iterator;
    typedef const skHashTableIncrementIterator<SelfType> ConstIterator;

    typedef skHashTableDecrementIterator<SelfType>       ReverseIterator;
    typedef const skHashTableDecrementIterator<SelfType> ConstReverseIterator;

	const SKuint32 npos = SK_NPOS32;

public:
    skHashTable() :
        m_size(0),
        m_capacity(0),
        m_iptr(0),
        m_nptr(0),
        m_bptr(0)
    {
    }

    skHashTable(SKuint32 capacity) :
        m_size(0),
        m_capacity(0),
        m_iptr(0),
        m_nptr(0),
        m_bptr(0)
    {
    }

    skHashTable(const skHashTable& rhs) :
        m_size(0),
        m_capacity(0),
        m_iptr(0),
        m_nptr(0),
        m_bptr(0)
    {
        doCopy(rhs);
    }

    ~skHashTable()
    {
        clear();
    }

    SelfType& operator=(const SelfType& rhs)
    {
        if (this != &rhs)
            doCopy(rhs);

        return *this;
    }

    void clear(void)
    {
        m_alloc.array_deallocate(m_bptr, m_capacity);
        m_ialloc.array_deallocate(m_iptr, m_capacity);
        m_ialloc.array_deallocate(m_nptr, m_capacity);

        m_bptr = 0;
        m_iptr = 0;
        m_nptr = 0;
        m_size = m_capacity = 0;
    }

    Value& at(SKuint32 i)
    {
        SK_ASSERT(m_bptr && i >= 0 && i < m_size);
        return m_bptr[i].second;
    }
    Value& operator[](SKuint32 i)
    {
        SK_ASSERT(m_bptr && i >= 0 && i < m_size);
        return m_bptr[i].second;
    }
    const Value& at(SKuint32 i) const
    {
        SK_ASSERT(m_bptr && i >= 0 && i < m_size);
        return m_bptr[i].second;
    }
    const Value& operator[](SKuint32 i) const
    {
        SK_ASSERT(m_bptr && i >= 0 && i < m_size);
        return m_bptr[i].second;
    }
    Key& keyAt(SKuint32 i)
    {
        SK_ASSERT(m_bptr && i >= 0 && i < m_size);
        return m_bptr[i].first;
    }
    const Key& keyAt(SKuint32 i) const
    {
        SK_ASSERT(m_bptr && i >= 0 && i < m_size);
        return m_bptr[i].first;
    }

    Value* get(const Key& key)
    {
        SKuint32 i = find(key);

        if (i == npos)
            return 0;

        return &m_bptr[i].second;
    }

    Value* operator[](const Key& key)
    {
        return get(key);
    }
    const Value* operator[](const Key& key) const
    {
        return get(key);
    }

    SKuint32 find(const Key& key) const
    {
        if (empty())
            return npos;

        SKuint32 hk = skHash(key);
        SKhash hr = (hk & (m_capacity - 1));
        SKuint32 fh = m_iptr[hr];

        while (fh != npos && hk != m_bptr[fh].hash)
            fh = m_nptr[fh];

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

        SKuint32 hk = skHash(key);
        SKhash hr = hk & (m_capacity - 1);

        m_bptr[m_size] = Pair(key, val, hk);
        m_nptr[m_size] = m_iptr[hr];
        m_iptr[hr]     = m_size++;
        return true;
    }

    void erase(const Key& key)
    {
        remove(key);
    }

    void remove(const Key& key)
    {
        SKhash hash, lhash;
        SKuint32 index, pindex, findex;

        if (empty())
            return;

        findex = find(key);

        hash = skHash(key) & (m_capacity - 1);

        index  = m_iptr[hash];
        pindex = npos;

        while (index != findex)
        {
            pindex = index;
            index  = m_nptr[index];
        }

        if (pindex != npos)
        {
            SK_ASSERT(m_nptr[pindex] == findex);
            m_nptr[pindex] = m_nptr[findex];
        }
        else
            m_iptr[hash] = m_nptr[findex];

        SKuint32 lindex = m_size - 1;

        if (lindex == findex)
        {
            --m_size;
            m_bptr[m_size].~Pair();
            return;
        }

        lhash  = m_bptr[lindex].hash & (m_capacity - 1);
        index  = m_iptr[lhash];
        pindex = npos;

        while (index != lindex)
        {
            pindex = index;
            index  = m_nptr[index];
        }

        if (pindex != npos)
        {
            SK_ASSERT(m_nptr[pindex] == lindex);
            m_nptr[pindex] = m_nptr[lindex];
        }
        else
            m_iptr[lhash] = m_nptr[lindex];

        m_bptr[findex] = m_bptr[lindex];
        m_nptr[findex] = m_iptr[lhash];
        m_iptr[lhash]  = findex;

        --m_size;
        m_bptr[m_size].~Pair();
        return;
    }

    SK_INLINE PointerType ptr(void)
    {
        return m_bptr;
    }
    SK_INLINE ConstPointerType ptr(void) const
    {
        return m_bptr;
    }
    SK_INLINE bool valid(void) const
    {
        return m_bptr != 0;
    }

    SK_INLINE SKuint32 size(void) const
    {
        return m_size;
    }
    SK_INLINE SKuint32 capacity(void) const
    {
        return m_capacity;
    }
    SK_INLINE bool empty(void) const
    {
        return m_size == 0;
    }

    Iterator iterator(void)
    {
        return m_bptr && m_size > 0 ? Iterator(m_bptr, m_size) : Iterator();
    }

    ConstIterator iterator(void) const
    {
        return m_bptr && m_size > 0 ? ConstIterator(m_bptr, m_size) : ConstIterator();
    }

    ReverseIterator reverseIterator(void)
    {
        return m_bptr && m_size > 0 ? ReverseIterator(m_bptr, m_size) : ReverseIterator();
    }

    ConstReverseIterator reverseIterator(void) const
    {
        return m_bptr && m_size > 0 ? ConstReverseIterator(m_bptr, m_size) : ConstReverseIterator();
    }

    void reserve(SKuint32 nr)
    {
        if (m_capacity < nr && nr != npos)
            rehash(nr);
    }

private:
    void _zeroIndices(SKuint32 from, SKuint32 to)
    {
        if (to <= 0 || from >= to)
            return;

        SKuint32 i = from;

        do
        {
            m_iptr[i] = m_nptr[i] = npos;
        } while (++i < to);
    }

    void doCopy(const SelfType& rhs)
    {
        if (rhs.valid() && !rhs.empty())
        {
            reserve(rhs.m_capacity);

            SKuint32 i, b;
            m_size     = rhs.m_size;
            m_capacity = rhs.m_capacity;

            b = m_size > 0 ? m_size - 1 : 0;
            _zeroIndices(b, m_capacity);

            for (i = 0; i < m_size; ++i)
            {
                m_bptr[i] = rhs.m_bptr[i];
                m_iptr[i] = rhs.m_iptr[i];
                m_nptr[i] = rhs.m_nptr[i];
            }
        }
    }

    void rehash(SKuint32 nr)
    {
        if (!_SK_HASHTABLE_IS_POW2(nr))
        {
            _SK_HASHTABLE_POW2(nr);
        }

        m_bptr = m_alloc.array_reallocate(m_bptr, nr, m_capacity);
        m_iptr = m_ialloc.array_reallocate(m_iptr, nr, m_capacity);
        m_nptr = m_ialloc.array_reallocate(m_nptr, nr, m_capacity);

        m_capacity = nr;
        SK_ASSERT(m_bptr && m_iptr && m_nptr);

        SKuint32 i, h;
        _zeroIndices(0, m_capacity);

        for (i = 0; i < m_size; i++)
        {
            h         = m_bptr[i].hash & (m_capacity - 1);
            m_nptr[i] = m_iptr[h];
            m_iptr[h] = i;
        }
    }

    SKuint32         m_size, m_capacity;
    IndexArray     m_iptr;
    IndexArray     m_nptr;
    PointerType    m_bptr;
    Allocator      m_alloc;
    IndexAllocator m_ialloc;
};




template <typename T, typename Allocator = skAllocator<skEntry<T, bool> > >
class skHashSet
{
public:
    typedef skHashTable<T, bool, Allocator> TableType;
    SK_DECLARE_REF_TYPE(TableType);

    typedef skHashSet<T, Allocator>                    SelfType;
    typedef skPointerIncrementIterator<SelfType>       Iterator;
    typedef const skPointerIncrementIterator<SelfType> ConstIterator;
    typedef skPointerDecrementIterator<SelfType>       ReverseIterator;
    typedef const skPointerDecrementIterator<SelfType> ConstReverseIterator;

    const SKuint32 npos = SK_NPOS32;

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

    SKuint32 find(const T& v)
    {
        return m_table.find(v);
    }

    SK_INLINE T& operator[](SKuint32 idx)
    {
        SK_ASSERT(idx >= 0 && idx < size());
        return m_table.at(idx);
    }

    SK_INLINE const T& operator[](SKuint32 idx) const
    {
        SK_ASSERT(idx >= 0 && idx < size());
        return m_table.at(idx);
    }

    SK_INLINE T& at(SKuint32 idx)
    {
        SK_ASSERT(idx >= 0 && idx < size());
        return m_table.at(idx);
    }

    SK_INLINE const T& at(SKuint32 idx) const
    {
        SK_ASSERT(idx >= 0 && idx < size());
        return m_table.at(idx);
    }

    SK_INLINE SKuint32 size(void) const
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
