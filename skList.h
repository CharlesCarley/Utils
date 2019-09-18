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
#ifndef _skList_h_
#define _skList_h_

#include "skAllocator.h"
#include "skSort.h"

template <typename T>
class skListIterator
{
public:
    typedef typename T::LinkPtr      LinkPtr;
    typedef typename T::DataRef      DataRef;
    typedef typename T::ConstDataRef ConstDataRef;

private:
    mutable LinkPtr m_cur;

public:
    skListIterator() :
        m_cur(0)
    {
    }

    skListIterator(LinkPtr first) :
        m_cur(first)
    {
    }

    explicit skListIterator(T& v) :
        m_cur(v.begin())
    {
    }

    skListIterator(const skListIterator& v) :
        m_cur(v.m_cur)
    {
    }

    skListIterator& operator=(const skListIterator& v)
    {
        if (this != &v)
            m_cur = v.m_cur;

        return *this;
    }

    SK_INLINE bool hasMoreElements(void) const
    {
        return m_cur != 0;
    }

    SK_INLINE void next(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        m_cur = m_cur->getNext();
    }

    DataRef getNext(void)
    {
        SK_ITER_DEBUG(hasMoreElements());
        DataRef ret = m_cur->getData();
        m_cur       = m_cur->getNext();
        return ret;
    }

    ConstDataRef getNext(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        DataRef ret = m_cur->getData();
        m_cur       = m_cur->getNext();
        return ret;
    }

    SK_INLINE DataRef peekNext(void)
    {
        SK_ITER_DEBUG(hasMoreElements());
        return m_cur->getData();
    }

    SK_INLINE ConstDataRef peekNext(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        return m_cur->getData();
    }
};

template <typename T>
class skListReverseIterator
{
public:
    typedef typename T::LinkPtr      LinkPtr;
    typedef typename T::DataRef      DataRef;
    typedef typename T::ConstDataRef ConstDataRef;

private:
    mutable LinkPtr m_cur;

public:
    skListReverseIterator() :
        m_cur(0)
    {
    }

    skListReverseIterator(LinkPtr first) :
        m_cur(first)
    {
    }

    explicit skListReverseIterator(T& v) :
        m_cur(v.end())
    {
    }

    skListReverseIterator(const skListReverseIterator& v) :
        m_cur(v.m_cur)
    {
    }

    skListReverseIterator& operator=(const skListReverseIterator& v)
    {
        if (this != &v)
            m_cur = v.m_cur;

        return *this;
    }

    SK_INLINE bool hasMoreElements(void) const
    {
        return m_cur != 0;
    }

    SK_INLINE void next(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        m_cur = m_cur->getPrev();
    }

    DataRef getNext(void)
    {
        SK_ITER_DEBUG(hasMoreElements());
        DataRef ret = m_cur->getData();
        m_cur       = m_cur->getPrev();
        return ret;
    }

    ConstDataRef getNext(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        DataRef ret = m_cur->getData();
        m_cur       = m_cur->getPrev();
        return ret;
    }

    SK_INLINE DataRef peekNext(void)
    {
        SK_ITER_DEBUG(hasMoreElements());
        return m_cur->getData();
    }

    SK_INLINE ConstDataRef peekNext(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        return m_cur->getData();
    }
};



template <typename T>
class skSinglyLinkedList
{
public:
    SK_DECLARE_TYPE(T);

    typedef skSinglyLinkedList<T>          SelfType;
    typedef skListIterator<SelfType>       Iterator;
    typedef const skListIterator<SelfType> ConstIterator;

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

        SK_INLINE PointerType getNext(void)
        {
            return m_next;
        }

        SK_INLINE typename SelfType::ReferenceType getData(void)
        {
            return m_data;
        }

    private:
        friend class skSinglyLinkedList;
        PointerType m_next;
        typename SelfType::ValueType m_data;
    };



    typedef Link*              LinkPtr;
    typedef ReferenceType      DataRef;
    typedef ConstReferenceType ConstDataRef;


private:
    Link * m_head, *m_tail;
    SKsize m_size;


