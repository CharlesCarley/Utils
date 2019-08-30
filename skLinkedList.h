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

#include "skAllocator.h"
#include "skSort.h"

template <typename T>
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
        Link() :
            m_next(0)
        {
        }
        Link(typename SelfType::ConstReferenceType v) :
            m_next(0),
            m_data(v)
        {
        }
        ~Link()
        {
        }

        SK_INLINE PointerType next(void)
        {
            return m_next;
        }
        SK_INLINE typename SelfType::ReferenceType data(void)
        {
            return m_data;
        }

    private:
        friend class skSinglyLinkedList;
        PointerType                  m_next;
        typename SelfType::ValueType m_data;
    };

private:
    Link * m_head, *m_tail;
    SKsize m_size;

public:
    skSinglyLinkedList() :
        m_size(0)
    {
        m_head = m_tail = 0;
    }

    ~skSinglyLinkedList()
    {
        clear();
    }

    void clear(void)
    {
        Link* link = m_head;
        while (link != 0)
        {
            Link* tmp = link->m_next;
            delete link;
            link = tmp;
        }
        m_tail = 0;
        m_head = 0;
        m_size = 0;
    }

    void push_back(ConstReferenceType v)
    {
        Link* link = new Link(v);
        if (!m_head)
        {
            m_head = link;
            m_tail = link;
        }
        else
        {
            SK_ASSERT(m_tail && m_tail->m_next == 0);
            m_tail->m_next = link;
            m_tail         = link;
        }

        m_size++;
    }

    void push_front(ConstReferenceType v)
    {
        Link* nd = new Link(v);

        if (!m_head)
            m_head = nd;
        else
        {
            Link* t        = m_head;
            m_head         = nd;
            m_head->m_next = t;
        }
        m_size++;
    }

    ValueType pop_back(void)
    {
        SK_ASSERT(m_head);

        Link *link = m_head, *prev = 0;
        while (link->m_next != 0)
        {
            prev = link;
            link = link->m_next;
        }

        ValueType val = link->m_data;
        if (prev)
            prev->m_next = 0;

        m_tail = prev;
        if (link == m_head)
            m_head = 0;

        delete link;
        m_size--;
        return val;
    }

    ValueType pop_front(void)
    {
        SK_ASSERT(m_head);

        ValueType val  = m_head->m_data;
        Link*     link = m_head->m_next;

        m_size--;
        delete m_head;
        m_head = link;
        return val;
    }

    void erase(ConstReferenceType v)
    {
        if (!m_head)
            return;
        Link *prev = 0, *found = 0;

        find(v, &prev, &found);
        if (found)
        {
            Link* t = found->m_next;
            if (prev)
                prev->m_next = t;

            m_tail = prev;
            if (found == m_head)
                m_head = 0;

            delete found;
            m_size--;
        }
    }

    SKsize find(ConstValueType v)
    {
        if (!m_head)
            return SK_NPOS;

        SKsize foundIndex = 0;
        Link*  node       = m_head;
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
        Link*  node       = m_head;
        while (node)
        {
            if (foundIndex == idx)
                break;
            foundIndex++;
            node = node->m_next;
        }
        SK_ASSERT(node);
        return node->m_data;
    }

    SK_INLINE Link* first(void)
    {
        return m_head;
    }
    SK_INLINE SKsize size(void)
    {
        return m_size;
    }
    SK_INLINE bool empty(void)
    {
        return m_size == 0;
    }

private:
    void find(ConstValueType v, Link** prev, Link** pos)
    {
        if (!m_head)
            return;

        SK_ASSERT(prev && pos);
        Link *node = m_head, *last = 0;
        while (node)
        {
            if (node->m_data == v)
            {
                (*prev) = last;
                (*pos)  = node;
                return;
            }
            last = node;
            node = node->m_next;
        }
        (*prev) = 0;
        (*pos)  = 0;
    }
};

#endif  //_skLinkedList_h_