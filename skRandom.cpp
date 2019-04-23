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
#include <stdlib.h>
#include "skRandom.h"
#include "skTimer.h"


void skRandInit(SKuint32 seed)
{
    if (seed != SK_NPOS)
        ::srand(seed);
    else
        ::srand(skGetTickCount() % (SK_NPOS-1));
}


SKuint32 skRandomUnsignedInt(SKuint32 mod)
{
    int r = ::rand();
    if (r < 0) r = -r;

    if (mod != SK_NPOS)
        r %= mod;

    return (SKuint32)r;
}


double skUnitRandom(SKuint32 mod)
{
    if (mod == 0)       mod = 256;
    if (mod == SK_NPOS) mod = 256;
    return (double)skRandomUnsignedInt(mod) / (double)mod;
}
