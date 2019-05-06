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
#include "skFixedString.h"
#include "skPlatformHeaders.h"


namespace skStringUtils
{

    bool toBool(const char* in)
    {
        return (equals(in, "true") == 0 ||
                equals(in, "yes") == 0 ||
                equals(in, "1") == 0);
    }


    long toLong(const char* in)
    {
        return ::atol(in);
    }

    int toInt(const char* in)          { return ::atoi(in); }
    float toFloat(const char* in)      { return (float)::atof(in); }
    double toDouble(const char* in)    { return (double)::atof(in); }

#ifndef SK_USE_STD_STRING_FUNCS



    SKsize length(const char* in)
    {
        if (!in)
            return 0;

        SKsize i = 0;
        while (in[i] != 0) ++i;

        return i;
    }



    SKsize copyn(char* out, const char* in, SKsize max)
    {
        if (!in || !out)
            return 0;
        SKsize i = 0;
        while (in[i] != 0 && i < max)
        {
            out[i] = in[i];
            ++i;
        };
        out[i] = 0;
        return i;
    }


    SKsize copy(char* out, const char* in)
    {
        return copyn(out, in, SK_NPOS);
    }



    SKsize equalsn(const char* a, const char* b, SKsize max)
    {
        if (!a || !b) return 1;

        if (*a != *b) return 1;

        SKsize i = 0;
        bool result = true;

        while (result && a[i] != 0 && b[i] != 0 && i < max)
        {
            result = a[i] == b[i];
            ++i;
        }

        return result ? 0 : 1;
    }



    SKsize equals(const char* a, const char* b)
    {
        return equalsn(a, b, SK_NPOS);
    }
#else


    SKsize length(const char* in)
    {
        if (!in) return 0;

        return (SKsize)::strlen(in);
    }


    SKsize copyn(char* out, const char* in, SKsize max)
    {
        if (!in || !out)
            return 0;
        return (SKsize)(size_t)::strncpy(out, in, (size_t)max);
    }



    SKsize copy(char* out, const char* in)
    {
        return (SKsize)(size_t)strcpy(out, in);
    }


    SKsize equalsn(const char* a, const char* b, SKsize max)
    {
        if (!a || !b) return 1;

        if (*a != *b) return 1;

        return (SKsize)(size_t)strncmp(a, b, (size_t)max);

    }


    SKsize equals(const char* a, const char* b)
    {
        if (!a || !b) return 1;

        if (*a != *b) return 1;

        return (SKsize)strcmp(a, b);
    }

#endif

    const  SKuint8  HexTable[16] = {
        '0', '1', '2', '3',
        '4', '5', '6', '7',
        '8', '9', 'A', 'B',
        'C', 'D', 'E', 'F',
    };

    const  SKuint16 BinaryTable[9] = {
        256, 128, 64, 32, 16, 8, 4, 2, 1
    };

}



void skString::clear(void)
{
    if (m_data)
    {
        delete[]m_data;
        m_data = 0;
    }
    m_size = 0;
    m_capacity = 0;
}


void skString::reserve(SKsize nr)
{
    if (m_capacity < nr)
    {
        ValueType* ptr = new ValueType[nr + 1];

        if (m_data)
        {
            skMemcpy(ptr, m_data, m_size);
            delete[]m_data;
        }

        m_data = ptr;
        m_capacity = nr;
        m_data[m_size] = 0;
    }
}

void skString::resize(SKsize nr)
{
    if (nr < m_size)
        m_data[nr] = 0;
    else if (nr > m_size)
        reserve(nr);
    m_size = nr;
}

void skString::alloc(const char* str, SKsize len)
{
    if (!str) return;

    m_size = len;
    if (!m_size)
        m_size = skStringUtils::length(str);

    reserve(m_size);
    if (m_data)
    {
        skStringUtils::copyn(m_data, str, m_size);
        m_data[m_size] = 0;
    }
}




void skString::alloc(const char p, SKsize len)
{
    m_size = len;
    reserve(m_size);

    if (m_data)
    {

        while (len != SK_NPOS)
            m_data[len--] = p;
        m_data[m_size] = 0;
    }
}


