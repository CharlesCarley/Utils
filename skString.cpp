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
#include "skString.h"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include "skFixedString.h"
#include "skPlatformHeaders.h"
#include "skStringConverter.h"

namespace skStringUtils
{
    bool toBool(const char* in)
    {
        return equals(in, "true") == 0 || equals(in, "yes") == 0 || equals(in, "1") == 0;
    }

    long toLong(const char* in, long defaultValue, int base)
    {
        if (in && *in)
            return (long)std::strtol(in, nullptr, base);
        return defaultValue;
    }

    SKint64 toInt64(const char* in, SKint64 defaultValue, int base)
    {
        if (in && *in)
            return (SKint64)std::strtoll(in, nullptr, base);
        return defaultValue;
    }

    int toInt(const char* in, int defaultValue, int base)
    {
        if (in && *in)
            return (int)std::strtol(in, nullptr, base);
        return defaultValue;
    }

    float toFloat(const char* in)
    {
        if (in && *in)
            return (float)atof(in);
        return 0.f;
    }

    double toDouble(const char* in)
    {
        if (in && *in)
            return (double)atof(in);
        return 0.0;
    }

    SKsize length(const char* in)
    {
        return in && *in ? (SKsize)::strlen(in) : 0;
    }

    void copyn(char* dest, const char* src, SKsize nr)
    {
        size_t len = length(src);
        if (len > nr)
            len = nr;

        ::strncpy(dest, src, (size_t)len);

        if (len <= nr)
            dest[len] = 0;
    }

    void copy(char* out, const char* in)
    {
        if (out && in && *in)
            ::strcpy(out, in);
    }

    SKsize equalsn(const char* a, const char* b, SKsize max)
    {
        if (!a || !b)
            return 1;
        if (*a != *b)
            return 1;
        return (SKsize)(size_t)strncmp(a, b, (size_t)max);
    }

    SKsize equals(const char* a, const char* b)
    {
        if (!a || !b)
            return 1;
        if (*a != *b)
            return 1;
        return (SKsize)strcmp(a, b);
    }

    const SKuint8 HexTable[16] = {
        '0',
        '1',
        '2',
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9',
        'A',
        'B',
        'C',
        'D',
        'E',
        'F',
    };

    const SKuint16 BinaryTable[9] = {256, 128, 64, 32, 16, 8, 4, 2, 1};
}  // namespace skStringUtils

const SKsize   skString::npos  = -1;
const skString skString::Blank = skStringConverter::BLANK;

////

skString::skString() :
    m_data(nullptr),
    m_size(0),
    m_capacity(0)
{
}

skString::skString(const ValueType* str, SKsize len) :
    m_data(nullptr),
    m_size(0),
    m_capacity(0)
{
    alloc(str, len);
}

skString::skString(const skString& str) :
    m_data(nullptr),
    m_size(0),
    m_capacity(0)
{
    alloc(str.c_str(), str.size());
}

skString::skString(const char ch, SKsize nr) :
    m_data(nullptr),
    m_size(0),
    m_capacity(0)
{
    alloc(ch, nr);
}

skString::~skString()
{
    clear();
}

void skString::clear(void)
{
    if (m_data)
    {
        delete[] m_data;
        m_data = nullptr;
    }
    m_size     = 0;
    m_capacity = 0;
}

void skString::reserve(SKsize nr)
{
    if (m_capacity < nr)
    {
        auto* ptr = new ValueType[nr + 1];
        memset(ptr, 0, nr + 1);
        if (m_data)
        {
            skMemcpy(ptr, m_data, m_size);
            delete[] m_data;
        }

        m_data     = ptr;
        m_capacity = nr;
    }
}

void skString::assign(const skString& rhs)
{
    if (rhs.empty())
    {
        clear();
        return;
    }

    if (m_size < rhs.size())
        resize(rhs.size());

    SK_ASSERT(m_data);
    if (!m_data)
        return;
    skMemcpy(m_data, rhs.m_data, m_size);
    m_data[m_size] = 0;
}

