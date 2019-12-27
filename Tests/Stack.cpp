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
#include "Utils/skStack.h"

#define TEST_CASE_CASE_NAME skStack
#define STACK_SIZE   100
#define STACK_MOD    12

typedef skStack<int> Stack;


TEST_CASE("Stack_Construct")
{
    Stack a;
    EXPECT_EQ(0, a.size());
    EXPECT_EQ(0, a.itop());
    EXPECT_EQ(0, a.capacity());
    EXPECT_EQ(0, a.ptr());
}

TEST_CASE("Stack_Reserve")
{
    Stack a;
    a.reserve(STACK_SIZE);

    EXPECT_EQ(0, a.size());
    EXPECT_EQ(0, a.itop());
    //EXPECT_EQ(STACK_SIZE+1, a.capacity());
    EXPECT_NE(0, a.ptr());
}


TEST_CASE("Stack_push_pop")
{
    Stack a;
    int i;
    for (i=0; i<STACK_SIZE; ++i)
    {
        a.push(i + 1);
        EXPECT_EQ(i + 1, a.top());
    }

    EXPECT_EQ(STACK_SIZE, a.size());

    for (i=0; i<STACK_SIZE; ++i)
    {
        
        int t = a.top();
        a.pop();
        EXPECT_EQ(STACK_SIZE - i, t);
    }

    EXPECT_EQ(0, a.size());
}


TEST_CASE("Stack_copy_ctor")
{
    Stack a;

    int i;

    for (i=0; i<STACK_SIZE; ++i)
        a.push(i);

    Stack b(a);

    EXPECT_EQ(a.size(), b.size());

    Stack::PointerType na, nb;


    na = a.ptr();
    nb = b.ptr();

    for (i=0; i<STACK_SIZE; ++i)
        EXPECT_EQ(na[i], nb[i]);
}


TEST_CASE("Stack_assign_operator")
{
    Stack a, b;
    int i;

    for (i=0; i<STACK_SIZE; ++i)
    {
        a.push(i);
        b.push(STACK_SIZE - i);
    }

    b = a;
    EXPECT_EQ(a.size(), b.size());


    Stack::PointerType na, nb;
    na = a.ptr();
    nb = b.ptr();

    for (i=0; i<STACK_SIZE; ++i)
        EXPECT_EQ(na[i], nb[i]);
}

void Test_ConstIterator(const Stack& a)
{
    Stack::ConstIterator it = a.iterator();
    while (it.hasMoreElements())
    {
        const int& a = it.getNext();
        if (it.hasMoreElements())
        {
            const int& b = it.getNext();
            EXPECT_GT(a, b);
        }
    }

}



TEST_CASE("Stack_stackIterators")
{
    Stack a;
    int i;

    for (i=0; i<STACK_SIZE; ++i)
        a.push(i + 1);

    Stack::Iterator it = a.iterator();
    while (it.hasMoreElements())
    {
        int& a = it.getNext();
        if (it.hasMoreElements())
        {
            int& b = it.getNext();
            EXPECT_GT(a, b);
        }
    }
    Test_ConstIterator(a);
}

