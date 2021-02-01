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
#include "Utils/Config/skConfig.h"
#include "catch/catch.hpp"


TEST_CASE("SizeCheck")
{
#if SK_ARCH == SK_ARCH_64
    // 2^64-1
    EXPECT_EQ(SK_NPOS, 18446744073709551615);
    // 2^64/2-1
    EXPECT_EQ(SK_NPOSH, 9223372036854775807);
#else
    EXPECT_EQ(SK_NPOS, 4294967295);
    EXPECT_EQ(SK_NPOSH, 2147483647);
#endif
    // 2^32-1
    EXPECT_EQ(SK_NPOS32, 4294967295);
    // 2^32/2-1
    EXPECT_EQ(SK_NPOS32H, 2147483647);
    // 2^16-1
    EXPECT_EQ(SK_NPOS16, 65535);
    // 2^16/2-1
    EXPECT_EQ(SK_NPOS16H, 32767);
}