void skString::resize(SKsize nr)
{
    if (nr < m_size)
        m_data[nr] = 0;
    else if (nr > m_size)
        reserve(nr);
    m_size = nr;
}

bool skString::equals(const skString& rhs) const
{
    return !skStringUtils::equals(m_data, rhs.m_data);
}

bool skString::equals(const char* rhs) const
{
    return !skStringUtils::equalsn(m_data, rhs, m_size);
}

void skString::alloc(const char* str, SKsize len)
{
    if (!str)
        return;

    if (!len)  // == 0 by default
        len = skStringUtils::length(str);

    if (len > 0)
    {
        reserve(len);
        if (m_data)
        {
            skStringUtils::copyn(m_data, str, len);
            m_data[len] = 0;
            m_size      = len;
        }
    }
}

void skString::alloc(const char p, SKsize len)
{
    m_size = len;
    reserve(m_size);

    if (m_data)
    {
        while (len != npos)
            m_data[len--] = p;
        m_data[m_size] = 0;
    }
}

void skString::alloc(SKsize len, const void* p)
{
    if (len != npos && p != nullptr)
    {
        resize(len);
        skMemcpy(m_data, p, m_size);
        m_data[m_size] = 0;
    }
}

skString& skString::operator=(const skString& rhs)
{
    if (this != &rhs)
        skString(rhs).swap(*this);
    return *this;
}

void skString::swap(skString& rhs)
{
    skSwap(m_data, rhs.m_data);
    skSwap(m_size, rhs.m_size);
    skSwap(m_capacity, rhs.m_capacity);
}

skString skString::substr(SKsize pos, SKsize nr) const
{
    skString s;
    substr(s, pos, nr);
    return s;
}

void skString::substr(skString& dest, SKsize pos, SKsize nr) const
{
    if (nr == npos || pos == npos)
        return;
    if (pos >= m_size)
        return;
    if (nr == 0)
        nr = m_size;
    if (nr + pos > m_size)
        nr = m_size - pos;
    skString oth(m_data + pos, nr);
    dest.swap(oth);
}

void skString::split(skArray<skString>& dst, const char* op) const
{
    if (m_size == 0)
        return;

    dst.reserve(32);
    skString     sub;
    SKsize       i, j = 0;
    const SKsize len = strlen(op);

    for (i = 0; i < m_size && i != npos;)
    {
        i = find(op, j);

        if (i == 0)
        {
            j += len;
            continue;
        }

        if (i != npos)
        {
            substr(sub, j, i);
            if (!sub.empty())
                dst.push_back(sub);

            j += i + len;
        }
        else
        {
            if (j < m_size && j != npos)
            {
                substr(sub, j, m_size);
                if (!sub.empty())
                    dst.push_back(sub);
            }
            break;
        }
    }
}

skString skString::format(const char* fmt, ...)
{
    char    buf[1025];
    va_list lst;
    va_start(lst, fmt);
    int size = skp_printf(buf, 1024, fmt, lst);
    va_end(lst);

    if (size < 0)
        size = 1024;

    buf[size] = 0;
    return skString(buf, size);
}

void skString::format(skString& dst, const char* fmt, ...)
{
    if (fmt)
    {
        va_list lst;

        int size = (int)dst.capacity();
        if (size <= 0)
        {
            va_start(lst, fmt);
            size = std::vsnprintf(nullptr, 0, fmt, lst);
            va_end(lst);

            dst.reserve((SKsize)size + 1);
        }

        if (size > 0)
        {
            va_start(lst, fmt);
            std::vsnprintf(dst.ptr(), dst.capacity(), fmt, lst);
            va_end(lst);
        }
    }
}

SKsize skString::find(char ch) const
{
    if (m_data != nullptr)
    {
        char* ptr = strchr(m_data, (int)ch);

        if (ptr)
        {
            const SKsize diff = ptr - m_data;
            if (diff > 0)
                return (SKsize)diff;
        }
    }
    return npos;
}

SKsize skString::find(const char* ch, SKsize offs) const
{
    if (m_data != nullptr)
    {
        if (offs >= m_size)
            return npos;

        char* sp  = m_data + offs;
        char* ptr = strstr(sp, ch);

        if (ptr)
        {
            const SKsize diff = ptr - sp;

            if (diff != SK_NPOS)
                return (SKsize)diff;
        }
    }

    return npos;
}

