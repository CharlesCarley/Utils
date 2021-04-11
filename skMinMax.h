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
#ifndef _skMinMax_h_
#define _skMinMax_h_

#include "Config/skConfig.h"

template <typename T>
void skSwap(T& a, T& b)
{
    T c(a);
    a = b, b = c;
}

template <typename T>
SK_INLINE T skABS(const T& a)
{
    return a < 0 ? -a : a;
}

template <typename T>
SK_INLINE T skMin(const T& a, const T& b)
{
    return a < b ? a : b;
}

template <typename T>
SK_INLINE T skMin3(const T& a, const T& b, const T& c)
{
    return skMin(a, skMin(b, c));
}

template <typename T>
SK_INLINE T skMax(const T& a, const T& b)
{
    return a > b ? a : b;
}

template <typename T>
SK_INLINE T skMax3(const T& a, const T& b, const T& c)
{
    return skMax(a, skMax(b, c));
}

template <typename T>
SK_INLINE T skClamp(const T& v, const T& a, const T& b)
{
    return v < a ? a : (v > b ? b : v);
}

template <typename T>
void skFill(T* dst, T* src, const SKsize nr)
{
    if (nr > 0 && nr != SK_NPOS)
    {
        SKsize i = 0;
        do
            dst[i] = src[i];
        while (++i < nr);
    }
}

template <typename T>
void skFill(T* dst, const T& src, const SKsize nr)
{
    if (nr > 0 && nr != SK_NPOS)
    {
        SKsize i = 0;
        do
            dst[i] = src;
        while (++i < nr);
    }
}

#endif  //_skMinMax_h_
