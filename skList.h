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
    typedef typename T::LinkPtr         LinkPtr;
    typedef typename T::LinkPtrRef      LinkPtrRef;
    typedef typename T::ConstLinkPtrRef ConstLinkPtrRef;

private:
    mutable LinkPtr m_cur;

public:
    skListIterator() : m_cur(0)
    {
    }

    skListIterator(LinkPtr first) : m_cur(first)
    {
    }

    explicit skListIterator(T& v) : m_cur(v.begin())
    {
    }

    skListIterator(const skListIterator& v) : m_cur(v.m_cur)
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

    LinkPtrRef getNext(void)
    {
        SK_ITER_DEBUG(hasMoreElements());
        LinkPtrRef ret = m_cur->getLink();
        m_cur = m_cur->getNext();
        return ret;
    }

    ConstLinkPtrRef getNext(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        LinkPtrRef ret = m_cur->getLink();
        m_cur = m_cur->getNext();
        return ret;
    }

    SK_INLINE LinkPtrRef peekNext(void)
    {
        SK_ITER_DEBUG(hasMoreElements());
        return m_cur->getLink();
    }

    SK_INLINE ConstLinkPtrRef peekNext(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        return m_cur->getLink();
    }
};

template <typename T>
class skListReverseIterator
{
public:
    typedef typename T::LinkPtr         LinkPtr;
    typedef typename T::LinkPtrRef      LinkPtrRef;
    typedef typename T::ConstLinkPtrRef ConstLinkPtrRef;

private:
    mutable LinkPtr m_cur;

public:
    skListReverseIterator() : m_cur(0)
    {
    }

    skListReverseIterator(LinkPtr first) : m_cur(first)
    {
    }

    explicit skListReverseIterator(T& v) : m_cur(v.end())
    {
    }

    skListReverseIterator(const skListReverseIterator& v) : m_cur(v.m_cur)
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

    LinkPtrRef getNext(void)
    {
        SK_ITER_DEBUG(hasMoreElements());
        LinkPtrRef ret = m_cur->getLink();
        m_cur = m_cur->getPrev();
        return ret;
    }

    ConstLinkPtrRef getNext(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        LinkPtrRef ret = m_cur->getLink();
        m_cur = m_cur->getPrev();
        return ret;
    }

    SK_INLINE LinkPtrRef peekNext(void)
    {
        SK_ITER_DEBUG(hasMoreElements());
        return m_cur->getLink();
    }

    SK_INLINE ConstLinkPtrRef peekNext(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        return m_cur->getLink();
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
        m_last = 0;
        m_size = 0;
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
        SKsize  i = 0;
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

    SK_IMPLEMENT_SORT(T, skList);

    class Link : public skAllocObject
    {
    public:
        Link() : m_next(0), m_prev(0), m_data(ValueType())
        {
        }

        explicit Link(ConstReferenceType v) : m_next(0), m_prev(0), m_data(v)
        {
        }

        ~Link()
        {
        }

        SK_INLINE ReferenceType getLink(void)
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
    typedef ReferenceType        LinkPtrRef;
    typedef ConstReferenceType   ConstLinkPtrRef;
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
            node = node->m_next;
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
    typedef T*       LinkPtrRef;
    typedef const T* ConstLinkPtrRef;

    typedef skListBase<LinkPtr, LinkPtr> BaseType;

    typedef skListIterator<skListClass<T> >              Iterator;
    typedef skListReverseIterator<skListClass<T> >       ReverseIterator;
    typedef const skListIterator<skListClass<T> >        ConstIterator;
    typedef const skListReverseIterator<skListClass<T> > ConstReverseIterator;

    SK_IMPLEMENT_SORT(T, skListClass);

    class Link
    {
    public:
        Link() : m_next(0), m_prev(0), m_data(0)
        {
        }

        ~Link()
        {
        }
        SK_INLINE LinkPtrRef getLink(void)
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
    skListClass& operator=(const skListClass& rhs);
    skListClass(const skListClass& rhs);

    mutable BaseType m_list;
};

#endif  //_skList_h_
