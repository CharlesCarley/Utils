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
    typedef typename T::LinkPointerType             LinkPointerType;
    typedef typename T::LinkValueReferenceType      LinkValueReferenceType;
    typedef typename T::LinkValueConstReferenceType LinkValueConstReferenceType;

private:
    mutable LinkPointerType m_cur;

public:
    skListIterator() :
        m_cur(nullptr)
    {
    }

    skListIterator(LinkPointerType first) :
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

    bool hasMoreElements(void) const
    {
        return m_cur != nullptr;
    }

    void next(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        m_cur = m_cur->getNext();
    }

    LinkValueReferenceType getNext(void)
    {
        SK_ITER_DEBUG(hasMoreElements());
        LinkValueReferenceType ret = m_cur->getData();
        m_cur                      = m_cur->getNext();
        return ret;
    }

    LinkValueConstReferenceType getNext(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        LinkValueReferenceType ret = m_cur->getData();
        m_cur                      = m_cur->getNext();
        return ret;
    }

    LinkValueReferenceType peekNext(void)
    {
        SK_ITER_DEBUG(hasMoreElements());
        return m_cur->getData();
    }

    LinkValueConstReferenceType peekNext(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        return m_cur->getData();
    }
};

template <typename T>
class skListReverseIterator
{
public:
    typedef typename T::LinkPointerType             LinkPointerType;
    typedef typename T::LinkValueReferenceType      LinkValueReferenceType;
    typedef typename T::LinkValueConstReferenceType LinkValueConstReferenceType;

private:
    mutable LinkPointerType m_cur;

public:
    skListReverseIterator() :
        m_cur(0)
    {
    }

    skListReverseIterator(LinkPointerType first) :
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

    bool hasMoreElements(void) const
    {
        return m_cur != 0;
    }

    void next(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        m_cur = m_cur->getPrev();
    }

    LinkValueReferenceType getNext(void)
    {
        SK_ITER_DEBUG(hasMoreElements());
        LinkValueReferenceType ret = m_cur->getData();
        m_cur                      = m_cur->getPrev();
        return ret;
    }

    LinkValueConstReferenceType getNext(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        LinkValueReferenceType ret = m_cur->getData();
        m_cur                      = m_cur->getPrev();
        return ret;
    }

    LinkValueReferenceType peekNext(void)
    {
        SK_ITER_DEBUG(hasMoreElements());
        return m_cur->getData();
    }

    LinkValueConstReferenceType peekNext(void) const
    {
        SK_ITER_DEBUG(hasMoreElements());
        return m_cur->getData();
    }
};

template <typename T>
class skSinglyLinkedList
{
public:
    SK_DECLARE_TYPE(T)

    typedef skSinglyLinkedList<T>          SelfType;
    typedef skListIterator<SelfType>       Iterator;
    typedef const skListIterator<SelfType> ConstIterator;

    class Link
    {
    public:
        SK_DECLARE_TYPE(Link)

    public:
        Link() :
            m_next(nullptr)
        {
        }

        Link(typename SelfType::ConstReferenceType v) :
            m_next(nullptr),
            m_data(v)
        {
        }

        ~Link()
        {
        }

        PointerType getNext(void)
        {
            return m_next;
        }

        typename SelfType::ReferenceType getData(void)
        {
            return m_data;
        }

    private:
        friend class skSinglyLinkedList;

        PointerType m_next;

        typename SelfType::ValueType m_data;
    };

