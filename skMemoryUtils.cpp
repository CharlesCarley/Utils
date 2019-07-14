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
#include <memory.h>
#include <string.h>
#include "Config/skConfig.h"

#if SK_ALLOCATOR == 1

#define USE_DL_PREFIX
#include "alloc/malloc.c"

void* skMalloc(unsigned int size)
{
    return dlmalloc(size);
}

void* skRealloc(void* ptr, unsigned int size)
{
    return dlrealloc(ptr, size);
}

void* skCalloc(unsigned int size, unsigned int nr)
{
    return dlcalloc(size, nr);
}

void skFree(void* ptr)
{
    dlfree(ptr);
}

#else

#include <malloc.h>

void* skMalloc(unsigned int size)
{
    return malloc(size);
}

void* skRealloc(void* ptr, unsigned int size)
{
    return realloc(ptr, size);
}

void* skCalloc(unsigned int size, unsigned int nr)
{
    return calloc(size, nr);
}

void skFree(void* ptr)
{
    free(ptr);
}

#endif

void* skMemset(void* ptr, int val, unsigned int nr)
{
    return ::memset(ptr, val, nr);
}

void* skMemcpy(void* dst, const void* src, unsigned int nr)
{
    return ::memcpy(dst, src, nr);
}

int skMemcmp(const void* cmp0, const void* cmp1, unsigned int nr)
{
    return ::memcmp(cmp0, cmp1, nr);
}
