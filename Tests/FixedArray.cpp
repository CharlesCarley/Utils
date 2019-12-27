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
#include "catch/catch.hpp"
#include "Macro.h"
#include "Utils/skFixedArray.h"


#define ARR_SIZE 500
#define LIST_MOD 12

typedef skFixedArray<int, ARR_SIZE> IntArray;

int _F_Greater(int a, int b)    { return a > b; }
int _F_Less(int a, int b)       { return a < b; }




TEST_CASE("FixedSizeArrayTest_MaxBounds")
{
    IntArray lst;
    int i;
    for (i = 0; i < ARR_SIZE*2; ++i)
        lst.push_back(i);

    EXPECT_EQ(ARR_SIZE, lst.size());
}


TEST_CASE("FixedSizeArrayTest_PopBack")
{
    IntArray lst;
    int i;
    for (i = 0; i <= ARR_SIZE; ++i)
        lst.push_back(i);

    while (!lst.empty())
        lst.pop_back();


    EXPECT_EQ(0, lst.size());
}

TEST_CASE("FixedSizeArrayTest_remove")
{
    int i2[5] = { 7,8,6,2,6 };

    IntArray lst;
    lst.push_back<5>(i2);

    EXPECT_EQ(7, lst[0]);
    EXPECT_EQ(8, lst[1]);
    EXPECT_EQ(6, lst[2]);
    EXPECT_EQ(2, lst[3]);
    EXPECT_EQ(6, lst[4]);

    lst.remove(lst.find(7));
    lst.remove(lst.find(2));
    lst.sort(_F_Less);


    EXPECT_EQ(6, lst[0]);
    EXPECT_EQ(6, lst[1]);
    EXPECT_EQ(8, lst[2]);
}

TEST_CASE("FixedSizeArrayTest_arrayCount")
{
    IntArray lst;
    int i;

    for (i=0; i<ARR_SIZE; ++i)
        lst.push_back(i);

    EXPECT_EQ(ARR_SIZE, lst.size());

    for (i=0; i<ARR_SIZE; ++i)
    {
        lst.pop_back();
        EXPECT_EQ(ARR_SIZE - (i + 1), lst.size());
    }

    EXPECT_EQ(0, lst.size());
}


TEST_CASE("FixedSizeArrayTest_arrayCopyCtor")
{
    IntArray a;
    int i;

    for (i=0; i<ARR_SIZE; ++i)
        a.push_back(i);

    IntArray b(a);
    EXPECT_EQ(a.size(), b.size());

    IntArray::PointerType na, nb;
    na = a.ptr();
    nb = b.ptr();

    for (i=0; i<ARR_SIZE; ++i)
    {
        EXPECT_EQ(na[i], nb[i]);
    }
}

TEST_CASE("FixedSizeArrayTest_arrayAssignOp")
{
    IntArray a, b;
    int i;

    for (i=0; i<ARR_SIZE; ++i)
    {
        a.push_back(i);
        b.push_back(ARR_SIZE - i);
    }

    b = a;
    EXPECT_EQ(a.size(), b.size());

    IntArray::PointerType na, nb;
    na = a.ptr();
    nb = b.ptr();

    for (i=0; i<ARR_SIZE; ++i)
        EXPECT_EQ(na[i], nb[i]);
}


TEST_CASE("FixedSizeArrayTest_arrayFind")
{
    IntArray a;
    int i;

    for (i=0; i<ARR_SIZE; ++i)
        a.push_back(i);

    for (i=0; i<ARR_SIZE; ++i)
    {
        SKsize p = a.find(i);
        EXPECT_NE(a.npos, p);
    }
}


TEST_CASE("FixedSizeArrayTest_arrayRemoveMod")
{
    IntArray a;
    SKuint16 i, total_removed;

    for (i=0; i<ARR_SIZE; ++i)
    {
        a.push_back(i);
    }

    total_removed = 0;
    for (i=0; i<a.size(); ++i)
    {
        if ((a[i] % LIST_MOD) == 0)
        {
            SKuint16 pos = a.find(a[i]);
            if (pos != a.npos)
            {
                i = 0;
                a.remove(pos);
                ++total_removed;
            }
        }
    }

    EXPECT_EQ(a.size(), ARR_SIZE - total_removed);
    for (i=0; i<a.size(); ++i)
        EXPECT_NE(a[i] % LIST_MOD, 0);
}

void arrIterConstness(const IntArray& a)
{
    int last = a.back(), realLast = -1;

    IntArray::ConstIterator it = a.iterator();
    while (it.hasMoreElements())
    {
        const int& a = it.getNext();
        realLast = a;
        if (it.hasMoreElements())
        {
            const int& b = it.getNext();
            realLast = b;
            EXPECT_LT(a, b);
        }
    }

    EXPECT_EQ(last, realLast);
    last = a.front();
    realLast = -1;

    IntArray::ConstReverseIterator rit = a.reverseIterator();
    while (rit.hasMoreElements())
    {
        const int& a = rit.getNext();
        realLast = a;
        if (rit.hasMoreElements())
        {
            const int& b = rit.getNext();
            realLast = b;
            EXPECT_GT(a, b);
        }
    }

    EXPECT_EQ(last, realLast);
}



TEST_CASE("FixedSizeArrayTest_arrayIterators")
{
    IntArray a;
    int i;
    for (i=0; i<ARR_SIZE; ++i)
        a.push_back(i + 1);

    IntArray::Iterator it = a.iterator();
    while (it.hasMoreElements())
    {
        int& a = it.getNext();
        if (it.hasMoreElements())
        {
            int& b = it.getNext();
            EXPECT_LT(a, b);
        }
    }

    IntArray::ReverseIterator rit = a.reverseIterator();
    while (rit.hasMoreElements())
    {
        int& a = rit.getNext();
        if (rit.hasMoreElements())
        {
            int& b = rit.getNext();
            EXPECT_GT(a, b);
        }
    }

    arrIterConstness(a);
}


TEST_CASE("FixedSizeArrayTest_arraySort")
{
    IntArray a;
    SKuint16  i;
    for (i=0; i<ARR_SIZE; ++i)
        a.push_back(rand() % ARR_SIZE);

    a.sort(_F_Greater);
    SKuint16 j = 0, tot = a.size() - 1;


    for (j = 0; j< tot; j += 2)
        EXPECT_GE(a[j], a[j+1]);

    a.sort(_F_Less);
    j = 0, tot = a.size() - 1;
    for (j = 0; j< tot; j += 2)
        EXPECT_LE(a[j], a[j+1]);
}


TEST_CASE("FixedSizeArrayTest_arraySort2")
{
    IntArray a;
    a.resize(9);
    SKuint16 i = 0;
    a[i]   = 2;
    a[++i] = 0;
    a[++i] = 9;
    a[++i] = 3;
    a[++i] = 81;
    a[++i] = 47;
    a[++i] = 5;
    a[++i] = 7;
    a[++i] = 3;

    a.sort(_F_Less);
    SKuint16 j = 0, tot = a.size() - 1;
    for (j = 0; j < tot; j += 2)
        EXPECT_LE(a[j], a[j + 1]);
}