void skString::alloc(SKsize len, const void* p)
{
    if (len != SK_NPOS && p != 0)
    {
        resize(len);
        skMemcpy(m_data, p, m_size);
        m_data[m_size] = 0;
    }

}


skString& skString::operator = (const skString& rhs)
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
    if (nr == SK_NPOS || pos == SK_NPOS)
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
    skString sub;
    SKsize i, j = 0, len = strlen(op);

    for (i = 0; i < m_size && i != SK_NPOS;)
    {
        i = find(op, j);

        if (i == 0)
        {
            j += len;
            continue;
        }

        if (i != SK_NPOS)
        {
            substr(sub, j, i);
            if (!sub.empty())
                dst.push_back(sub);

            j += i + len;

        }
        else
        {
            if (j < m_size && j != SK_NPOS)
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
    static char buf[1025];

    va_list lst;
    va_start(lst, fmt);
    int size = skp_printf(buf, 1024, fmt, lst);
    va_end(lst);

    if (size < 0)
        size = 1024;

    buf[size] = 0;
    return skString(buf, size);
}

SKsize skString::find(char ch) const
{
    if (m_data != 0)
    {
        char* ptr = strchr(m_data, (int)ch);

        if (ptr)
        {
            int diff = ptr - m_data;
            if (diff > 0) return (SKsize)diff;
        }
    }

    return SK_NPOS;
}

SKsize skString::find(const char* ch,  SKsize pos) const
{
    if (m_data != 0)
    {
        if (pos >= m_size) return SK_NPOS;


        char* sp = m_data + pos;
        char* ptr = strstr(sp, ch);

        if (ptr)
        {
            int diff = ptr - sp;

            if (diff >= 0) return (SKsize)diff;
        }
    }

    return SK_NPOS;
}

skString& skString::append(char ch)
{

    reserve(m_size + 1);

    m_data[m_size] = ch;
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

SKsize skString::copy(char* arr, SKsize nr, SKsize pos)
{
    if (!arr || !m_data || nr + pos > m_size)
        return 0;


    char* src = m_data + pos;

    SKsize i;

    for (i = 0; i < nr; ++i) arr[i] = src[i];

    return i;
}

skString& skString::erase(SKsize pos, SKsize nr)
{
    if (!m_data) return *this;

    if (nr == 0 && pos < m_size)
    {
        m_size =  pos;
        m_data[m_size] = 0;
        return *this;
    }

    if (nr >= m_size && pos == 0)
    {
        m_size =  0;
        m_data[m_size] = 0;
        return *this;
    }


    SKsize left = pos;
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
    // There are two characters per one in the hex string 
    //          Old                             New
    //  -------------------------- -------------------------- 
    // |                        <-|                       <--|
    //  -------------------------- -------------------------- 
    //  work in reverse, and by the time it gets to the first
    //  old character, it will be in ival and nothing
    //  will be overridden 

    SKsize old_size = m_size;
    resize(m_size * 2);

    const char* cp = m_data;
    char* dp = ptr();

    int ival, dv, rv;

    SKsize i, j=m_size;
    for (i = (old_size -1); i != SK_NPOS; --i)
    {
        ival = (int)(unsigned char)cp[i];
        dv = ival / 16;
        rv = ival % 16;
        if (ival < 256)
        {
            (dp[--j]) = skStringUtils::HexTable[rv];
            (dp[--j]) = skStringUtils::HexTable[dv];
        }
    }

    dp[m_size] = 0;
}


void skString::fromHex(void)
{
    skString result;
    result.resize(m_size / 2);
    const char* cp = m_data;
    char* dp = result.ptr();

    int dv, rv;
    SKsize i, j = 0;
    for (i = 0; i < m_size; i += 2)
    {
        char c0 = cp[i + 0];
        char c1 = cp[i + 1];

        dv = rv = 0;
        if (c0 >= '0' && c0 <= '9')
            dv = ((int)c0) - '0';
        else if (c0 >= 'A' && c0 <= 'Z')
            dv = 10 + ((int)c0) - 'A';
        if (c1 >= '0' && c1 <= '9')
            rv = ((int)c1) - '0';
        else if (c1 >= 'A' && c1 <= 'Z')
            rv = 10 + ((int)c1) - 'A';
        dv = 16 * dv + rv;
        dp[j++] = (char)dv;
        dp[j] = 0;
    }

    swap(result);
}



void skString::toBinary(void)
{
    
    SKsize i;
    skString s;
    SKuint16 c, j, counter;
    bool splitHiLo = false;
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

                if (splitHiLo && j == 3)
                    tempBuf[counter++] = 0x20;
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

        c = 0;
        for (j = 9; j > 0; j--)
        {
            if (j < cs.size() && cs[j] == 0x31)
                c += skStringUtils::BinaryTable[j];
        }
        s.append((SKbyte)c);
    }
    this->operator=(s);
}

void skString::encrypt(SKbyte* LB, int b1, SKuint16* UB, int b2)
{
    SKsize i, j = 0, r;
    double a, b;
    SKuint16* sh;
    for (i = 0; i < m_size; ++i)
        m_data[i] = (m_data[i] + ((i % 3 ? -1 : 1) * LB[i % b1])) % 256;

    r = m_size % 2;
    sh = new SKuint16[m_size + r + 1];
    sh[m_size + r] = 0;

    for (i = 0; i < m_size; i += 2)
    {
        a = (double)(SKuint8)m_data[i + 0];
        b = (double)(SKuint8)m_data[i + 1];
        sh[j++] = (SKuint16)((b + a / 256.0) * 256.0);
    }

    for (i = 0; i < j; i++)
        sh[i] = ((SKuint16)sh[i]) + UB[i % b2] % 65536;

    alloc((j * 2) + r, sh);
    delete[]sh;
}


void skString::decrypt(SKbyte* LB, int b1, SKuint16* UB, int b2)
{
    SKsize i, j = 0, r;
    double a, b;
    char aa, bb;
    SKuint16 s;
    skString dest;
    dest.resize(m_size);

    SKuint16* sp = (SKuint16*)m_data;
    SKbyte* dp = (SKbyte*)dest.ptr();
    r = m_size % 2;

    for (i = 0; i < m_size / 2; ++i)
    {
        s = sp[i] - UB[i % b2] % 65536;
        a = (double)s / 256.0;
        b = (a - (int)a) * 256.0;
        a = (int)a;
        aa = (SKbyte)(int)a;
        bb = (SKbyte)(int)b;

        dp[j++] = bb;
        dp[j++] = aa;
    }

    for (i = 0; i < j; ++i)
        dp[i] = (dp[i] + ((i % 3 ? 1 : -1) * LB[i % b1])) % 256;

    alloc(j - r, dp);
}

void skString::encrypt(void)
{
    static SKuint16 UB[] =
    { 13673, 32696, 27676, 5443, 22638 };
    static SKbyte LB[] =
    { 98, 83, 49, 81, 126 };

    encrypt(
        LB,
        sizeof(LB) / sizeof(LB[0]),
        UB,
        sizeof(UB) / sizeof(UB[0])
    );
}

void skString::decrypt(void)
{
    static SKuint16 UB[] =
    { 13673, 32696, 27676, 5443, 22638 };
    static SKbyte LB[] =
    { 98, 83, 49, 81, 126 };

    decrypt(
        LB,
        sizeof(LB) / sizeof(LB[0]),
        UB,
        sizeof(UB) / sizeof(UB[0])
    );
}

void skString::encrypt(const char* password)
{
    static SKuint16 UB[] =
    { 13673, 32696, 27676, 5443, 22638 };

    encrypt((SKbyte*)password,
            skStringUtils::length(password),
            UB,
            sizeof(UB) / sizeof(UB[0])
           );
}

void skString::decrypt(const char* password)
{
    static SKuint16 UB[] =
    { 13673, 32696, 27676, 5443, 22638 };

    decrypt((SKbyte*)password,
            skStringUtils::length(password),
            UB,
            sizeof(UB) / sizeof(UB[0])
           );
}

int skSprintf(char* dst,  int max_size, const char* fmt, ...)
{
    int size;
    va_list lst;

    if (max_size <= 0 || !dst || !fmt)
        return 0;

    max_size -= 1;

    va_start(lst, fmt);
    size = skp_printf(dst, max_size, fmt, lst);
    va_end(lst);


    if (size < 0)
        size = max_size;

    dst[size] = 0;
    return size;
}

