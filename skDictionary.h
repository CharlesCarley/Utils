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
#ifndef _skDictionary_h_
#define _skDictionary_h_

#include <memory.h>
#include "Config/skConfig.h"
#include "Utils/skArray.h"
#include "Utils/skMap.h"
#include "Utils/skTraits.h"

template <typename Key, typename Value>
class skDictionary
{
public:
    class Pair
    {
    public:
        Key   first;
        Value second;

        Pair() :
            first(),
            second(),
            hash(-1)
        {
        }

        Pair(const Key& k, const Value& v, SKhash hk) :
            first(k),
            second(v),
            hash((SKuint32)hk)
        {
        }

        Pair(const Pair& oth) :
            first(oth.first),
            second(oth.second),
            hash(oth.hash)
        {
        }

        SK_INLINE bool operator==(const Pair& rhs)
        {
            return hash == rhs.hash && first == rhs.first && second == rhs.second;
        }

        Pair& operator=(const Pair& rhs)
        {
            if (this != &rhs)
            {
                first  = rhs.first;
                second = rhs.second;
                hash   = rhs.hash;
            }
            return *this;
        }

        ~Pair()
        {
        }

    private:
        friend class skDictionary;
        SKuint32 hash;
    };

    SK_DECLARE_TYPE(Pair)

    typedef skDictionary<Key, Value>                   SelfType;
    typedef skPointerIncrementIterator<SelfType>       Iterator;
    typedef const skPointerIncrementIterator<SelfType> ConstIterator;
    typedef skPointerDecrementIterator<SelfType>       ReverseIterator;
    typedef const skPointerDecrementIterator<SelfType> ConstReverseIterator;

    const SKuint32 npos = -1;

private:
    PointerType m_data;
    SKuint32    m_size, m_capacity;
    SKuint32*   m_index;

    SK_INLINE SKuint32 hash(const Key& key) const
    {
        return skHash(key) % m_capacity;
    }

    SK_INLINE SKuint32 linearProbe(const SKhash& key, SKuint32 i) const
    {
        return (key + i) % m_capacity;
    }

    SK_INLINE SKuint32 probe(const SKhash& key, SKuint32 i) const
    {
        return linearProbe(key, i);
    }

public:
    skDictionary() :
        m_data(nullptr),
        m_size(0),
        m_capacity(0),
        m_index(nullptr)
    {
    }

    skDictionary(const skDictionary& o) :
        m_data(nullptr),
        m_size(0),
        m_capacity(0),
        m_index(nullptr)
    {
        SK_ASSERT(0 && "TODO");
    }

    ~skDictionary()
    {
        clear();
    }

    void clear(void)
    {
        if (m_data)
        {
            delete[] m_data;
            delete[] m_index;
        }
        m_index    = nullptr;
        m_data     = nullptr;
        m_size     = 0;
        m_capacity = 0;
    }

    void reserve(SKuint32 nr)
    {
        if (nr > m_capacity)
        {
            m_capacity = nr;
            rehash(m_capacity);
        }
    }

    void insert(const Key& key, const Value& val)
    {
        if ((m_size + 1) * 2 >= m_capacity)  // assure that the load factor is balanced
            reserve(m_size == 0 ? 16 : m_capacity * 2);

        if (find(key) != npos)
            return;

        SKhash mapping   = probeKey(key);
        m_data[m_size]   = Pair(key, val, mapping);
        m_index[mapping] = m_size;
        ++m_size;
    }

    bool has_key(const Key& key)
    {
        if (m_size == 0)
            return false;
        return find(key) != npos;
    }

    SKuint32 find(const Key& k) const
    {
        if (m_size == 0)
            return npos;

        SKhash mapping = hash(k);
        if (m_index[mapping] == npos)
            return npos;

        SKuint32 idx = m_index[mapping];
        if (m_data[idx].hash != mapping || m_data[idx].first != k)
        {
            SKuint32 i = 0;
            while (i < m_capacity)
            {
                mapping = probe(mapping, i++);
                idx     = m_index[mapping];
                if (idx != npos && m_data[idx].hash == mapping && m_data[idx].first == k)
                    return idx;
            }
            idx = npos;
        }
        return idx;
    }

