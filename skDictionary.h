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


#include "Config/skConfig.h"
#include "Utils/skTraits.h"
#include "Utils/skArray.h"
#include "Utils/skLinkedList.h"



template <typename Key, typename Value>
class skDictionary
{
public:

    class Pair : public skAllocObject
    {
    public:
        Key     first;
        Value   second;
 
        Pair() :
            hash(SK_NPOS)
        {
        }

        Pair(const Key& k, const Value& v, SKhash hk)
            : first(k), second(v), hash(hk)
        {
        }

        Pair(const Pair& oth)
            : first(oth.first), second(oth.second), hash(oth.hash)
        {
        }

        SK_INLINE bool operator == (const Pair& rhs)
        {
            return hash == rhs.hash && first == rhs.first && second == rhs.second;
        }

        Pair& operator= (const Pair& rhs)
        {
            if (this != &rhs)
            {
                first = rhs.first;
                second = rhs.second;
                hash = rhs.hash;
            }
            return *this;
        }

        ~Pair() {}

    private:
        friend class skDictionary;
        SKsize  hash;
    };

    SK_DECLARE_TYPE(Pair);

    typedef skDictionary<Key, Value>                      SelfType;
    typedef skPointerIncrementIterator<SelfType>          Iterator;
    typedef const skPointerIncrementIterator<SelfType>    ConstIterator;
    typedef skPointerDecrementIterator<SelfType>          ReverseIterator;
    typedef const skPointerDecrementIterator<SelfType>    ConstReverseIterator;


private:
    PointerType m_data;
    SKsize      m_size, m_capacity;
    SKsize*     m_index;

    SK_INLINE SKsize hash(const Key& key) const
    {
        return skHash(key) & (m_capacity - 1);
    }

    SK_INLINE SKsize linearProbe(const SKhash& key, SKsize i) const
    {
        return (key + i) & (m_capacity - 1);
    }

    SK_INLINE SKsize quadradicProbe(const SKhash& key, SKsize i) const
    {
        return ((i * i) + key) & (m_capacity - 1);
    }

    SK_INLINE SKsize probe(const SKhash& key, SKsize i) const
    {
        return linearProbe(key, i);
    }


public:
    skDictionary() :
        m_data(0),
        m_index(0),
        m_size(0),
        m_capacity(0)
    {
    }

    skDictionary(const skDictionary& o) :
        m_data(0), 
        m_index(0),
        m_size(0),
        m_capacity(0)
    {
        SK_ASSERT(0 && "TODO");
    }

    ~skDictionary() { clear(); }

    void clear(void)
    {
        if (m_data)
        {
            delete[]m_data;
            delete[]m_index;
        }
        m_index = 0;
        m_data = 0;
        m_size = 0;
        m_capacity = 0;
    }


    void reserve(SKsize nr)
    {
        if (nr > m_capacity)
        {
            m_capacity = nr;
            rehash(m_capacity);
        }
    }

    void insert(const Key& key, const Value& val)
    {
        if (m_size*2 >= m_capacity) // assure that the load factor is balanced  
            reserve(m_size == 0 ? 16 : m_capacity * 2);

        if (find(key) != SK_NPOS)
            return;

        SKhash mapping = hash(key);
        SKsize i = 0;

        while (m_index[mapping] != SK_NPOS && i < m_capacity)
            mapping = probe(mapping, i++);

        SK_ASSERT(i != m_capacity);

        m_data[m_size] = Pair(key, val, mapping);
        m_index[mapping] = m_size;
        ++m_size;
    }

    bool has_key(const Key& key)
    {
        if (m_size == 0)
            return false;
        return find(key) != SK_NPOS;
    }

    SKsize find(const Key& k) const
    {
        if (m_size == 0)
            return SK_NPOS;

        SKhash mapping = hash(k);
        if (m_index[mapping] == SK_NPOS)
            return SK_NPOS;

        SKsize i = 0, idx;
        while (i < m_capacity) // worst case
        {
            mapping = probe(mapping, i++);
            idx = m_index[mapping];
            if (idx < m_size)
            {
                if (m_data[idx].hash == mapping && m_data[idx].first == k)
                    return m_index[mapping];
            }
        }
        return SK_NPOS;
    }

    void erase(const Key& k)
    {
        if (m_size == 0)
            return;

        SKsize mapping = hash(k);
        if (m_index[mapping] == SK_NPOS)
            return;

        SKsize i = 0, idx;
        while (i < m_capacity)
        {
            mapping = probe(mapping, i++);

            idx = m_index[mapping];
            if (m_index[mapping] == SK_NPOS || idx > m_size)
                continue;

            if (m_data[idx].hash == mapping && m_data[idx].first == k)
            {
                swap_keys(idx, m_size - 1);
                --m_size;
                break;
            }
        }
    }

    SK_INLINE ReferenceType operator[](SKsize idx)
    {
        SK_ASSERT(idx >= 0 && idx < m_capacity);
        return m_data[idx];
    }

    SK_INLINE ConstReferenceType operator[](SKsize idx) const
    {
        SK_ASSERT(idx >= 0 && idx < m_capacity);
        return m_data[idx];
    }

    SK_INLINE ReferenceType at(SKsize idx)
    {
        SK_ASSERT(idx >= 0 && idx < m_capacity);
        return m_data[idx];
    }

    SK_INLINE ConstReferenceType at(SKsize idx) const
    {
        SK_ASSERT(idx >= 0 && idx < m_capacity);
        return m_data[idx];
    }

    SK_INLINE ConstPointerType  ptr(void)       const { return m_data; }
    SK_INLINE PointerType       ptr(void)             { return m_data; }
    SK_INLINE bool              valid(void)     const { return m_data != 0; }
    SK_INLINE SKsize            capacity(void)  const { return m_capacity; }
    SK_INLINE SKsize            size(void)      const { return m_size; }
    SK_INLINE bool              empty(void)     const { return m_size == 0; }

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

    void swap_keys(SKsize k3, SKsize k4)
    {
        m_index[m_data[k3].hash] = SK_NPOS;
        m_index[m_data[k4].hash] = k3;
        m_data[k3].hash = m_data[k4].hash;

        skSwap(m_data[k3], m_data[k4]);
    }

    void rehash(SKsize nr)
    {
        Pair* data = new Pair[nr];
        SKsize* index = new SKsize[nr];
        skMemset(index, SK_NPOS, nr * sizeof(SKsize));

        SKsize i = 0, mapping, j;
        for (i = 0; i < m_size; ++i)
        {
            mapping = hash(m_data[i].first);

            j = 0;
            while (index[mapping] != SK_NPOS && j < m_capacity)
                mapping = probe(mapping, j++);

            SK_ASSERT(j != m_capacity);

            data[i] = Pair(m_data[i].first, m_data[i].second, mapping);
            index[mapping] = i;
        }

        delete[]m_data;
        delete[]m_index;
        m_data  = data;
        m_index = index;
    }
};

#endif//_skDictionary_h_
