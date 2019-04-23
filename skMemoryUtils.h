/*
-------------------------------------------------------------------------------

    Copyright (c) Charles Carley.

    All Rights Reserved.

-------------------------------------------------------------------------------
*/
#ifndef _skMemoryUtils_h_
#define _skMemoryUtils_h_

#include "Config/skConfig.h"

extern void*    skMalloc(unsigned int size);
extern void*    skRealloc(void* ptr, unsigned int size);
extern void*    skCalloc(unsigned int size, unsigned int nr);
extern void     skFree(void* ptr);
extern void*    skMemset(void* ptr, int val, unsigned int nr);
extern void*    skMemcpy(void* dst, const void* src, unsigned int nr);
extern int      skMemcmp(const void* cmp0, const void* cmp1, unsigned int nr);

#endif//_skMemoryUtils_h_