    void erase(const Key& k)
    {
        if (m_size == 0)
            return;

        SKuint32 A = find(k), B = m_size - 1;
        if (A != npos)
        {
            const SKuint32 mapB = m_data[B].hash;
            const SKuint32 mapA = m_data[A].hash;

            m_size--;
            if (m_size == 0)
            {
                m_index[mapA] = npos;
                m_index[mapB] = npos;
            }
            else
            {
                const SKuint32 idx = m_index[mapA];
                m_index[mapB]      = idx;
                m_index[mapA]      = npos;

                skSwap(m_data[A], m_data[B]);
            }
        }
    }

    SK_INLINE ReferenceType operator[](SKuint32 idx)
    {
        SK_ASSERT(idx >= 0 && idx < m_size);
        return m_data[idx];
    }

    SK_INLINE ConstReferenceType operator[](SKuint32 idx) const
    {
        SK_ASSERT(idx >= 0 && idx < m_size);
        return m_data[idx];
    }

    SK_INLINE ReferenceType at(SKuint32 idx)
    {
        SK_ASSERT(idx >= 0 && idx < m_size);
        return m_data[idx];
    }

    SK_INLINE ConstReferenceType at(SKuint32 idx) const
    {
        SK_ASSERT(idx >= 0 && idx < m_size);
        return m_data[idx];
    }

    SK_INLINE ConstPointerType ptr(void) const
    {
        return m_data;
    }
    SK_INLINE PointerType ptr(void)
    {
        return m_data;
    }

    SK_INLINE const SKuint32* iptr(void) const
    {
        return m_index;
    }

    SK_INLINE bool valid(void) const
    {
        return m_data != nullptr;
    }

    SK_INLINE SKuint32 capacity(void) const
    {
        return m_capacity;
    }

    SK_INLINE SKuint32 size(void) const
    {
        return m_size;
    }

    SK_INLINE bool empty(void) const
    {
        return m_size == 0;
    }

    SK_INLINE Iterator iterator(void)
    {
        return m_data && m_size > 0 ? Iterator(m_data, m_size) : Iterator();
    }

    SK_INLINE ConstIterator iterator(void) const
    {
        return m_data && m_size > 0 ? ConstIterator(m_data, m_size) : ConstIterator();
    }

    SK_INLINE ReverseIterator reverseIterator(void)
    {
        return m_data && m_size > 0 ? ReverseIterator(m_data, m_size) : ReverseIterator();
    }

    SK_INLINE ConstReverseIterator reverseIterator(void) const
    {
        return m_data && m_size > 0 ? ConstReverseIterator(m_data, m_size) : ConstReverseIterator();
    }

private:
    SKuint32 probeKey(const Key& k)
    {
        SKhash   mapping = hash(k);
        SKuint32 i       = 0;
        while (m_index[mapping] != npos && i < m_capacity)
            mapping = probe(mapping, i++);

        SK_ASSERT(i != m_capacity);
        SK_ASSERT(m_index[mapping] == npos);
        return (SKuint32)mapping;
    }

    void rehash(SKuint32 nr)
    {
        Pair* data  = new Pair[nr];
        auto* index = new SKuint32[nr];
        skMemset(index, npos, (nr) * sizeof(SKuint32));

        SKuint32 i, mapping, j;
        for (i = 0; i < m_size; ++i)
        {
            mapping = hash(m_data[i].first);
            j       = 0;
            while (index[mapping] != npos && j < m_capacity)
                mapping = probe(mapping, j++);

            SK_ASSERT(j != m_capacity);
            SK_ASSERT(index[mapping] == npos);

            data[i]        = Pair(m_data[i].first, m_data[i].second, mapping);
            index[mapping] = i;
        }

        delete[] m_data;
        delete[] m_index;

        m_data  = data;
        m_index = index;
    }
};

#endif  //_skDictionary_h_
