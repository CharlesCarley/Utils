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
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1
#endif
#include "Utils/skChar.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include "skString.h"

SKsize skChar::length(const char* in)
{
    return in && *in ? (SKsize)strlen(in) : 0;
}

void skChar::copy(char* dest, const char* src)
{
    if (dest && src && *src)
        strcpy(dest, src);
}

void skChar::copyn(char* dest, const char* src, SKsize max)
{
    if (dest && src && *src)
    {
        SKsize len = length(src);

        // restrict it to max
        if (len > max)
            len = max;

        ::strncpy(dest, src, (size_t)len);

        if (len <= max)
            dest[len] = 0;
    }
}

SKsize skChar::equals(const char* a, const char* b)
{
    if (!a || !b)
        return 1;
    if (*a != *b)
        return 1;
    return (SKsize)strcmp(a, b);
}

SKsize skChar::equalsn(const char* a, const char* b, const SKsize max)
{
    if (!a || !b)
        return 1;
    if (*a != *b)
        return 1;
    return (SKsize)(size_t)strncmp(a, b, (size_t)max);
}

SKint16 skChar::toInt16(const char* in, SKint16 def, int base)
{
    if (in && *in)
    {
        const long long v = std::strtoll(in, nullptr, base);
        if (v > std::numeric_limits<SKint16>::max())
            return std::numeric_limits<SKint16>::max();
        if (v < std::numeric_limits<SKint16>::min())
            return std::numeric_limits<SKint16>::min();
        return (SKint16)v;
    }
    return def;
}

SKint32 skChar::toInt32(const char* in, SKint32 def, int base)
{
    if (in && *in)
    {
        const long long v = std::strtoll(in, nullptr, base);
        if (v > std::numeric_limits<SKint32>::max())
            return std::numeric_limits<SKint32>::max();
        if (v < std::numeric_limits<SKint32>::min())
            return std::numeric_limits<SKint32>::min();
        return (SKint32)v;
    }
    return def;
}

SKint64 skChar::toInt64(const char* in, SKint64 def, int base)
{
    if (in && *in)
        return (SKint64)std::strtoll(in, nullptr, base);
    return def;
}

SKuint16 skChar::toUint16(const char* in, SKuint16 def, int base)
{
    if (in && *in)
    {
        const unsigned long long v = std::strtoull(in, nullptr, base);
        if (v > std::numeric_limits<SKuint16>::max())
            return std::numeric_limits<SKuint16>::max();
        return (SKuint16)v;
    }
    return def;
}

SKuint32 skChar::toUint32(const char* in, SKuint32 def, int base)
{
    if (in && *in)
    {
        const unsigned long long v = std::strtoull(in, nullptr, base);
        if (v > std::numeric_limits<SKuint32>::max())
            return std::numeric_limits<SKuint32>::max();
        return (SKuint32)v;
    }
    return def;
}

SKuint64 skChar::toUint64(const char* in, SKuint64 def, int base)
{
    if (in && *in)
        return (SKuint64)std::strtoull(in, nullptr, base);
    return def;
}

SKint16 skChar::toInt16(const skString& in, SKint16 def, int base)
{
    return toInt16(in.c_str(), def, base);
}

SKint32 skChar::toInt32(const skString& in, SKint32 def, int base)
{
    return toInt32(in.c_str(), def, base);
}

SKint64 skChar::toInt64(const skString& in, SKint64 def, int base)
{
    return toInt64(in.c_str(), def, base);
}

SKuint16 skChar::toUint16(const skString& in, SKuint16 def, int base)
{
    return toUint16(in.c_str(), def, base);
}

SKuint32 skChar::toUint32(const skString& in, SKuint32 def, int base)
{
    return toUint32(in.c_str(), def, base);
}

SKuint64 skChar::toUint64(const skString& in, SKuint64 def, int base)
{
    return toUint64(in.c_str(), def, base);
}

bool skChar::isNullOrEmpty(const char* in)
{
    return !in || !*in;
}


bool skChar::toBool(const char* in)
{
    if (in)
    {
        switch (*in)
        {
        case 't':
            return strncmp(in, "true", 4) == 0;
        case 'y':
            return strncmp(in, "yes", 3) == 0;
        case '1':
            return strncmp(in, "1", 1) == 0;
        default:
            break;
        }
    }
    return false;
}

