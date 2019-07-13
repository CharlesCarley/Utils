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
#ifndef _skStringConverter_h_
#define _skStringConverter_h_

#include "skString.h"

class skStringConverter
{
public:
    static skString BLANK;

public:
    static void   toString(skString& dest, int v);
    static void   toString(skString& dest, long v);
    static void   toString(skString& dest, float v);
    static void   toString(skString& dest, double v);
    static void   toString(skString& dest, bool v);
    static int    toInt(const skString& src);
    static long   toLong(const skString& src);
    static float  toFloat(const skString& src);
    static double toDouble(const skString& src);
    static bool   toBool(const skString& src);
};

#endif //_skStringConverter_h_
