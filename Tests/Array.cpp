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
#include "Utils/skArray.h"
#include "catch/catch.hpp"

const int MaxSize = 100;
const int Limit   = 50;

typedef skNewAllocator<int, SKuint8, Limit> TinyAllocator;
typedef skArray<int>                        IntArray;
typedef skArray<int, TinyAllocator>         IntArraySmall;

TEST_CASE("Default Constructor")
{
    IntArray ia;
    REQUIRE(ia.empty());
    REQUIRE(ia.capacity() == 0);
    REQUIRE(ia.ptr() == nullptr);
}

TEST_CASE("Expanding Capacity")
{
    IntArray ia;
    for (int i = 0; i < MaxSize; ++i)
        ia.push_back(i);

    REQUIRE(ia.size() == MaxSize);
    //REQUIRE(ia.capacity() == 128);
    REQUIRE(ia.ptr() != nullptr);
}

TEST_CASE("Controlled Capacity")
{
    IntArray ia;
    ia.reserve(MaxSize);
    for (int i = 0; i < MaxSize; ++i)
        ia.push_back(i);

    REQUIRE(ia.size() == MaxSize);
    // REQUIRE(ia.capacity() == MaxSize);
    REQUIRE(ia.ptr() != nullptr);
}

TEST_CASE("Capacity Untouched")
{
    IntArray ia;
    ia.reserve(MaxSize);
    for (int i = 0; i < MaxSize; ++i)
        ia.push_back(i);

    ia.resize(0);
    REQUIRE(ia.empty());
    //REQUIRE(ia.capacity() == MaxSize);
    REQUIRE(ia.ptr() != nullptr);
}

TEST_CASE("Capacity And Data Reset")
{
    IntArray ia;
    ia.reserve(MaxSize);
    for (int i = 0; i < MaxSize; ++i)
        ia.push_back(i);

    ia.clear();

    REQUIRE(ia.empty());
    REQUIRE(ia.capacity() == 0);
    REQUIRE(ia.ptr() == nullptr);
}

TEST_CASE("Array Copy Constructor")
{
    IntArray ia;
    ia.reserve(MaxSize);

    int i;
    for (i = 0; i < MaxSize; ++i)
        ia.push_back(i);

    IntArray ib(ia);

    IntArray::ConstPointerType p1 = ia.ptr();
    IntArray::ConstPointerType p2 = ib.ptr();

    EXPECT_EQ(ia.size(), ib.size());
    EXPECT_EQ(ia.capacity() + 1, ib.capacity());

    for (i = 0; i < MaxSize; ++i)
    {
        EXPECT_EQ(p1[i], p2[i]);
    }
}

TEST_CASE("Array Assignment Operator")
{
    IntArray ia;
    ia.reserve(MaxSize);

    int i;
    for (i = 0; i < MaxSize; ++i)
        ia.push_back(i);

    IntArray                   ib = ia;
    IntArray::ConstPointerType p1 = ia.ptr();
    IntArray::ConstPointerType p2 = ib.ptr();

    EXPECT_EQ(ia.size(), ib.size());
    EXPECT_EQ(ia.capacity() + 1, ib.capacity());

    for (i = 0; i < MaxSize; ++i)
    {
        EXPECT_EQ(p1[i], p2[i]);
    }
}

TEST_CASE("Small Array Maxed")
{
    IntArraySmall ia;
    ia.reserve(MaxSize);

    EXPECT_EQ(ia.size(), 0);
    EXPECT_EQ(ia.capacity(), Limit);
}

TEST_CASE("Push to max")
{
    IntArraySmall ia;
    int           i;

    try
    {
        for (i = 0; i < MaxSize; ++i)
            ia.push_back(i);
    }
    catch (const TinyAllocator::SizeType l)
    {
        EXPECT_EQ(l, Limit);
    }

    EXPECT_EQ(ia.size(), Limit);
    EXPECT_EQ(ia.capacity(), Limit);
}