skString& skString::append(char ch)
{
    if (m_size + 1 > m_capacity)
        reserve(m_size + 1);

    m_data[m_size]   = ch;
    m_data[++m_size] = 0;

    return *this;
}

skString& skString::append(const char* rhs, SKsize rhsLen)
{
    if (rhs)
    {
        if (!rhsLen)
            rhsLen = skStringUtils::length(rhs);

        reserve(m_size + rhsLen);

        char* src = m_data + m_size;
        skMemcpy(src, rhs, rhsLen);

        m_size += rhsLen;
        m_data[m_size] = 0;
    }

    return *this;
}

SKsize skString::copy(char* arr, SKsize nr, SKsize offs) const
{
    if (!arr || !m_data || nr + offs > m_size)
        return 0;

    char* src = m_data + offs;

    SKsize i;

    for (i = 0; i < nr; ++i)
        arr[i] = src[i];

    return i;
}

skString& skString::erase(SKsize pos, SKsize nr)
{
    if (!m_data)
        return *this;

    if (nr == 0 && pos < m_size)
    {
        m_size         = pos;
        m_data[m_size] = 0;
        return *this;
    }

    if (nr >= m_size && pos == 0)
    {
        m_size         = 0;
        m_data[m_size] = 0;
        return *this;
    }

    const SKsize left = pos;
    if (left > m_size)
        return *this;

    SKsize right = left + nr;
    if (right > m_size)
        right = m_size;

    swap(substr(0, left).append(substr(right, m_size)));
    return *this;
}

void skString::toHex(void)
{
    if (m_size == 0)
        return;

    const SKsize old_size = m_size;
    resize(m_size * 2);

    const char* cp = m_data;
    char*       dp = m_data;

    if (cp == nullptr)
        return;

    int iVal, dv, rv;

    SKsize i, j = m_size;
    for (i = old_size - 1; i != npos; --i)
    {
        iVal = (int)(unsigned char)cp[i];
        dv   = iVal / 16;
        rv   = iVal % 16;
        if (iVal < 256)
        {
            dp[--j] = skStringUtils::HexTable[rv];
            dp[--j] = skStringUtils::HexTable[dv];
        }
    }

    dp[m_size] = 0;
}

void skString::fromHex(void)
{
    skString result;
    result.resize(m_size / 2);
    const char* cp = m_data;
    char*       dp = result.ptr();

    int    dv, rv;
    SKsize i, j = 0;
    for (i = 0; i < m_size; i += 2)
    {
        const char c0 = cp[i + 0];
        const char c1 = cp[i + 1];

        dv = rv = 0;
        if (c0 >= '0' && c0 <= '9')
            dv = (int)c0 - '0';
        else if (c0 >= 'A' && c0 <= 'Z')
            dv = 10 + (int)c0 - 'A';
        if (c1 >= '0' && c1 <= '9')
            rv = (int)c1 - '0';
        else if (c1 >= 'A' && c1 <= 'Z')
            rv = 10 + (int)c1 - 'A';
        dv      = 16 * dv + rv;
        dp[j++] = (char)dv;
        dp[j]   = 0;
    }

    swap(result);
}

void skString::toBinary(void)
{
    SKsize   i;
    skString s;
    SKuint16 c, j, counter;
    s.reserve(9 * m_size + 2);

    for (i = 0; i < m_size; ++i)
    {
        c = (SKuint8)m_data[i];

        if (c < skStringUtils::BinaryTable[0])
        {
            char tempBuf[12];
            counter = 0;

            if (i != 0)
                tempBuf[counter++] = 0x20;

            for (j = 0; j < 9; ++j)
            {
                if (c >= skStringUtils::BinaryTable[j])
                {
                    tempBuf[counter++] = 0x31;
                    c -= skStringUtils::BinaryTable[j];
                }
                else
                    tempBuf[counter++] = 0x30;
            }

            tempBuf[counter] = 0x00;
            s += tempBuf;
        }
    }
    swap(s);
}

