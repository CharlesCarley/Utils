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
    Link*       m_first;
    SKsize      m_size;

public:

    skSinglyLinkedList() :
        m_size(0)
    {
        m_first = 0;
    }

    ~skSinglyLinkedList() { clear(); }


    void clear(void)
    {
        Link* link = m_first;
        while (link != 0)
        {
            Link* tmp = link->m_next;
            delete link;
            link = tmp;
        }
        m_first = 0;
        m_size = 0;
    }

    void push_back(ConstReferenceType v)
    {
        Link* nd = new Link(v);
        if (!m_first)
            m_first = nd;
        else
            push_last(m_first, nd);

        m_size++;
    }

    void push_front(ConstReferenceType v)
    {
        Link* nd = new Link(v);

        if (!m_first)
            m_first = nd;
        else
        {
            Link* t = m_first;
            m_first = nd;
            m_first->m_next = t;
        }
        m_size++;
    }

    ValueType pop_back(void)
    {
        SK_ASSERT(m_first);

        Link* link = m_first, *prev = 0;
        while (link->m_next != 0)
        {
            prev = link;
            link = link->m_next;
        }

        ValueType val = link->m_data;
        if (prev)
            prev->m_next = 0;

        if (link == m_first)
            m_first = 0;

        delete link;
        m_size--;
        return val;
    }

    ValueType pop_front(void)
    {
        SK_ASSERT(m_first);

        ValueType val = m_first->m_data;
        Link* link = m_first->m_next;

        m_size--;
        delete m_first;
        m_first = link;
        return val;
    }

    void erase(ConstReferenceType v)
    {
        if (!m_first)
            return;

        m_first = erase_recursive(0, m_first, v);
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
        SK_ASSERT(idx != SK_NPOS && idx < m_size);

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
    SK_INLINE bool      empty(void) { return m_size == 0; }


private:

    void push_last(Link* node, Link* val)
    {
        if (node->m_next)
            push_last(node->m_next, val);
        else
            node->m_next = val;

    }

    Link* erase_recursive(Link* prev, Link* node, ConstReferenceType v)
    {
        if (!node)
            return 0;

        if (node->m_data == v)
        {
            Link* t = node->m_next;
            if (prev)
                prev->m_next = node->m_next;

            delete node;
            m_size--;
            node = t;

        }
        else if (node->m_next)
            erase_recursive(node, node->m_next, v);

        return node;
    }

};


#endif//_skLinkedList_h_