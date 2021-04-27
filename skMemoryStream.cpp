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
#include "skMemoryStream.h"
#include "skFileStream.h"
#include "skMinMax.h"
#include "skPlatformHeaders.h"

skMemoryStream::skMemoryStream(int mode) :
    m_data(nullptr),
    m_pos(0),
    m_size(0),
    m_capacity(0),
    m_initialOffset(0),
    m_isExternal(false)
{
    m_mode = mode;
}

skMemoryStream::~skMemoryStream()
{
    clear();
}

void skMemoryStream::open(const char* path, int mode)
{
    skFileStream fs;
    fs.open(path, READ);

    if (fs.isOpen())
        open(fs, mode);
}

void skMemoryStream::open(const skStream& other, int mode)
{
    if (other.isOpen())
    {
        m_initialOffset = 0;
        m_isExternal    = false;
        m_size          = 0;
        m_capacity      = 0;
        m_pos           = 0;
        m_mode          = mode;

        // FIXME: this should dump the contents
        // of one stream to another.
        // other.write( *this );
    }
}

void skMemoryStream::open(const void* data, SKsize sizeInBytes, int mode)
{
    if (data && sizeInBytes > 0 && sizeInBytes != SK_NPOS)
    {
        m_pos           = 0;
        m_capacity      = 0;
        m_isExternal    = false;
        m_initialOffset = 0;

        m_mode = mode;
        m_size = sizeInBytes;

        reserve(m_size);
        skMemcpy(m_data, data, m_size);
    }
}

void skMemoryStream::open(const void* data, SKsize sizeInBytes, SKsize posInData, bool externalData)
{
    if (data && sizeInBytes > 0 && sizeInBytes != SK_NPOS)
    {
        if (m_data)
            clear();

        m_mode = READ;
        m_size = sizeInBytes;
        m_pos  = skMin(posInData, m_size);
        m_data = (SKbyte*)data;

        m_capacity      = sizeInBytes;
        m_isExternal    = true;
        m_initialOffset = posInData;
    }
}

void skMemoryStream::clear(void)
{
    if (!m_isExternal)
    {
        if (m_data != nullptr)
        {
            delete[] m_data;
            m_data = nullptr;
        }
        m_size = m_pos = 0;
        m_capacity     = 0;
    }
    else
        m_pos = m_initialOffset;
}

void skMemoryStream::close(void)
{
    if (!m_isExternal)
    {
        m_size = m_pos = 0;
        if (m_data)
            m_data[0] = 0;
    }
}

bool skMemoryStream::seek(SKint64 offset, SKsize dir)
{
    if (!m_data || m_size == 0)
        return false;

    if (dir == SEEK_SET)
        m_pos = skClamp<SKsize>((SKsize)offset, 0, m_size);
    else if (dir == SEEK_CUR)
        m_pos = skClamp<SKsize>(m_pos + (SKsize)offset, 0, m_size);
    else if (dir == SEEK_END)
        m_pos = m_size;

    return true;
}

SKsize skMemoryStream::read(void* dest, SKsize nr) const
{
    if (!canRead() || !dest || !isOpen())
        return SK_NPOS;

    if (m_pos < m_size)
    {
        // trim off the excess.
        if (m_size - m_pos < nr)
            nr = m_size - m_pos;

        char* cp = (char*)dest;
        char* op = (char*)&m_data[m_pos];
        skMemcpy(cp, op, nr);

        m_pos += nr;
        return nr;
    }
    return 0;
}

SKsize skMemoryStream::write(const void* src, SKsize nr)
{
    if (!canWrite() || !src)
        return SK_NPOS;

    if (m_data == nullptr)
    {
        m_pos = 0;
        reserve(m_pos + nr);
    }
    else if (m_pos + nr > m_capacity)
        reserve(m_pos + (nr > 1024 ? nr : nr + 1024));

    if (m_data != nullptr)
    {
        char* cp = &m_data[m_pos];
        skMemcpy(cp, (char*)src, nr);
        m_pos += nr;
        m_size += nr;
    }
    return nr;
}

void skMemoryStream::reserve(SKsize nr)
{
    if (!m_isExternal)
    {
        if (m_capacity < nr)
        {
            char* buf = new char[nr + 1];
            if (m_data != nullptr)
            {
                skMemcpy(buf, m_data, m_size);
                delete[] m_data;
            }
            m_data         = buf;
            m_data[m_size] = 0;
            m_capacity     = nr;
        }
    }
}

bool skMemoryStream::isValidStream(int offs) const
{
    return m_data && m_pos + offs < m_size;
}

void skMemoryStream::readInt8(SKuint8& out) const
{
    if (m_mode != WRITE && isValidStream(2))
        out = m_data[m_pos++];
}

void skMemoryStream::readInt16(SKuint16& out) const
{
    if (m_mode != WRITE && isValidStream(2))
    {
        out = *(SKuint16*)addressAtPosition();
        m_pos += 2;
    }
}

void skMemoryStream::readInt32(SKuint32& out) const
{
    if (m_mode != WRITE && isValidStream(4))
    {
        out = *(SKuint32*)addressAtPosition();
        m_pos += 4;
    }
}

void skMemoryStream::readInt64(SKuint64& out) const
{
    if (m_mode != WRITE && isValidStream(8))
    {
        out = *(SKuint64*)addressAtPosition();
        m_pos += 8;
    }
}

void skMemoryStream::readVaryingInt(SKsize& out) const
{
    if (m_mode != WRITE && isValidStream(sizeof(SKsize)))
    {
        out = *(SKsize*)addressAtPosition();
        m_pos += sizeof(SKsize);
    }
}

SKsize skMemoryStream::getVaryingInt() const
{
    SKsize out = SK_NPOS;
    if (m_mode != WRITE && isValidStream(sizeof(SKsize)))
        out = (SKsize)addressAtPosition();
    return out;
}

SKsize skMemoryStream::seekString() const
{
    SKsize bs = 0;
    if (m_mode != WRITE && m_data)
    {
        const SKsize os = m_pos;
        while (m_pos < m_size && m_data[m_pos])
            m_pos++;
        if (!m_data[m_pos])
            ++m_pos;
        bs = m_pos - os;
    }
    if (m_pos > m_size)
        return SK_NPOS;
    return bs;
}

void skMemoryStream::readString(SKbyte* dest, SKuint32 destLen) const
{
    if (m_mode != WRITE && m_data && destLen > 0)
    {
        const SKsize oldLoc = m_pos;
        SKsize       i = oldLoc, j = 0;

        while (i < m_size && j < destLen && m_data[i])
            dest[j++] = m_data[i++];
        ++i;
        dest[j < destLen ? j : destLen - 1] = 0;
        m_pos += i - oldLoc;
    }
}