float skChar::toFloat(const char* in, const float& def)
{
    if (in && *in)
        return std::strtof(in, nullptr);
    return def;
}

double skChar::toDouble(const char* in, const double& def)
{
    if (in && *in)
        return std::strtod(in, nullptr);
    return def;
}

bool skChar::toBool(const skString& in)
{
    return toBool(in.c_str());
}

float skChar::toFloat(const skString& in, const float& def)
{
    return toFloat(in.c_str(), def);
}

double skChar::toDouble(const skString& in, const double& def)
{
    return toDouble(in.c_str(), def);
}

void skChar::toString(skString& dest, float v)
{
    const double d = (double)v;
    dest.reserve(32);
    const int size = snprintf(dest.ptr(), dest.capacity(), "%f", d);
    if (size < (int)dest.capacity() && size > 0)
        dest.resize(size);
}

void skChar::toString(skString& dest, double v)
{
    dest.reserve(32);
    const int size = snprintf(dest.ptr(), dest.capacity(), "%f", v);
    if (size < (int)dest.capacity() && size > 0)
        dest.resize(size);
}

void skChar::toString(skString& dest, bool v)
{
    dest.reserve(2);
    dest.resize(0);
    if (v)
        dest.append('1');
    else
        dest.append('0');
}

void skChar::toString(skString& dest, SKint16 v)
{
    dest.reserve(7);
    const int size = snprintf(dest.ptr(), dest.capacity(), "%d", v);
    if (size < (int)dest.capacity() && size > 0)
        dest.resize(size);
}

void skChar::toString(skString& dest, SKint32 v)
{
    dest.reserve(12);
    const int size = snprintf(dest.ptr(), dest.capacity(), "%d", v);
    if (size < (int)dest.capacity() && size > 0)
        dest.resize(size);
}

void skChar::toString(skString& dest, SKint64 v)
{
    dest.reserve(21);
    const int size = snprintf(dest.ptr(), dest.capacity(), "%lld", v);
    if (size < (int)dest.capacity() && size > 0)
        dest.resize(size);
}

void skChar::toString(skString& dest, SKuint16 v)
{
    dest.reserve(16);
    const int size = snprintf(dest.ptr(), dest.capacity(), "%u", v);
    if (size < (int)dest.capacity() && size > 0)
        dest.resize(size);
}

void skChar::toString(skString& dest, SKuint32 v)
{
    dest.reserve(24);
    const int size = snprintf(dest.ptr(), dest.capacity(), "%u", v);
    if (size < (int)dest.capacity() && size > 0)
        dest.resize(size);
}

void skChar::toString(skString& dest, SKuint64 v)
{
    dest.reserve(32);
    const int size = snprintf(dest.ptr(), dest.capacity(), "%llu", v);
    if (size < (int)dest.capacity() && size > 0)
        dest.resize(size);
}

skString skChar::toString(float v)
{
    skString copyOnReturn;
    toString(copyOnReturn, v);
    return copyOnReturn;
}

skString skChar::toString(double v)
{
    skString copyOnReturn;
    toString(copyOnReturn, v);
    return copyOnReturn;
}

skString skChar::toString(bool v)
{
    skString copyOnReturn;
    toString(copyOnReturn, v);
    return copyOnReturn;
}

skString skChar::toString(SKint16 v)
{
    skString copyOnReturn;
    toString(copyOnReturn, v);
    return copyOnReturn;
}

skString skChar::toString(SKint32 v)
{
    skString copyOnReturn;
    toString(copyOnReturn, v);
    return copyOnReturn;
}

skString skChar::toString(SKint64 v)
{
    skString copyOnReturn;
    toString(copyOnReturn, v);
    return copyOnReturn;
}

skString skChar::toString(SKuint16 v)
{
    skString copyOnReturn;
    toString(copyOnReturn, v);
    return copyOnReturn;
}

skString skChar::toString(SKuint32 v)
{
    skString copyOnReturn;
    toString(copyOnReturn, v);
    return copyOnReturn;
}

skString skChar::toString(SKuint64 v)
{
    skString copyOnReturn;
    toString(copyOnReturn, v);
    return copyOnReturn;
}
