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
#include "Macro.h"
#include "Utils/skMap.h"
#include "Utils/skString.h"
#include "catch/catch.hpp"

typedef skHashTable<skString, SKuint32> StrMap;
typedef skHashTable<SKuint32, SKuint32> IntMap;

typedef skHashSet<skString> StrSet;
typedef skHashSet<SKuint32> IntSet;

TEST_CASE("HashTable_BasicMap")
{
    StrMap map;

    for (int i = 0; i < 10; ++i)
        map.insert(skString::format("%i", i), i);

    bool f = map.find("0") != map.npos;
    EXPECT_TRUE(f);

    f = map.find("10") != map.npos;
    EXPECT_FALSE(f);
}

TEST_CASE("HashTable_MapIterBasic")
{
    IntMap map;
    for (int i = 0; i < 10; ++i)
        map.insert((SKuint32)i, i);

    IntMap::Iterator it = map.iterator();
    while (it.hasMoreElements())
    {
        IntMap::Pair& p = it.getNext();
        EXPECT_EQ(p.first, p.second);
    }
}

void MapIter_Constness(const IntMap& map)
{
    IntMap::ConstIterator it = map.iterator();
    while (it.hasMoreElements())
    {
        const IntMap::Pair& p = it.getNext();
        EXPECT_GT(p.first, p.second);

        if (it.hasMoreElements())
        {
            EXPECT_LT(p.first, it.peekNextKey());
            EXPECT_LT(p.second, it.peekNextValue());
        }
    }

    IntMap::ConstReverseIterator rit = map.reverseIterator();
    while (rit.hasMoreElements())
    {
        const IntMap::Pair& p = rit.getNext();
        EXPECT_GT(p.first, p.second);

        if (rit.hasMoreElements())
        {
            EXPECT_GT(p.first, rit.peekNextKey());
            EXPECT_GT(p.second, rit.peekNextValue());
        }
    }
}

TEST_CASE("HashTable_MapIter")
{
    IntMap map;
    for (int i = 0; i < 10; ++i)
        map.insert(10 + i, i);

    IntMap::Iterator it = map.iterator();
    while (it.hasMoreElements())
    {
        IntMap::Pair& p = it.getNext();
        EXPECT_GT(p.first, p.second);

        if (it.hasMoreElements())
        {
            EXPECT_LT(p.first, it.peekNextKey());
            EXPECT_LT(p.second, it.peekNextValue());
        }
    }

    IntMap::ReverseIterator rit = map.reverseIterator();
    while (rit.hasMoreElements())
    {
        IntMap::Pair& p = rit.getNext();
        EXPECT_GT(p.first, p.second);

        if (rit.hasMoreElements())
        {
            EXPECT_GT(p.first, rit.peekNextKey());
            EXPECT_GT(p.second, rit.peekNextValue());
        }
    }

    MapIter_Constness(map);
}

TEST_CASE("HashTable_BasicSet")
{
    StrSet set;
    for (int i = 0; i < 10; ++i)
        set.insert(skString::format("%i", i));

    bool f = set.find("0") != set.npos;
    EXPECT_TRUE(f);

    f = set.find("10") != set.npos;
    EXPECT_FALSE(f);
}

TEST_CASE("HashTable_SetIterBasic")
{
    IntSet map;
    for (int i = 0; i < 10; ++i)
        map.insert(i);

    int              t  = 0;
    IntSet::Iterator it = map.iterator();
    while (it.hasMoreElements())
    {
        IntSet::ValueType& p = it.getNext();
        EXPECT_EQ(p.first, t);
        ++t;
    }
}

void SetIter_Constness(const IntSet& map)
{
    SKsize t;

    t                        = 0;
    IntSet::ConstIterator it = map.iterator();
    while (it.hasMoreElements())
    {
        const IntSet::ValueType& p = it.getNext();
        EXPECT_EQ(p.first, t);
        ++t;
    }

    t = map.size() - 1;

    IntSet::ConstReverseIterator rit = map.reverseIterator();
    while (rit.hasMoreElements())
    {
        const IntSet::ValueType& p = rit.getNext();
        EXPECT_EQ(p.first, t);
        --t;
    }
}

TEST_CASE("HashTable_SetIter")
{
    IntSet map;
    for (int i = 0; i < 10; ++i)
        map.insert(i);

    SKsize t;

    t = 0;

    IntSet::Iterator it = map.iterator();
    while (it.hasMoreElements())
    {
        IntSet::ValueType& p = it.getNext();
        EXPECT_EQ(p.first, t);
        ++t;
    }

    t = map.size() - 1;

    IntSet::ReverseIterator rit = map.reverseIterator();
    while (rit.hasMoreElements())
    {
        IntSet::ValueType& p = rit.getNext();
        EXPECT_EQ(p.first, t);
        --t;
    }

    SetIter_Constness(map);
}
