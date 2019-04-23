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
#ifndef _skLinkedList_h_
#define _skLinkedList_h_

#include "skSort.h"
#include "skAllocator.h"



template<typename T>
class skSinglyLinkedList
{
public:
    SK_DECLARE_TYPE(T);

    typedef skSinglyLinkedList<T> SelfType;


    class Link : public skAllocObject
    {
    public:
        SK_DECLARE_TYPE(Link);

    public:

        Link() : m_next(0) {}
        Link(SelfType::ConstReferenceType v) : m_next(0), m_data(v) {}
        ~Link() {}


        SK_INLINE PointerType               next(void) { return m_next; }
        SK_INLINE SelfType::ReferenceType   data(void) { return m_data; }


    private:
        friend class SelfType;

        PointerType         m_next;
        SelfType::ValueType m_data;
    };


private:
    Link*       m_last;
    Link*       m_first;

    SKsize      m_size;

public:

    skSinglyLinkedList() :
        m_size(0)
    {
        m_last  = 0;
        m_first = 0;
    }

    ~skSinglyLinkedList() { clear(); }


    void clear(void)
    {
        Link *l = m_first;
        while (l != 0)
        {
            Link *t = l->m_next;
            delete l;
            l = t;
        }
        m_last = 0;
        m_first = 0;

        m_size = 0;
    }

    void push_back(ConstReferenceType v)
    {
        Link *nd = new Link(v);

        if (!m_first)
            m_first = nd;
        else
        {
            if (m_last)
                m_last->m_next = nd;
        }
        m_last = nd;
        m_size++;
    }

    void push_front(ConstReferenceType v)
    {
        Link *nd = new Link(v);

        if (!m_first)
        {
            m_first = nd;
            m_last = nd;
        }
        else
        {
            Link *t = m_first;
            m_first = nd;
            m_first->m_next = t;
        }
        m_size++;
    }

    ValueType pop_back(void)
    {
        SK_ASSERT(m_last);
        m_size--;

        Link *l = m_first;
        while (l != 0 && l->m_next != m_last) l = l->m_next;

        SK_ASSERT(l && l->m_next == m_last);

        // This must create a copy, so watch it if it's 
        //
        // for example: 
        // skSinglyLinkedList<skArray<type> >
        //
        ValueType val = m_last->m_data;

        delete m_last;
        m_last = l;
        m_last->m_next = 0;
        return val;
    }


    void erase(ConstReferenceType v)
    {
        if (!m_first)
            return;

        Link* node = m_first, *prev = 0;
        while (node)
        {
            if (node->m_data == v)
            {
                if (node == m_first)
                    m_first = node->m_next;

                if (prev && node->m_next)
                    prev->m_next = node->m_next;

                if (node == m_last)
                {
                    if (prev) prev->m_next = 0;
                    m_last = prev;
                }

                delete node;
                --m_size;
                return;
            }
            prev = node;
            node = node->m_next;
        }
    }

    ValueType pop_front(void)
    {
        SK_ASSERT(m_first);
        m_size--;

        // This must create a copy, so watch it if it's 
        //
        // for example: 
        // skSinglyLinkedList<skArray<type> >
        //
        ValueType val = m_first->m_data;

        Link *t = m_first->m_next;
        delete m_first;
        m_first = t;
        return val;
    }



    SKsize find(ConstValueType v)
    {
        if (!m_first)
            return SK_NPOS;

        SKsize foundIndex = 0;
        Link* node = m_first;
        while (node)
        {
            if (node->m_data == v)
                break;
            foundIndex++;
            node = node->m_next;
        }
        return foundIndex;
    }

    ReferenceType at(SKsize idx)
    {
        SK_ASSERT(idx != SK_NPOS && idx<m_size);

        SKsize foundIndex = 0;
        Link* node = m_first;
        while (node)
        {
            if (foundIndex == idx) break;
            foundIndex++;
            node = node->m_next;
        }
        SK_ASSERT(node);
        return node->m_data;
    }

    SK_INLINE Link*     first(void) { return m_first; }
    SK_INLINE SKsize    size(void)  { return m_size; }
};


#endif//_skLinkedList_h_