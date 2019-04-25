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



template < typename Key, typename Value>
class skDictionary
{
public:

    class Pair : public skAllocObject
    {
    public:

        Key     first;
        Value   second;

        Pair()
            : hash(SK_NPOS), idx(SK_NPOS)
        {
        }

        Pair(const Key& k, const Value& v, SKhash hk, SKsize i)
            : first(k), second(v), hash(hk), idx(i)
        {
        }

        Pair(const Pair& oth)
            : first(oth.first), second(oth.second), hash(oth.hash), idx(oth.idx)
        {
        }

        SK_INLINE bool operator == (const Pair& rhs)
        {
            return first == rhs.first && second == rhs.second;
        }


        Pair& operator= (const Pair& rhs)
        {
            if (this != &rhs)
            {
                first = rhs.first;
                second = rhs.second;
                hash = rhs.hash;
                idx = rhs.idx;
            }
            return *this;
        }

        ~Pair() {}

    private:
        friend class skDictionary;
        SKsize  hash;
        SKsize  idx;
    };

    typedef Pair*                       Result;
    typedef Result                      PairList;
    typedef SKsize*                     Indices;
    typedef skDictionary<Key, Value> SelfType;

    SK_DECLARE_TYPE(Pair);

private:
    PointerType m_data;
    SKsize      m_size, m_capacity;
    Indices     m_index;


    // h(k) = k % (c - 1)
    SK_INLINE SKsize hash(const Key& key)
    {
        return skHash(key) & (m_capacity - 1);
    }


    // f(i) = i
    //   i = 0,1,2,3...
    // h1(k) = [ h(k) + f(i) ] % (c-1)
    SK_INLINE SKsize linearProbe(const SKhash& key, SKsize i)
    {
        return (key + i) & (m_capacity - 1);
    }

    // f(i) = i^2
    //   i = 0,1,2,3...
    // h1(k) = [ h(k) + f(i) ] % (c-1)
    SK_INLINE SKsize quadradicProbe(const SKhash& key, SKsize i)
    {
        return ((i * i) + key) & (m_capacity - 1);
    }

    SK_INLINE SKsize probe(const SKhash& key, SKsize i)
    {
        return quadradicProbe(key, i);
    }


public:
    skDictionary() :
        m_data(0),
        m_size(0),
        m_capacity(0)
    {
    }

    skDictionary(const skDictionary& o) :
        m_data(0),
        m_size(0),
        m_capacity(0)
    {
    }

    ~skDictionary() { clear(); }

    void clear(void)
    {
        if (m_data)
        {
            delete[]m_data;
            delete[]m_index;
        }
        m_data = 0;
        m_size = 0;
        m_capacity = 0;
    }


    void reserve(SKsize nr)
    {
        if (!m_data)
        {
            m_data  = new Pair[nr];
            m_index = new SKsize[nr];
            skMemset(m_index, SK_NPOS, nr * sizeof(SKsize));
        }
        else
        {
            if (nr > m_capacity)
                _rehash(nr);
        }

        m_capacity = nr;
    }

    void insert(const Key& key, const Value& val)
    {
        if (m_size + 1 > m_capacity)
            reserve(m_size == 0 ? 64 : m_size * 2);

        if (find(key) != SK_NPOS)
            return;


        SKhash mapping = hash(key);

        SKsize i = 0;
        while (m_index[mapping] != SK_NPOS)
            mapping = probe(mapping, i++);

        m_data[m_size] = Pair(key, val, mapping, m_size);
        m_index[mapping] = m_size;

        ++m_size;
    }

    bool has_key(const Key& key)
    {
        if (m_size == 0)
            return false;
        return !(m_data[hash()].empty());
    }

    SKsize find(const Key& k)
    {
        if (m_size == 0)
            return SK_NPOS;

        SKhash mapping = hash(k);
        if (m_index[mapping] == SK_NPOS)
            return SK_NPOS;

        SKsize i = 0, idx;
        while (i < m_capacity)
        {
            mapping = probe(mapping, i++);

            if (m_index[mapping] != SK_NPOS)
            {
                idx = m_index[mapping];
                if (idx < m_size)
                {
                    if (m_data[idx].hash == mapping &&
                            m_data[idx].first == k)
                        return m_index[mapping];
                }
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


        SKsize i = 0, idx, k3, k4;
        while (i < m_capacity)
        {
            mapping = probe(mapping, i++);

            if (m_index[mapping] != SK_NPOS)
            {
                idx = m_index[mapping];
                if (idx < m_size)
                {
                    if (m_data[idx].hash == mapping && m_data[idx].first == k)
                    {
                        idx = m_index[mapping];
                        k3 = idx;
                        k4 = m_size - 1;
                        m_index[m_data[k3].hash] = SK_NPOS;
                        m_index[m_data[k4].hash] = k3;
                        m_data[k3].hash = m_data[k4].hash;

                        skSwap(m_data[k3], m_data[k4]);
                        --m_size;

                        break;
                    }
                }
            }
        }
    }

    SK_INLINE SKsize size(void)     { return m_size; }
    SK_INLINE SKsize capacity(void) { return m_capacity; }

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




private:


    void _rehash(SKsize nr)
    {
        Pair* data     = new Pair[nr];
        SKsize* index = new SKsize[nr];
        skMemset(index, SK_NPOS, nr * sizeof(SKsize));

        SKsize i = 0;
        for (i = 0; i < m_size; ++i)
        {

            SKhash mapping = hash(m_data[i].first);

            SKsize j = 0;

            if (index[mapping] != SK_NPOS)
            {
                while (index[mapping] != SK_NPOS)
                    mapping = probe(mapping, j++);
            }

            data[i] = Pair(m_data[i].first, m_data[i].second, mapping, i);
            index[mapping] = i;
        }


        delete[]m_data;
        delete[]m_index;
        m_data = data;
        m_index = index;
    }
};





#endif//_skDictionary_h_
