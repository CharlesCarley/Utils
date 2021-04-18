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
#ifndef __skChar_h__
#define __skChar_h__

#include "Utils/Config/skConfig.h"

class skString;

class skChar
{
public:
    static SKsize length(const char* in);

    static void copy(char* dest, const char* src);

    static void copyn(char* dest, const char* src, SKsize max);

    static SKsize equals(const char* a, const char* b);

    static SKsize equalsn(const char* a, const char* b, SKsize max);

    [[deprecated]] static SKint32 toInt(const char* in, SKint32 def = -1, int base = 10)
    {
        return toInt32(in, def, base);
    }

    static SKint16 toInt16(const char* in, SKint16 def = -1, int base = 10);

    static SKint32 toInt32(const char* in, SKint32 def = -1, int base = 10);

    static SKint64 toInt64(const char* in, SKint64 def = -1, int base = 10);

    static SKuint16 toUint16(const char* in, SKuint16 def = SK_NPOS16, int base = 10);

    static SKuint32 toUint32(const char* in, SKuint32 def = SK_NPOS32, int base = 10);

    static SKuint64 toUint64(const char* in, SKuint64 def = SK_NPOS, int base = 10);

    [[deprecated]] static SKint32 toInt(const skString& in, SKint32 def = -1, int base = 10)
    {
        return toInt32(in, def, base);
    }

    static SKint16 toInt16(const skString& in, SKint16 def = -1, int base = 10);

    static SKint32 toInt32(const skString& in, SKint32 def = -1, int base = 10);

    static SKint64 toInt64(const skString& in, SKint64 def = -1, int base = 10);

    static SKuint16 toUint16(const skString& in, SKuint16 def = SK_NPOS16, int base = 10);

    static SKuint32 toUint32(const skString& in, SKuint32 def = SK_NPOS32, int base = 10);

    static SKuint64 toUint64(const skString& in, SKuint64 def = SK_NPOS, int base = 10);

    static bool isNullOrEmpty(const char* in);

    static bool toBool(const char* in);

    static float toFloat(const char* in, const float& def = 0.f);

    static double toDouble(const char* in, const double& def = 0.0);

    static bool toBool(const skString& in);

    static float toFloat(const skString& in, const float& def = 0.f);

    static double toDouble(const skString& in, const double& def = 0.0);

    static void toString(skString& dest, float v);

    static void toString(skString& dest, double v);

    static void toString(skString& dest, bool v);

    static void toString(skString& dest, SKint16 v);

    static void toString(skString& dest, SKint32 v);

    static void toString(skString& dest, SKint64 v);

    static void toString(skString& dest, SKuint16 v);

    static void toString(skString& dest, SKuint32 v);

    static void toString(skString& dest, SKuint64 v);

    static skString toString(float v);

    static skString toString(double v);

    static skString toString(bool v);

    static skString toString(SKint16 v);

    static skString toString(SKint32 v);

    static skString toString(SKint64 v);

    static skString toString(SKuint16 v);

    static skString toString(SKuint32 v);

    static skString toString(SKuint64 v);
};

#endif  //__skChar_h__