    typedef Link*              LinkPointerType;
    typedef ReferenceType      LinkValueReferenceType;
    typedef ConstReferenceType LinkValueConstReferenceType;

private:
    Link*    m_head;
    Link*    m_tail;
    SKuint32 m_size;

public:
    skSinglyLinkedList() :
        m_head(nullptr),
        m_tail(nullptr),
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
        while (node != nullptr)
        {
            Link* tLink = node->m_next;
            delete node;
            node = tLink;
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

                while (next != nullptr && next->m_data <= v)
                {
                    prev = next;
                    next = next->m_next;
                }
                prev->m_next = node;
                node->m_next = next;
                if (next == nullptr)
                    m_tail = prev;
            }
        }
        m_size++;
    }

    ValueType pop_back(void)
    {
        SK_ASSERT(m_head);
        Link *link = m_head, *prev = nullptr;
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

        Link *prev = nullptr, *found = nullptr;

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

    SKuint32 find(ConstValueType v)
    {
        if (!m_head)
            return -1;

        SKuint32 foundIndex = 0;
        Link*    node       = m_head;
        while (node)
        {
            if (node->m_data == v)
                break;
            foundIndex++;
            node = node->m_next;
        }
        return foundIndex;
    }

    ReferenceType at(const SKuint32 idx)
    {
        SK_ASSERT(idx != -1 && idx < m_size);

        SKuint32 foundIndex = 0;
        Link*    node       = m_head;
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

    Link* first(void)
    {
        return m_head;
    }

    Link* last(void)
    {
        return m_tail;
    }

    SKuint32 size(void) const
    {
        return m_size;
    }

    bool empty(void) const
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
        if (!m_head || !prev || !pos)
            return;

        Link *node = m_head, *last = nullptr;
        while (node)
        {
            if (node->m_data == v)
            {
                *prev = last;
                *pos  = node;
                return;
            }
            last = node;
            node = node->m_next;
        }
        *prev = 0;
        *pos  = 0;
    }
};

template <typename T, typename LinkPointerType, class SubType>
class skListBase
{
public:
    SK_DECLARE_TYPE(T)

public:
    skListBase() :
        m_first(nullptr),
        m_last(nullptr),
        m_size(0)
    {
    }

    ~skListBase()
    {
        clear();
    }

    void clear(void)
    {
        m_first = nullptr;
        m_last  = nullptr;
        m_size  = 0;
    }

    bool push_back(LinkPointerType link)
    {
        bool result = false;
        if (link != nullptr)
        {
            link->m_prev = m_last;
            if (m_last)
                m_last->m_next = link;

            if (!m_first)
                m_first = link;

            m_last = link;
            ++m_size;
            result = true;
        }
        return result;
    }

    bool push_front(LinkPointerType link)
    {
        bool result = false;

        if (link != nullptr)
        {
            link->m_next = m_first;

            if (m_first)
                m_first->m_prev = link;

            if (!m_last)
                m_last = link;

            m_first = link;
            ++m_size;
            result = true;
        }
        return result;
    }

    bool insert_front(LinkPointerType inFront, LinkPointerType elem)
    {
        bool result = false;

        if (inFront != nullptr && elem != nullptr)
        {
            elem->m_next    = inFront;
            elem->m_prev    = inFront->m_prev;
            inFront->m_prev = elem;

            if (elem->m_prev)
                elem->m_prev->m_next = elem;

            if (m_first == inFront)
                m_first = elem;

            ++m_size;

            result = true;
        }
        return result;
    }

    LinkPointerType find_link(ConstReferenceType v)
    {
        LinkPointerType node = m_first;

        while (node)
        {
            if (node->m_data == v)
                return node;

            node = node->m_next;
        }

        return nullptr;
    }

    LinkPointerType link_at(SKuint32 pos) const
    {
        SKuint32        i    = 0;
        LinkPointerType node = m_first;

        while (node)
        {
            if (i == pos)
                return node;

            node = node->m_next;
            ++i;
        }

        return nullptr;
    }

    bool erase_link(LinkPointerType link)
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

    LinkPointerType first(void)
    {
        return m_first;
    }

    LinkPointerType last(void)
    {
        return m_last;
    }

    SKuint32 size(void) const
    {
        return m_size;
    }

private:
    friend SubType;

    LinkPointerType m_first;
    LinkPointerType m_last;
    SKuint32        m_size;
};

template <typename T>
class skList
{
public:
    SK_DECLARE_TYPE(T)

public:
    typedef skList<T>                       SelfType;
    typedef skListIterator<SelfType>        Iterator;
    typedef skListReverseIterator<SelfType> ReverseIterator;
    typedef const skListIterator<SelfType>  ConstIterator;
    typedef const skListIterator<SelfType>  ConstReverseIterator;

    SK_IMPLEMENT_SORT(T, skList, SKuint32)

    class Link
    {
    public:
        Link() :
            m_next(nullptr),
            m_prev(nullptr),
            m_data(ValueType())
        {
        }

        explicit Link(ConstReferenceType v) :
            m_next(nullptr),
            m_prev(nullptr),
            m_data(v)
        {
        }