public:
    skSinglyLinkedList() :
        m_head(0),
        m_tail(0),
        m_size(0)
    {
    }

    ~skSinglyLinkedList()
    {
        clear();
    }


    void clear(void)
    {
        Link* node = m_head;
        while (node != 0)
        {
            Link* tlink = node->m_next;
            delete node;
            node = tlink;
        }
        m_tail = 0;
        m_head = 0;
        m_size = 0;
    }

    void push_back(ConstReferenceType v)
    {
        Link* node = new Link(v);
        if (!m_head)
        {
            m_head = node;
            m_tail = node;
        }
        else
        {
            SK_ASSERT(m_tail && m_tail->m_next == 0);

            m_tail->m_next = node;
            m_tail         = node;
        }
        m_size++;
    }

    void push_front(ConstReferenceType v)
    {
        Link* node = new Link(v);
        if (!m_head)
        {
            m_head = node;
            m_tail = node;
        }
        else
        {
            node->m_next = m_head;
            m_head       = node;
        }
        m_size++;
    }

    void push_ordered(ConstReferenceType v)
    {
        Link* node = new Link(v);

        Link *prev, *next;
        if (!m_head)
        {
            m_head = node;
            m_tail = node;
        }
        else
        {
            if (v < m_head->m_data)
            {
                node->m_next = m_head;
                m_head       = node;
            }
            else
            {
                prev = m_head;
                next = m_head;

                while (next != 0 && next->m_data <= v)
                {
                    prev = next;
                    next = next->m_next;
                }
                prev->m_next = node;
                node->m_next = next;
                if (next == 0)
                     m_tail = prev;
            }
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

        delete link;
        m_size--;

        if (m_size == 0)
            m_head = m_tail = 0;

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
            return -1;

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
        SK_ASSERT(idx != -1 && idx < m_size);

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

    SK_INLINE Link* last(void)
    {
        return m_tail;
    }

    SK_INLINE SKsize size(void)
    {
        return m_size;
    }
    SK_INLINE bool empty(void)
    {
        return m_size == 0;
    }


    Iterator iterator(void)
    {
        return Iterator(m_head);
    }

    ConstIterator const_iterator(void) const
    {
        return ConstIterator(m_head);
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


template <typename T, typename LinkPtr>
class skListBase
{
public:
    SK_DECLARE_TYPE(T);

public:
    skListBase()
    {
        clear();
    }

    ~skListBase()
    {
        clear();
    }

    void clear(void)
    {
        m_first = 0;
        m_last  = 0;
        m_size  = 0;
    }

    void push_back(LinkPtr link)
    {
        SK_ASSERT(link);
        link->m_prev = m_last;

        if (m_last)
            m_last->m_next = link;

        if (!m_first)
            m_first = link;

        m_last = link;
        ++m_size;
    }

    void push_front(LinkPtr link)
    {
        SK_ASSERT(link);

        link->m_next = m_first;

        if (m_first)
            m_first->m_prev = link;

        if (!m_last)
            m_last = link;

        m_first = link;
        ++m_size;
    }

    LinkPtr find_link(ConstReferenceType v)
    {
        LinkPtr node = m_first;

        while (node)
        {
            if (node->m_data == v)
                return node;

            node = node->m_next;
        }

        return 0;
    }

    LinkPtr link_at(SKsize pos) const
    {
        SKsize  i    = 0;
        LinkPtr node = m_first;

        while (node)
        {
            if (i == pos)
                return node;

            node = node->m_next;
            ++i;
        }

        return 0;
    }

    bool erase_link(LinkPtr link)
    {
        if (!link || m_size == 0)
            return false;

        if (link->m_next)
            link->m_next->m_prev = link->m_prev;

        if (link->m_prev)
            link->m_prev->m_next = link->m_next;

        if (m_last == link)
            m_last = link->m_prev;

        if (m_first == link)
            m_first = link->m_next;

        --m_size;
        return true;
    }

    SK_INLINE LinkPtr first(void)
    {
        return m_first;
    }
    SK_INLINE LinkPtr last(void)
    {
        return m_last;
    }
    SK_INLINE SKsize size(void)
    {
        return m_size;
    }

private:
    LinkPtr m_first;
    LinkPtr m_last;
    SKsize  m_size;
};

template <typename T>
class skList
{
public:
    SK_DECLARE_TYPE(T);

public:
    typedef skListIterator<skList<T> >        Iterator;
    typedef skListReverseIterator<skList<T> > ReverseIterator;
    typedef const skListIterator<skList<T> >  ConstIterator;
    typedef const skListIterator<skList<T> >  ConstReverseIterator;

    SK_IMPLEMENT_SORT(T, skList, SKsize);

    class Link : public skAllocObject
    {
    public:
        Link() :
            m_next(0),
            m_prev(0),
            m_data(ValueType())
        {
        }

        explicit Link(ConstReferenceType v) :
            m_next(0),
            m_prev(0),
            m_data(v)
        {
        }

        ~Link()
        {
        }

        SK_INLINE ReferenceType getData(void)
        {
            return m_data;
        }
        SK_INLINE Link* getNext(void)
        {
            return m_next;
        }
        SK_INLINE Link* getPrev(void)
        {
            return m_prev;
        }

    protected:
        Link *    m_next, *m_prev;
        ValueType m_data;
        friend class skList;
        friend class skListBase<T, Link*>;
        friend class skListIterator<skList<T> >;
        friend class skListReverseIterator<skList<T> >;
    };
    typedef Link*                LinkPtr;
    typedef ReferenceType        DataRef;
    typedef ConstReferenceType   ConstDataRef;
    typedef skListBase<T, Link*> BaseType;

public:
    skList()
    {
    }

    skList(const skList& rhs)
    {
        ConstIterator iter = rhs.iterator();
        while (iter.hasMoreElements())
            push_back(iter.getNext());
    }

    ~skList()
    {
        clear();
    }

    void clear(void)
    {
        LinkPtr node = m_list.first();
        while (node)
        {
            LinkPtr temp = node;
            node         = node->m_next;
            delete temp;
        }
        m_list.clear();
    }

    SK_INLINE void push_back(ConstReferenceType v)
    {
        m_list.push_back(new Link(v));
    }

    SK_INLINE void push_front(ConstReferenceType v)
    {
        m_list.push_front(new Link(v));
    }

    LinkPtr find(ConstReferenceType v)
    {
        return m_list.find_link(v);
    }

    ReferenceType at(SKsize index)
    {
        SK_ASSERT(index < m_list.size());
        LinkPtr node = m_list.link_at(index);

        SK_ASSERT(node);
        return node->m_data;
    }

    ConstReferenceType at(SKsize index) const
    {
        SK_ASSERT(index < m_list.size());
        LinkPtr node = m_list.link_at(index);

        SK_ASSERT(node);
        return node->m_data;
    }

    LinkPtr link_at(SKsize index)
    {
        return m_list.link_at(index);
    }

    void erase(LinkPtr link)
    {
        if (m_list.erase_link(link))
            delete link;
    }

    void erase(ConstReferenceType v)
    {
        LinkPtr fnd = m_list.find_link(v);

        if (fnd)
        {
            if (m_list.erase_link(fnd))
                delete fnd;
        }
    }

    SK_INLINE void pop_back(void)
    {
        erase(m_list.last());
    }

    SK_INLINE void pop_front(void)
    {
        erase(m_list.first());
    }

    SK_INLINE bool empty(void) const
    {
        return m_list.size() == 0;
    }

    SK_INLINE SKsize size(void) const
    {
        return m_list.size();
    }

    SK_INLINE LinkPtr begin(void)
    {
        return m_list.first();
    }

    SK_INLINE LinkPtr end(void)
    {
        return m_list.last();
    }

    SK_INLINE const LinkPtr begin(void) const
    {
        return m_list.first();
    }

    SK_INLINE const LinkPtr end(void) const
    {
        return m_list.last();
    }

    SK_INLINE ReferenceType front(void)
    {
        SK_ASSERT(m_list.first());
        return m_list.first()->m_data;
    }

    SK_INLINE ReferenceType back(void)
    {
        SK_ASSERT(m_list.last());
        return m_list.last()->m_data;
    }

    SK_INLINE Iterator iterator(void)
    {
        return m_list.first() ? Iterator(m_list.first()) : Iterator();
    }

    SK_INLINE ReverseIterator reverseIterator(void)
    {
        return m_list.last() ? ReverseIterator(m_list.last()) : ReverseIterator();
    }

    SK_INLINE ConstIterator iterator(void) const
    {
        return m_list.first() ? ConstIterator(m_list.first()) : ConstIterator();
    }

    SK_INLINE ConstReverseIterator reverseIterator(void) const
    {
        return m_list.last() ? ConstReverseIterator(m_list.last()) : ConstReverseIterator();
    }

    skList& operator=(const skList& rhs)
    {
        if (this != &rhs)
        {
            clear();
            ConstIterator iter = rhs.iterator();

            while (iter.hasMoreElements())
                push_back(iter.getNext());
        }

        return *this;
    }

private:
    mutable BaseType m_list;
};

template <typename T>
class skListClass
{
public:
    SK_DECLARE_TYPE(T);

public:
    typedef T*       LinkPtr;
    typedef T*       DataRef;
    typedef const T* ConstDataRef;

    typedef skListBase<LinkPtr, LinkPtr> BaseType;

    typedef skListIterator<skListClass<T> >              Iterator;
    typedef skListReverseIterator<skListClass<T> >       ReverseIterator;
    typedef const skListIterator<skListClass<T> >        ConstIterator;
    typedef const skListReverseIterator<skListClass<T> > ConstReverseIterator;

    SK_IMPLEMENT_SORT(T, skListClass, SKsize);

    class Link
    {
    public:
        Link() :
            m_next(0),
            m_prev(0),
            m_data(0)
        {
        }

        ~Link()
        {
        }

        SK_INLINE DataRef getData(void)
        {
            return m_data;
        }

        SK_INLINE LinkPtr getNext(void)
        {
            return m_next;
        }

        SK_INLINE LinkPtr getPrev(void)
        {
            return m_prev;
        }

    protected:
        LinkPtr m_next, m_prev;
        LinkPtr m_data;

        friend class skListClass;
        friend class skListBase<LinkPtr, LinkPtr>;
        friend class skListIterator<skListClass<T> >;
        friend class skListReverseIterator<skListClass<T> >;
    };

public:
    skListClass()
    {
    }
    ~skListClass()
    {
        clear();
    }

    void clear(void)
    {
        m_list.clear();
    }

    SK_INLINE void push_back(LinkPtr v)
    {
        if (v)
        {
            static_cast<Link*>(v)->m_data = v;
            m_list.push_back(v);
        }
    }

    SK_INLINE void push_front(LinkPtr v)
    {
        if (v)
        {
            static_cast<Link*>(v)->m_data = v;
            m_list.push_front(v);
        }
    }

    LinkPtr at(SKsize index)
    {
        SK_ASSERT(index < m_list.size());
        Link* node = m_list.link_at(index);

        SK_ASSERT(node);
        return node->m_data;
    }

    LinkPtr at(SKsize index) const
    {
        SK_ASSERT(index < m_list.size());
        Link* node = m_list.link_at(index);
        SK_ASSERT(node);
        return node->m_data;
    }

    LinkPtr link_at(SKsize index)
    {
        return m_list.link_at(index);
    }

    void erase(LinkPtr v)
    {
        return m_list.erase_link(v);
    }

    SK_INLINE void pop_back(void)
    {
        m_list.erase_link(m_list.last());
    }

    SK_INLINE void pop_front(void)
    {
        m_list.erase_link(m_list.first());
    }

    SK_INLINE bool empty(void) const
    {
        return m_list.size() == 0;
    }

    SK_INLINE SKsize size(void) const
    {
        return m_list.size();
    }

    SK_INLINE LinkPtr begin(void)
    {
        return static_cast<LinkPtr>(m_list.first());
    }

    SK_INLINE LinkPtr end(void)
    {
        return static_cast<LinkPtr>(m_list.last());
    }

    SK_INLINE const LinkPtr begin(void) const
    {
        return static_cast<const LinkPtr>(m_list.first());
    }

    SK_INLINE const LinkPtr end(void) const
    {
        return static_cast<const LinkPtr>(m_list.last());
    }

    SK_INLINE LinkPtr front(void)
    {
        SK_ASSERT(m_list.first());
        return static_cast<LinkPtr>(m_list.first()->m_data);
    }

    SK_INLINE LinkPtr back(void)
    {
        SK_ASSERT(m_list.last());
        return static_cast<LinkPtr>(m_list.last()->m_data);
    }

    SK_INLINE Iterator iterator(void)
    {
        return m_list.first() ? Iterator(m_list.first()) : Iterator();
    }

    SK_INLINE ReverseIterator reverseIterator(void)
    {
        return m_list.last() ? ReverseIterator(m_list.last()) : ReverseIterator();
    }

    SK_INLINE ConstIterator iterator(void) const
    {
        return m_list.first() ? ConstIterator(m_list.first()) : ConstIterator();
    }

    SK_INLINE ConstReverseIterator reverseIterator(void) const
    {
        return m_list.last() ? ConstReverseIterator(m_list.last()) : ConstReverseIterator();
    }

private:
    skListClass& operator=(const skListClass& rhs)
    {
        // no assignment
        return *this;
    }

    skListClass(const skListClass& rhs)
    {
        // no copy
    }


    mutable BaseType m_list;
};

#endif  //_skList_h_
