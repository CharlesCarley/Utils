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
#ifndef _skMemoryUtils_h_
#define _skMemoryUtils_h_

#include "Utils/Config/skConfig.h"

extern void* skMalloc(SKsize size);
extern void* skRealloc(void* ptr, SKsize size);
extern void* skCalloc(SKsize size, SKsize nr);
extern void  skFree(void* ptr);


#define SK_NO_HEADERS 0

#if SK_NO_HEADERS != 1

// #include "Utils/skPlatformHeaders.h"
#define skMemset(ptr, val, nr) ::memset(ptr, val, nr)
#define skMemcpy(dst, src, nr) ::memcpy(dst, src, nr)
#define skMemcmp(cp0, cp1, nr) ::memcmp(cp0, cp1, nr)


#else

extern void* skMemset(void* ptr, int val, unsigned int nr);
extern void* skMemcpy(void* dst, const void* src, unsigned int nr);
extern int   skMemcmp(const void* cmp0, const void* cmp1, unsigned int nr);


#endif



#endif  //_skMemoryUtils_h_