void skString::fromBinary(void)
{
    skString s;
    SKuint16 c, j;

    skStringArray sa;
    split(sa, " ");

    skStringArray::Iterator it = sa.iterator();
    while (it.hasMoreElements())
    {
        const skString& cs = it.getNext();
        c                  = 0;
        for (j = 8; j > 0; j--)
        {
            if (j < cs.size() && cs[j] == 0x31)
                c += skStringUtils::BinaryTable[j];
        }
        s.append((SKbyte)c);
    }
    this->operator=(s);
}

void skString::encrypt(const SKbyte* lb, int b1, const SKuint16* ub, int b2)
{
    SKsize i, j = 0, r;
    double a, b;

    SKuint16* sh;
    for (i = 0; i < m_size; ++i)
        m_data[i] = (SKbyte)(SKubyte)((m_data[i] + (i % 3 ? -1 : 1) * lb[i % b1]) % 256);

    r  = m_size % 2;
    sh = new SKuint16[m_size + r + 1];

    sh[m_size + r] = 0;

    for (i = 0; i < m_size; i += 2)
    {
        a = (double)(SKuint8)m_data[i + 0];
        b = (double)(SKuint8)m_data[i + 1];

        sh[j++] = (SKuint16)((b + a / 256.0) * 256.0);
    }

    for (i = 0; i < j; i++)
        sh[i] = (SKuint16)sh[i] + ub[i % b2] % 65536;

    alloc(j * 2 + r, sh);
    delete[] sh;
}

void skString::decrypt(const SKbyte* lb, int b1, const SKuint16* ub, int b2)
{
    SKsize i, j = 0, r;
    double a, b;
    char   aa, bb;

    SKuint16 s;
    skString dest;
    dest.resize(m_size);

    auto* sp = (SKuint16*)m_data;
    auto* dp = (SKbyte*)dest.ptr();

    r = m_size % 2;
    for (i = 0; i < m_size / 2; ++i)
    {
        s  = sp[i] - ub[i % b2] % 65536;
        a  = (double)s / 256.0;
        b  = (a - (int)a) * 256.0;
        a  = (int)a;
        aa = (SKbyte)(int)a;
        bb = (SKbyte)(int)b;

        dp[j++] = bb;
        dp[j++] = aa;
    }

    for (i = 0; i < j; ++i)
        dp[i] = (dp[i] + (i % 3 ? 1 : -1) * lb[i % b1]) % 256;

    alloc(j - r, dp);
}

void skString::encrypt(void)
{
    const SKuint16 UB[] = {13673, 32696, 27676, 5443, 22638};
    const SKbyte   LB[] = {98, 83, 49, 81, 126};

    encrypt(LB, sizeof LB / sizeof LB[0], UB, sizeof UB / sizeof UB[0]);
}

void skString::decrypt(void)
{
    const SKuint16 UB[] = {13673, 32696, 27676, 5443, 22638};
    const SKbyte   LB[] = {98, 83, 49, 81, 126};

    decrypt(LB, sizeof LB / sizeof LB[0], UB, sizeof UB / sizeof UB[0]);
}

void skString::encrypt(const char* password)
{
    const SKuint16 UB[] = {13673, 32696, 27676, 5443, 22638};

    encrypt((const SKbyte*)password, (int)skStringUtils::length(password), UB, sizeof UB / sizeof UB[0]);
}

void skString::decrypt(const char* password)
{
    const SKuint16 UB[] = {13673, 32696, 27676, 5443, 22638};

    decrypt((const SKbyte*)password, (int)skStringUtils::length(password), UB, sizeof UB / sizeof UB[0]);
}

int skSprintf(char* dst, int maxSize, const char* fmt, ...)
{
    if (maxSize <= 0 || !dst || !fmt)
        return 0;

    int     size;
    va_list lst;
    maxSize -= 1;

    va_start(lst, fmt);
    size = skp_printf(dst, maxSize, fmt, lst);
    va_end(lst);

    if (size < 0)
        size = maxSize;

    dst[size] = 0;
    return size;
}
