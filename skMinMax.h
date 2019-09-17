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

#include <new>
#include "Config/skConfig.h"

template <typename T>
void skSwap(T& a, T& b)
{
    T c(a);
    a = b;
    b = c;
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
    return v < a ? a : v > b ? b : v;
}

template <typename T>
void skFill(T* dst, T* src, const SKsize nr)
{
    if (nr > 0 && nr != -1)
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
    if (nr > 0 && nr != -1)
    {
        SKsize i = 0;
        do
            dst[i] = src;
        while (++i < nr);
    }
}

template <typename T>
void skConstruct(T* beg, T* end, const T& x)
{
    while (beg != end)
    {
        new (beg) T(x);
        ++beg;
    }
}

template <typename T>
void skConstructDefault(T* beg, T* end)
{
    while (beg != end)
    {
        new (beg) T();
        ++beg;
    }
}

template <typename T>
void skConstructCopy(T* beg, T* end, T* oth)
{
    while (beg != end)
    {
        new (beg) T(*oth++);
        ++beg;
    }
}

template <typename T>
void skDestruct(T* beg, T* end)
{
    while (beg != end)
    {
        beg->~T();
        ++beg;
    }
}

#endif  //_skMinMax_h_