        ~Link()
        {
        }

        ReferenceType getData(void)
        {
            return m_data;
        }

        Link* getNext(void)
        {
            return m_next;
        }

        Link* getPrev(void)
        {
            return m_prev;
        }

    protected:
        Link *    m_next, *m_prev;
        ValueType m_data;

        friend class skList<T>;
        friend class skListBase<T, Link*, SelfType>;
        friend class skListIterator<skList<T> >;
        friend class skListReverseIterator<skList<T> >;
    };

    typedef Link*                                    LinkPointerType;
    typedef skListBase<T, LinkPointerType, SelfType> BaseType;
    typedef ReferenceType                            LinkValueReferenceType;
    typedef ConstReferenceType                       LinkValueConstReferenceType;

public:
    skList() :
        m_list()
    {
    }

    skList(const SelfType& rhs) :
        m_list()
    {
        this->replicate(rhs);
    }

    ~skList()
    {
        clear();
    }

    void clear(void)
    {
        LinkPointerType node = m_list.m_first;
        while (node)
        {
            Link* temp = node;
            node       = node->m_next;
            delete temp;
        }

        m_list.clear();
    }

    void push_back(ConstReferenceType v)
    {
        m_list.push_back(new Link(v));
    }

    void push_front(ConstReferenceType v)
    {
        m_list.push_front(new Link(v));
    }

    void insert_front(LinkPointerType link, ConstReferenceType v)
    {
        m_list.insert_front(link, new Link(v));
    }

    LinkPointerType find(ConstReferenceType v)
    {
        return m_list.find_link(v);
    }

    ReferenceType at(SKuint32 index)
    {
        SK_ASSERT(index < m_list.m_size);
        LinkPointerType node = m_list.link_at(index);

        SK_ASSERT(node);
        return node->m_data;
    }

    ConstReferenceType at(SKuint32 index) const
    {
        SK_ASSERT(index < m_list.m_size);
        LinkPointerType node = m_list.link_at(index);

        SK_ASSERT(node);
        return node->m_data;
    }

    LinkPointerType link_at(SKuint32 index)
    {
        return m_list.link_at(index);
    }

    void erase(LinkPointerType link)
    {
        if (m_list.erase_link(link))
            delete link;
    }

    void erase(ConstReferenceType v)
    {
        LinkPointerType fnd = m_list.find_link(v);

        if (fnd)
        {
            if (m_list.erase_link(fnd))
                delete fnd;
        }
    }

    void pop_back(void)
    {
        erase(m_list.m_last);
    }

    void pop_front(void)
    {
        erase(m_list.m_first);
    }

    bool empty(void) const
    {
        return m_list.m_size <= 0;
    }

    SKuint32 size(void) const
    {
        return m_list.m_size;
    }

    LinkPointerType begin(void)
    {
        return m_list.m_first;
    }

    LinkPointerType end(void)
    {
        return m_list.m_last;
    }

    ConstPointerType begin(void) const
    {
        return m_list.m_first;
    }

    ConstPointerType end(void) const
    {
        return m_list.m_last;
    }

    ReferenceType front(void)
    {
        SK_ASSERT(m_list.m_first);
        return m_list.m_first->m_data;
    }

    ReferenceType back(void)
    {
        SK_ASSERT(m_list.m_last);
        return m_list.m_last->m_data;
    }

    Iterator iterator(void)
    {
        return m_list.m_first ? Iterator(m_list.m_first) : Iterator();
    }

    ReverseIterator reverseIterator(void)
    {
        return m_list.m_last ? ReverseIterator(m_list.m_last) : ReverseIterator();
    }

    ConstIterator iterator(void) const
    {
        return m_list.m_last ? ConstIterator(m_list.m_last) : ConstIterator();
    }

    ConstReverseIterator reverseIterator(void) const
    {
        return m_list.m_last ? ConstReverseIterator(m_list.m_last) : ConstReverseIterator();
    }

    skList& operator=(const SelfType& rhs)
    {
        if (this != &rhs)
            replicate(rhs);
        return *this;
    }

private:
    friend class skListBase<T, LinkPointerType, SelfType>;

    mutable BaseType m_list;

    void replicate(const SelfType& rhs)
    {
        clear();
        ConstIterator it = rhs.iterator();
        while (it.hasMoreElements())
            push_back(it.getNext());
    }
};

