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

#include "Utils/skQueue.h"
#include "catch/catch.hpp"

#define ARR_SIZE 20
#define LIST_MOD 12

typedef skQueue<int> Queue;

TEST_CASE("Queue Test 1")
{
    Queue lst;
    int   i;

    for (int k = 0; k < 4; ++k)
    {
        for (i = 0; i < ARR_SIZE; ++i)
            lst.enqueue(i + 1);

        i = 0;
        while (!lst.empty())
        {
            int v = lst.dequeue();
            printf("%3i ", v);
            if (i++ % 20 == 19)
                printf("\n");

            if (i % 3 == 0)
                lst.enqueue(v);
        }
    }
    printf("\n");
}
