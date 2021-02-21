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
#include "skStringConverter.h"
#include "skPlatformHeaders.h"

using namespace skStringUtils;

const skString skStringConverter::BLANK = "";

void skStringConverter::toString(skString& dest, int v)
{
    dest = skString::format("%d", v);
}

void skStringConverter::toString(skString& dest, long v)
{
    dest = skString::format("%d", v);
}

void skStringConverter::toString(skString& dest, float v)
{
    dest = skString::format("%.4f", v);
}

void skStringConverter::toString(skString& dest, double v)
{
    dest = skString::format("%f", v);
}

void skStringConverter::toString(skString& dest, bool v)
{
    dest = skString::format("%s", v ? "true" : "false");
}


void skStringConverter::toString(skString& dest, SKuint64 v)
{
    dest = skString::format("%I64d", v);
}


int skStringConverter::toInt(const skString& src, int defaultValue, int base)
{
    return skStringUtils::toInt(src.c_str(), defaultValue, base);
}

SKint64 skStringConverter::toInt64(const skString& src, SKint64 defaultValue, int base)
{
    return skStringUtils::toInt64(src.c_str(), defaultValue, base);
}

long skStringConverter::toLong(const skString& src, long defaultValue, int base)
{
    return skStringUtils::toLong(src.c_str(), defaultValue, base);
}

float skStringConverter::toFloat(const skString& src)
{
    return skStringUtils::toFloat(src.c_str());
}

double skStringConverter::toDouble(const skString& src)
{
    return skStringUtils::toDouble(src.c_str());
}

bool skStringConverter::toBool(const skString& src)
{
    return equalsn(src.c_str(), "true", 4) == 0 ||
           equalsn(src.c_str(), "yes", 3) == 0 ||
           equalsn(src.c_str(), "1", 1) == 0;
}
