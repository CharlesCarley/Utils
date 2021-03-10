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
#include "Utils/skStringBuilder.h"
#include "Utils/skPlatformHeaders.h"

void skStringBuilder::clear()
{
    delete[] m_buffer;
    m_buffer = nullptr;

    m_size     = 0;
    m_capacity = 0;
}

void skStringBuilder::reserve(SKsize len)
{
    const SKsize newCap = getNextCapacity(len);

    SK_ASSERT(newCap > m_size);

    SKbyte* newMem = new SKbyte[newCap + 1];
    if (newMem != nullptr)
    {
        if (m_size > 0)
            skMemcpy(newMem, m_buffer, m_size);

        delete[] m_buffer;
        m_buffer   = newMem;
        m_capacity = newCap;
    }
}

SKsize skStringBuilder::getNextCapacity(SKsize len) const
{
    SKsize next;
    if (m_strategy == ALLOC_MUL2)
        next = m_capacity * 2;
    else
        next = m_capacity + m_nByteBlock;
    next += len;
    return next;
}

SKsize skStringBuilder::writeToBuffer(const void* source, SKsize len)
{
    const SKsize nextPosition = m_size + len;

    if (nextPosition > m_capacity)
    {
        reserve(nextPosition);
        SK_ASSERT(m_capacity > nextPosition);
    }

    if (m_buffer != nullptr)
    {
        memcpy(&m_buffer[m_size], source, len);
        m_size = nextPosition;
        return len;
    }
    return SK_NPOS;
}

void skStringBuilder::write(const skStringBuilder& oth)
{
    if (oth.size() > 0)
        writeToBuffer(oth.m_buffer, oth.size());
}

void skStringBuilder::write(const skString& str)
{
    if (!str.empty())
        writeToBuffer(str.c_str(), str.size());
}

void skStringBuilder::write(SKbyte ch)
{
    writeToBuffer(&ch, 1);
}

void skStringBuilder::write(const SKbyte* ptr)
{
    if (ptr && *ptr)
        writeToBuffer(ptr, skChar::length(ptr));
}

void skStringBuilder::write(const SKbyte* ptr, SKsize len)
{
    if (ptr && *ptr)
        writeToBuffer(ptr, len);
}

void skStringBuilder::write(SKint16 i16)
{
    m_dest.resize(0);
    skChar::toString(m_dest, i16);
    writeToBuffer(m_dest.c_str(), m_dest.size());
}

void skStringBuilder::write(SKint32 i32)
{
    m_dest.resize(0);
    skChar::toString(m_dest, i32);
    writeToBuffer(m_dest.c_str(), m_dest.size());
}

void skStringBuilder::write(SKint64 i64)
{
    m_dest.resize(0);
    skChar::toString(m_dest, i64);
    writeToBuffer(m_dest.c_str(), m_dest.size());
}

void skStringBuilder::write(SKuint16 u16)
{
    m_dest.resize(0);
    skChar::toString(m_dest, u16);
    writeToBuffer(m_dest.c_str(), m_dest.size());
}

void skStringBuilder::write(SKuint32 u32)
{
    m_dest.resize(0);
    skChar::toString(m_dest, u32);
    writeToBuffer(m_dest.c_str(), m_dest.size());
}

void skStringBuilder::write(SKuint64 u64)
{
    m_dest.resize(0);
    skChar::toString(m_dest, u64);
    writeToBuffer(m_dest.c_str(), m_dest.size());
}

void skStringBuilder::write(double d)
{
    m_dest.resize(0);
    skChar::toString(m_dest, d);
    writeToBuffer(m_dest.c_str(), m_dest.size());
}

void skStringBuilder::write(float f)
{
    m_dest.resize(0);
    skChar::toString(m_dest, f);
    writeToBuffer(m_dest.c_str(), m_dest.size());
}