template <typename T>
class skListClass
{
public:
    SK_DECLARE_TYPE(T)

public:
    typedef T*       LinkPointerType;
    typedef T*       LinkValueReferenceType;
    typedef const T* LinkValueConstReferenceType;

    typedef skListClass<T>                                         SelfType;
    typedef skListBase<LinkPointerType, LinkPointerType, SelfType> BaseType;
    typedef skListIterator<skListClass<T> >                        Iterator;
    typedef skListReverseIterator<skListClass<T> >                 ReverseIterator;
    typedef const skListIterator<skListClass<T> >                  ConstIterator;
    typedef const skListReverseIterator<skListClass<T> >           ConstReverseIterator;

    SK_IMPLEMENT_SORT(T, skListClass, SKuint32)

    class Link
    {
    public:
        Link() :
            m_next(nullptr),
            m_prev(nullptr),
            m_data(nullptr)
        {
        }

        virtual ~Link()
        {
        }

        LinkValueReferenceType getData(void)
        {
            return m_data;
        }

        LinkPointerType getNext(void)
        {
            return m_next;
        }

        LinkPointerType getPrev(void)
        {
            return m_prev;
        }

    protected:
        LinkPointerType m_next, m_prev;
        LinkPointerType m_data;

        friend class skListClass<T>;
        friend class skListBase<LinkPointerType, LinkPointerType, SelfType>;
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

    void push_back(LinkPointerType v)
    {
        if (v)
        {
            static_cast<Link*>(v)->m_data = v;
            m_list.push_back(v);
        }
    }

    void push_front(LinkPointerType v)
    {
        if (v)
        {
            static_cast<Link*>(v)->m_data = v;
            m_list.push_front(v);
        }
    }

    LinkPointerType at(SKuint32 index)
    {
        SK_ASSERT(index < m_list.size());
        Link* node = m_list.link_at(index);

        SK_ASSERT(node);
        return node->m_data;
    }

    LinkPointerType at(SKuint32 index) const
    {
        SK_ASSERT(index < m_list.size());
        Link* node = m_list.link_at(index);
        SK_ASSERT(node);
        return node->m_data;
    }

    LinkPointerType link_at(SKuint32 index)
    {
        return m_list.link_at(index);
    }

    void erase(LinkPointerType v)
    {
        return m_list.erase_link(v);
    }

    void pop_back(void)
    {
        m_list.erase_link(m_list.last());
    }

    void pop_front(void)
    {
        m_list.erase_link(m_list.first());
    }

    bool empty(void) const
    {
        return m_list.size() == 0;
    }

    SKuint32 size(void) const
    {
        return m_list.size();
    }

    LinkPointerType begin(void)
    {
        return static_cast<LinkPointerType>(m_list.first());
    }

    LinkPointerType end(void)
    {
        return static_cast<LinkPointerType>(m_list.last());
    }

    LinkValueConstReferenceType begin(void) const
    {
        return static_cast<const LinkPointerType>(m_list.first());
    }

    LinkValueConstReferenceType end(void) const
    {
        return static_cast<const LinkPointerType>(m_list.last());
    }

    LinkPointerType front(void)
    {
        SK_ASSERT(m_list.first());
        return static_cast<LinkPointerType>(m_list.first()->m_data);
    }

    LinkPointerType back(void)
    {
        SK_ASSERT(m_list.last());
        return static_cast<LinkPointerType>(m_list.last()->m_data);
    }

    Iterator iterator(void)
    {
        return m_list.first() ? Iterator(m_list.first()) : Iterator();
    }

    ReverseIterator reverseIterator(void)
    {
        return m_list.last() ? ReverseIterator(m_list.last()) : ReverseIterator();
    }

    ConstIterator iterator(void) const
    {
        return m_list.first() ? ConstIterator(m_list.first()) : ConstIterator();
    }

    ConstReverseIterator reverseIterator(void) const
    {
        return m_list.last() ? ConstReverseIterator(m_list.last()) : ConstReverseIterator();
    }

private:
    skListClass& operator=(const skListClass&)
    {
        // no assignment
        return *this;
    }

    skListClass(const skListClass&)
    {
        // no copy
    }

    mutable BaseType m_list;
};

#endif  //_skList_h_
