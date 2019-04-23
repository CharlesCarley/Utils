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

#ifndef _skEndian_h_
#define _skEndian_h_

#include "skAllocator.h"


typedef enum skEndian {
    SK_ENDIAN_IS_BIG       = 0,
    SK_ENDIAN_IS_LITTLE    = 1,
    SK_ENDIAN_NATIVE,
} skEndian;

typedef union skEndianTest {
    SKbyte  bo[4];
    SKint32 test;
} skEndianTest;



SK_INLINE skEndian skGetEndian(void)
{
    skEndianTest e;
    e.test = SK_ENDIAN_IS_LITTLE;
    return static_cast<skEndian>(e.bo[0]);
}


SK_INLINE bool skIsEndian(const skEndian& endian)
{
    skEndianTest e;
    e.test = endian;
    return static_cast<skEndian>(e.bo[0]) == endian;
}

SK_INLINE SKuint16 skSwap16(SKuint16 in)
{
    return static_cast<SKuint16>(((in & 0xFF00) >> 8) | ((in & 0x00FF) << 8));
}

SK_INLINE SKuint32 skSwap32(const SKuint32& in)
{
    return (((in & 0xFF000000) >> 24) | ((in & 0x00FF0000) >> 8) | ((in & 0x0000FF00) << 8)  | ((in & 0x000000FF) << 24));
}

SK_INLINE SKint16 skSwap16(SKint16 in)
{
    return skSwap16(static_cast<SKuint16>(in));
}

SK_INLINE SKint32 skSwap32(const SKint32& in)
{
    return skSwap32(static_cast<SKuint32>(in));
}


SK_INLINE SKuint64 skSwap64(const SKuint64& in)
{
    SKuint64 r = 0;
    const SKubyte* src = reinterpret_cast<const SKubyte*>(&in);
    SKubyte* dst = reinterpret_cast<SKubyte*>(&r);

    dst[0] = src[7];
    dst[1] = src[6];
    dst[2] = src[5];
    dst[3] = src[4];
    dst[4] = src[3];
    dst[5] = src[2];
    dst[6] = src[1];
    dst[7] = src[0];
    return r;
}

SK_INLINE SKint64 skSwap64(const SKint64& in)
{
    return skSwap64(static_cast<SKuint64>(in));
}



#endif//_skEndian_h_
