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



skMemoryStream::skMemoryStream() :
    m_data(0),
    m_pos(0),
    m_size(0),
    m_capacity(0),
    m_initialOffset(0),
    m_mode(0),
    m_isExternal(false)
{
}



skMemoryStream::~skMemoryStream()
{
    clear();
}

void skMemoryStream::open(skStream::Mode mode)
{
    m_initialOffset = 0;
    m_isExternal    = false;
    m_size          = 0;
    m_capacity      = 0;
    m_pos           = 0;
    m_mode          = mode;
}


void skMemoryStream::open(const char* path, skStream::Mode mode)
{
    skFileStream fs;
    fs.open(path, skStream::READ);

    if (fs.isOpen())
        open(fs, mode);
}

void skMemoryStream::open(const skStream& cpy, skStream::Mode mode)
{
    if (cpy.isOpen())
    {
        m_initialOffset = 0;
        m_isExternal    = false;
        m_size          = 0;
        m_capacity      = 0;
        m_pos           = 0;
        m_mode          = mode;

        // FIXME: this should dump the contents
        // of one stream to another.

        // cpy.write( *this );
    }
}

void skMemoryStream::open(const void* data, SKsize sizeInBytes, skStream::Mode mode)
{
    if (data && sizeInBytes > 0 && sizeInBytes != npos)
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
    if (data && sizeInBytes > 0 && sizeInBytes != npos)
    {
        if (m_data)
            clear();
        m_mode = skStream::READ;
        m_size = sizeInBytes;
        m_pos  = skMin(posInData, m_size);

        m_data     = (SKbyte*)data;
        m_capacity = sizeInBytes;

        m_isExternal    = true;
        m_initialOffset = posInData;
    }
}


void skMemoryStream::clear(void)
{
    if (!m_isExternal)
    {
        if (m_data != 0)
        {
            delete[] m_data;
            m_data = 0;
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
    else
        printf("Invalid direction: skMemoryStream::seek, position was not changed\n");
    return true;
}

SKsize skMemoryStream::read(void* dest, SKsize nr) const
{
    if (m_mode == skStream::WRITE)
        return npos;

    if (m_pos > m_size)
        return 0;

    if (!dest || !m_data)
        return 0;

    if ((m_size - m_pos) < nr)
        nr = m_size - m_pos;

    char* cp = (char*)dest;
    char* op = (char*)&m_data[m_pos];
    skMemcpy(cp, op, nr);
    m_pos += nr;
    return nr;
}

SKsize skMemoryStream::write(const void* src, SKsize nr)
{
    if (m_mode == skStream::READ || !src)
        return npos;

    if (m_pos > m_size)
        return 0;

    if (m_data == 0)
        reserve(m_pos + (nr));
    else if (m_pos + nr > m_capacity)
        reserve(m_pos + (nr > 1024 ? nr : nr + 1024));

    char* cp = &m_data[m_pos];
    skMemcpy(cp, (char*)src, nr);

    m_pos += nr;
    m_size += nr;
    return nr;
}

void skMemoryStream::reserve(SKsize nr)
{
    if (!m_isExternal)
    {
        if (m_capacity < nr)
        {
            char* buf = new char[nr + 1];
            if (m_data != 0)
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
    if (m_mode != skStream::WRITE && isValidStream(2))
        out = m_data[m_pos++];
}

void skMemoryStream::readInt16(SKuint16& out) const
{
    if (m_mode != skStream::WRITE && isValidStream(2))
    {
        out = (*((SKuint16*)addressAtPosition()));
        m_pos += 2;
    }
}

void skMemoryStream::readInt32(SKuint32& out) const
{
    if (m_mode != skStream::WRITE && isValidStream(4))
    {
        out = (*((SKuint32*)addressAtPosition()));
        m_pos += 4;
    }
}

void skMemoryStream::readInt64(SKuint64& out) const
{
    if (m_mode != skStream::WRITE && isValidStream(8))
    {
        out = (*((SKuint64*)addressAtPosition()));
        m_pos += 8;
    }
}

void skMemoryStream::readVaryingInt(SKsize& out) const
{
    if (m_mode != skStream::WRITE && isValidStream(sizeof(SKsize)))
    {
        out = (*((SKsize*)addressAtPosition()));
        m_pos += sizeof(SKsize);
    }
}


SKsize skMemoryStream::getVaryingInt() const
{
    SKsize out = SK_NPOS;
    if (m_mode != skStream::WRITE && isValidStream(sizeof(SKsize)))
        out = ((SKsize)addressAtPosition());
    return out;
}


SKsize skMemoryStream::seekString() const
{
    SKsize bs = 0;
    if (m_mode != skStream::WRITE && m_data)
    {
        SKsize os = m_pos;
        while (m_pos < m_size && m_data[m_pos])
            m_pos++;
        if (!m_data[m_pos])
            ++m_pos;
        bs = m_pos - os;
    }
    return bs;
}


void skMemoryStream::readString(SKbyte* dest, SKuint32 destLen) const
{
    if (m_mode != skStream::WRITE && m_data && destLen > 0)
    {
        SKsize oldLoc = m_pos;
        SKsize i = oldLoc, j = 0;

        while (i < m_size && j < destLen && m_data[i])
            dest[j++] = m_data[i++];
        ++i;
        dest[j < destLen ? j : destLen - 1] = 0;
        m_pos += (i - oldLoc);
    }
}
