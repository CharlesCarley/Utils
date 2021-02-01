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
#include <cstdlib>
#include <cstring>
#include "Config/skConfig.h"


void* skMalloc(SKsize size)
{
    return ::malloc(size);
}

void* skRealloc(void* ptr, SKsize size)
{
    return ::realloc(ptr, size);
}

void* skCalloc(SKsize size, SKsize nr)
{
    return ::calloc(size, nr);
}

void skFree(void* ptr)
{
    ::free(ptr);
}

#if SK_NO_HEADERS != 1


void* skMemset(void* ptr, int val, SKsize nr)
{
    return ::memset(ptr, val, nr);
}

void* skMemcpy(void* dst, const void* src, SKsize nr)
{
    return ::memcpy(dst, src, nr);
}

int skMemcmp(const void* cmp0, const void* cmp1, SKsize nr)
{
    return ::memcmp(cmp0, cmp1, nr);
}

#endif
