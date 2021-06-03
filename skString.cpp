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
#include "skFileStream.h"
#include "skPlatformHeaders.h"
#include "skStringConverter.h"

const SKsize   skString::npos = -1;
const skString skString::Blank;

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
        ValueType* ptr = new ValueType[nr + 2];
        //skMemset(ptr, 0, nr + 2);

        if (m_data)
        {
            skMemcpy(ptr, m_data, m_size);
            delete[] m_data;
        }

        m_data     = ptr;
        m_capacity = nr + 1;
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
    return !skChar::equals(m_data, rhs.m_data);
}

bool skString::equals(const char* rhs) const
{
    return !skChar::equalsn(m_data, rhs, m_size);
}

bool skString::endsWith(const char* str) const
{
    const SKsize len = skChar::length(str);
    if (len > 0 && len < m_size)
        return skChar::equalsn(m_data + (m_size-len), str, len) == 0;
    return false;
}

bool skString::startsWith(const char* str) const
{
    const SKsize len = skChar::length(str);
    if (len > 0 && len < m_size)
        return skChar::equalsn(m_data, str, len) == 0;
    return false;
}

void skString::alloc(const char* str, SKsize len)
{
    if (!str)
        return;

    if (!len)
        len = skChar::length(str);

    if (len > 0)
    {
        reserve(len);

        if (m_data)
        {
            skChar::copyn(m_data, str, len);
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

void skString::swap(skString& rhs) noexcept
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
    if (nr != npos && pos != npos && pos < m_size)
    {
        if (nr == 0)
            nr = m_size;

        if (nr + pos > m_size)
            nr = m_size - pos;

        skString oth(m_data + pos, nr);
        dest.swap(oth);
    }
}

void skString::split(skArray<skString>& dst, char op) const
{
    const char buf[2] = {op, '\0'};
    split(dst, buf);
}

void skString::split(skArray<skString>& dst, const char* op) const
{
    if (m_size == 0 || !op || !*op)
        return;

    dst.reserve(32);
    skString     sub;
    const SKsize len = strlen(op);

    SKsize j = 0;
    for (SKsize i = 0; i < m_size && i != npos;)
    {
        i = find(op, j);

        if (i == 0)
            j += len;
        else
        {
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
}

skString skString::format(const char* fmt, ...)
{
    skString dst;
    if (fmt)
    {
        va_list lst;

        va_start(lst, fmt);
        const int size = std::vsnprintf(nullptr, 0, fmt, lst);
        va_end(lst);

        if (size > 0)
        {
            dst.resize((SKsize)size);

            va_start(lst, fmt);
            std::vsnprintf(dst.ptr(), (size_t)size + 1, fmt, lst);
            va_end(lst);
        }
    }

    return dst;
}

void skString::format(skString& dst, const char* fmt, ...)
{
    if (fmt)
    {
        va_list lst;

        va_start(lst, fmt);
        const int size = std::vsnprintf(nullptr, 0, fmt, lst);
        va_end(lst);

        if (size > 0)
        {
            dst.resize((SKsize)size);

            va_start(lst, fmt);
            std::vsnprintf(dst.ptr(), (size_t)size + 1, fmt, lst);
            va_end(lst);
        }
    }
}

skString skString::fromFile(const char* path)
{
    skString result;
    fromFile(result, path);
    return result;
}

void skString::fromFile(skString& dest, const char* path)
{
    skFileStream fs;
    fs.open(path, skStream::READ_TEXT);
    if (fs.isOpen())
    {
        const SKsize len = fs.size();
        if (len > 0)
        {
            dest.resize(len + 1);
            const SKsize br = fs.read(dest.ptr(), len);
            if (br <= len)
                dest.ptr()[br] = 0;
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
    SKsize rv = npos;
    if (m_data != nullptr && offs < m_size)
    {
        char* sp  = m_data + offs;
        char* ptr = strstr(sp, ch);
        if (ptr)
            rv = ptr - sp;
    }
    return rv;
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
            rhsLen = skChar::length(rhs);

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
    static const char* HexTable = "0123456789ABCDEF";

    if (m_size == 0)
        return;

    const SKsize oldSize = m_size;

    resize(m_size * 2);

    const char* cp = m_data;
    char*       dp = m_data;

    if (cp == nullptr)
        return;

    SKsize j = m_size;
    for (SKsize i = oldSize - 1; i != npos; --i)
    {
        const int iVal = (int)(unsigned char)cp[i];
        const int dv   = iVal / 16;
        const int rv   = iVal % 16;
        dp[--j]        = HexTable[rv];
        dp[--j]        = HexTable[dv];
    }
    dp[m_size] = 0;
}

void skString::fromHex(void)
{
    skString result;
    result.resize(m_size / 2);

    const char* cp = m_data;
    char*       dp = result.ptr();

    int    rv;
    SKsize j = 0;
    for (SKsize i = 0; i < m_size; i += 2)
    {
        const char c0 = cp[i + 0];
        const char c1 = cp[i + 1];

        int dv = rv = 0;
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

static const SKuint16 BinaryTable[9] = {256, 128, 64, 32, 16, 8, 4, 2, 1};

void skString::toBinary(void)
{
    skString s;
    s.reserve(9 * m_size + 2);

    for (SKsize i = 0; i < m_size; ++i)
    {
        SKuint16 c = (SKuint8)m_data[i];

        if (c < BinaryTable[0])
        {
            char     tempBuf[12]{};
            SKuint16 counter = 0;

            if (i != 0)
                tempBuf[counter++] = 0x20;

            for (unsigned short j : BinaryTable)
            {
                if (c >= j)
                {
                    tempBuf[counter++] = 0x31;
                    c -= j;
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

    skStringArray sa;
    split(sa, " ");

    skStringArray::Iterator it = sa.iterator();
    while (it.hasMoreElements())
    {
        const skString& cs = it.getNext();
        SKuint16        c  = 0;
        for (SKuint16 j = 8; j > 0; j--)
        {
            if (j < cs.size() && cs[j] == 0x31)
                c += BinaryTable[j];
        }
        s.append((SKbyte)c);
    }
    this->operator=(s);
}

int skSprintf(char* dst, int maxSize, const char* fmt, ...)
{
    if (maxSize <= 0 || !dst || !fmt)
        return 0;

    va_list lst;
    maxSize -= 1;
    va_start(lst, fmt);
    int size = skp_printf(dst, maxSize, fmt, lst);
    va_end(lst);

    if (size < 0)
        size = maxSize;

    dst[size] = 0;
    return size;
}
