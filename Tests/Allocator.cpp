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
#include "Utils/skAllocator.h"
#include "catch/catch.hpp"

class Construct
{
public:
    static int construct;
    int        test;
    Construct() :
        test(123)
    {
        construct = 1;
    }

    ~Construct()
    {
        construct = 0;
    }
};

int Construct::construct = -1;

typedef skMallocAllocator<Construct> Malloc;
typedef skNewAllocator<Construct>    Alloc;

typedef skMallocAllocator<int, SKuint8, 10> IntMalloc;

TEST_CASE("DefaultAlloc malloc/free")
{
    Malloc     alloc;
    Construct* ptr = alloc.allocate();

    EXPECT_EQ(alloc.npos, SK_NPOS);
    EXPECT_EQ(alloc.limit, SK_MAX);

    EXPECT_TRUE(ptr != 0);
    EXPECT_EQ(Construct::construct, 1);

    alloc.deallocate(ptr);

    EXPECT_EQ(Construct::construct, 0);
}

TEST_CASE("Alloc new/delete")
{
    Alloc      alloc;
    Construct* ptr = alloc.allocate();

    EXPECT_EQ(alloc.npos, SK_NPOS);
    EXPECT_EQ(alloc.limit, SK_MAX);

    EXPECT_TRUE(ptr != 0);
    EXPECT_EQ(Construct::construct, 1);

    alloc.deallocate(ptr);

    EXPECT_EQ(Construct::construct, 0);
}

TEST_CASE("IntMalloc limit malloc/free")
{
    IntMalloc alloc;
    try
    {
        int* ptr = alloc.array_allocate(40, 0);
        alloc.array_deallocate(ptr);
    }
    catch (IntMalloc::SizeType st)
    {
        EXPECT_EQ(st, 30);
    }
}

TEST_CASE("IntMalloc array_allocate default constructor")
{
    IntMalloc alloc;

    int* ptr = alloc.array_allocate(5, 123);
    EXPECT_TRUE(ptr != 0);

    if (ptr != 0)
    {
        int i;
        for (i = 0; i < 5; ++i)
        {
            int I = ptr[i];
            EXPECT_EQ(I, 123);
        }
    }

    alloc.array_deallocate(ptr);
}
