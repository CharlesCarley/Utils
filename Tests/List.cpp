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
#include "Utils/skList.h"
#include "Utils/skTimer.h"
#include "catch/catch.hpp"
#include "Macro.h"


typedef skSinglyLinkedList<int> ListBase;
const int                       MaxSize = 10;


int squ(int a)
{
    return a * a;
}


TEST_CASE("List Default Constructor")
{
    ListBase lb;
    REQUIRE(lb.first() == 0);
    REQUIRE(lb.last() == 0);
    REQUIRE(lb.size() == 0);
}


TEST_CASE("List Size Test")
{
    ListBase lb;
    for (int i = 0; i < MaxSize; ++i)
    {
        int result = 4 * squ(i) - 12 * (i - 1) - 3;
        if (i == 0)
        {
            REQUIRE(lb.first() == lb.last());
        }
        else if (i > 1)
        {
            REQUIRE(lb.first() != lb.last());
        }


        lb.push_back(result);
        REQUIRE(lb.size() == i + 1);
        REQUIRE(lb.last() != 0);
        REQUIRE(squ(i * 2 - 3) == lb.last()->getData());
    }
    REQUIRE(lb.size() == MaxSize);
}



TEST_CASE("List Push")
{
    ListBase lb;
    for (int i = 0; i < MaxSize; ++i)
    {
        int result = i + 1;
        if (i == 0)
        {
            REQUIRE(lb.first() == lb.last());
        }
        else if (i > 1)
        {
            REQUIRE(lb.first() != lb.last());
        }

        if (i % 2 == 0)
            lb.push_front(result);
        else
            lb.push_back(result);
    }

    REQUIRE(lb.size() == MaxSize);
}


TEST_CASE("List_PushOrdered")
{
    srand(skGetTickCount());
    ListBase lb;
    for (int i = 0; i < MaxSize; ++i)
    {
        int result = MaxSize * (rand() % squ(MaxSize));
        if (i == 0)
        {
            REQUIRE(lb.first() == lb.last());
        }
        lb.push_ordered(result);
        REQUIRE(lb.size() == i + 1);
        REQUIRE(lb.last() != 0);
    }
    REQUIRE(lb.size() == MaxSize);
    int p = -1;

    ListBase::ConstIterator it = lb.const_iterator();
    while (it.hasMoreElements())
    {
        const int &v = it.getNext();
        EXPECT_GE(v, p);
        p = v;
    }
}

TEST_CASE("List_PopBack")
{
    ListBase lb;
    for (int i = 0; i < MaxSize; ++i)
    {
        int result = i + 1;
        if (lb.size() == 0)
        {
            REQUIRE(lb.first() == lb.last());
        }
        else if (i > 1)
        {
            REQUIRE(lb.first() != lb.last());
        }

        lb.push_front(result);
        lb.pop_back();
    }

    REQUIRE(lb.size() == 0);
}
