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
#ifndef _skBinarySearchTree_h_
#define _skBinarySearchTree_h_

#include "Config/skConfig.h"
#include "Utils/skTraits.h"
#include "Utils/skArray.h"

template < typename T >
class skBinarySearchTree
{
public:
    SK_DECLARE_TYPE(T);

    class Node : public skAllocObject
    {

    public:

        Node() : m_left(0), m_right(0) {}
        Node(const Node& oth) : m_left(oth.m_left), m_right(oth.m_right), m_data(oth.m_data) {}
        Node(ConstValueType v) : m_left(0), m_right(0), m_data(v) {}
        ~Node() { destruct(); }

        SK_INLINE Node*         left(void)   { return m_left; }
        SK_INLINE Node*         right(void)  { return m_right; }
        SK_INLINE ReferenceType data(void)   { return m_data; }

    private:
        friend class skBinarySearchTree;

        Node* m_left, *m_right;
        ValueType m_data;

        void destruct(void)
        {
            if (m_left)  delete m_left;
            if (m_right) delete m_right;
        }
    };

    SK_DECLARE_TYPE_NAME(Node, Node);

    typedef skArray<T> Array;
    typedef typename Array::Iterator Iterator;

private:
    NodePointerType     m_root;
    SKsize              m_size;
    Array               m_array;

public:
    skBinarySearchTree() :
        m_root(0),
        m_size(0)
    {
    }

    skBinarySearchTree(const skBinarySearchTree& o) :
        m_root(0),
        m_size(0)
    {
    }

    ~skBinarySearchTree() { clear(); }

    void clear(void)
    {
        delete m_root;
        m_root = 0;
        m_array.clear();
    }


    void insert(ConstReferenceType val)
    {
        if (m_root == 0)
            m_root = new Node(val);
        else
            insert_recursive(m_root, val);

        m_size++;
    }


    bool find(ConstReferenceType val) const
    {
        if (m_root == 0)
            return false;

        return find_recursive(m_root, val) != 0;
    }

    bool findNonRecursive(ConstReferenceType val) const
    {
        if (m_root == 0)
            return false;

        NodePointerType node = m_root;
        while (node != 0)
        {
            if (node->m_data == val)
                return true;
            else if (node->m_data < val)
                node = node->m_right;
            else
                node = node->m_left;
        }
        return false;
    }


    void erase(ConstReferenceType val)
    {
        if (m_root == 0)
            return;

        m_root = erase_recursive(m_root, val);

    }

    Iterator iterator_ascending(void)
    {
        m_array.resize(0);
        populate(m_root, false);
        return m_array.iterator();
    }

    Iterator iterator_decending(void)
    {
        m_array.resize(0);
        populate(m_root, true);
        return m_array.iterator();
    }


    SK_INLINE NodePointerType root(void)    { return m_root; }
    SK_INLINE NodePointerType left(void)    { return m_root? m_root->m_left : 0; }
    SK_INLINE NodePointerType right(void)   { return m_root ? m_root->m_right : 0; }
    SK_INLINE SKsize          size(void)    { return m_size; }


private:
    void insert_recursive(NodePointerType node, ConstReferenceType val)
    {
        if (val < node->m_data)
        {
            if (node->m_left)
                insert_recursive(node->m_left, val);
            else
                node->m_left = new Node(val);
        }
        else
        {
            if (node->m_right)
                insert_recursive(node->m_right, val);
            else
                node->m_right = new Node(val);
        }
    }

    NodePointerType find_recursive(NodePointerType node, ConstReferenceType val) const
    {
        if (!node)
            return 0;

        if (node->m_data == val)
            return node;

        if (node->m_data > val)
            return find_recursive(node->m_left, val);

        return find_recursive(node->m_right, val);
    }


    NodePointerType erase_recursive(NodePointerType node, ConstReferenceType val)
    {
        if (!node) return 0;

        if (node->m_data > val)
            node->m_left = erase_recursive(node->m_left, val);
        else if (node->m_data < val)
            node->m_right = erase_recursive(node->m_right, val);
        else
            return detach(node);
        return node;
    }

    NodePointerType detach(NodePointerType node)
    {
        SK_ASSERT(node);

        if (node->m_left == 0 && node->m_right == 0)
        {
            --m_size;
            delete node;
            node = 0;
        }
        else if (node->m_left == 0 || node->m_right == 0)
        {
            NodePointerType local = node->m_left == 0 ? node->m_right : node->m_left;
            node->m_left = node->m_right = 0;

            --m_size;
            delete node;
            node = local;
        }
        else
        {
            NodePointerType cur = node->m_right;
            while (cur && cur->m_left != 0)
                cur = cur->m_left;

            node->m_data = cur->m_data;
            node->m_right = erase_recursive(node->m_right, cur->m_data);

        }
        return node;
    }


    void populate(NodePointerType node, bool descending)
    {
        if (!node)
            return;

        populate(descending ? node->m_right : node->m_left, descending);
        m_array.push_back(node->m_data);
        populate(descending ? node->m_left : node->m_right, descending);
    }

};





#endif//_skBinarySearchTree_h_
