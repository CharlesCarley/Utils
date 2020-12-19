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

#include "Utils/skDeque.h"
#include "catch/catch.hpp"

#define TEST_CASE_NAME DequeTest
#define ARR_SIZE 513
#define LIST_MOD 12

typedef skDeque<int> IntArray;

TEST_CASE("DequeTest_dequeCount")
{
    IntArray lst;
    int      i;
    for (i = 0; i < ARR_SIZE; ++i)
        lst.push_back(i);

    REQUIRE(ARR_SIZE == lst.size());

    for (i = 0; i < ARR_SIZE; ++i)
    {
        lst.pop_front();
        REQUIRE(ARR_SIZE - (i + 1) == lst.size());
    }

    REQUIRE(0 == lst.size());
}

TEST_CASE("DequeTest_DequeRefill")
{
    IntArray lst;
    int      j;
    for (j = 0; j < 5; ++j)
    {
        int i;
        for (i = 0; i < ARR_SIZE; ++i)
            lst.push_back(i);
        REQUIRE(ARR_SIZE == lst.size());

        lst.clear();
        REQUIRE(0 == lst.size());
    }
}

TEST_CASE("DequeTest_DequePushPop")
{
    IntArray lst;
    int      j;
    for (j = 0; j < 5; ++j)
    {
        int i;
        for (i = 0; i < ARR_SIZE; ++i)
        {
            lst.push_back(i);
            lst.pop_back();
        }
        REQUIRE(0 == lst.size());
    }
}

TEST_CASE("DequeTest_DequePushPopFront")
{
    IntArray lst;
    int      j;
    for (j = 0; j < 5; ++j)
    {
        int i;
        for (i = 0; i < ARR_SIZE; ++i)
        {
            lst.push_front(i);
            REQUIRE(i == lst.front());
            lst.pop_front();
        }
        REQUIRE(0 == lst.size());
    }
}
