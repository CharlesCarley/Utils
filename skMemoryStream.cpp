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
    m_buffer(0),
    m_pos(0),
    m_size(0),
    m_capacity(0),
    m_mode(0)
{
}



skMemoryStream::~skMemoryStream()
{
    if (m_buffer != 0)
    {
        delete[] m_buffer;
        m_buffer = 0;
    }

    m_size = m_pos = 0;
    m_capacity     = 0;
}

void skMemoryStream::flush(void)
{
}

void skMemoryStream::open(skStream::Mode mode)
{
    m_mode = mode;
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
        // cpy.write( *this );
        m_mode = mode;
    }
}



void skMemoryStream::open(const void* buffer, SKsize size, skStream::Mode mode)
{
    if (buffer && size > 0 && size != npos)
    {
        m_mode = mode;
        m_size = size;
        m_pos  = 0;

        reserve(m_size);
        skMemcpy(m_buffer, buffer, m_size);
    }
}

void skMemoryStream::clear(void)
{
    // Keeps reserved memory for reuse
    m_size = m_pos = 0;
    if (m_buffer)
        m_buffer[0] = 0;
}


void skMemoryStream::close(void)
{
    m_size = m_pos = 0;
    if (m_buffer)
        m_buffer[0] = 0;
}

void skMemoryStream::seek(SKint64 offset, SKsize dir)
{
    if (dir == SEEK_SET)
        m_pos = skClamp<SKsize>((SKsize)offset, 0, m_size);
    else if (dir == SEEK_CUR)
        m_pos = skClamp<SKsize>(m_pos + (SKsize)offset, 0, m_size);
    else if (dir == SEEK_END)
        m_pos = m_size;
    else
        printf("Invalid direction: skMemoryStream::seek, position was not changed\n");
}



SKsize skMemoryStream::read(void* dest, SKsize nr) const
{
    if (m_mode == skStream::WRITE)
        return npos;

    if (m_pos > m_size)
        return 0;

    if (!dest || !m_buffer)
        return 0;

    if ((m_size - m_pos) < nr)
        nr = m_size - m_pos;

    char* cp = (char*)dest;
    char* op = (char*)&m_buffer[m_pos];
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

    if (m_buffer == 0)
        reserve(m_pos + (nr));
    else if (m_pos + nr > m_capacity)
        reserve(m_pos + (nr > 1024 ? nr : nr + 1024));

    char* cp = &m_buffer[m_pos];
    skMemcpy(cp, (char*)src, nr);

    m_pos += nr;
    m_size += nr;
    return nr;
}


void skMemoryStream::reserve(SKsize nr)
{
    if (m_capacity < nr)
    {
        char* buf = new char[nr + 1];
        if (m_buffer != 0)
        {
            skMemcpy(buf, m_buffer, m_size);
            delete[] m_buffer;
        }

        m_buffer         = buf;
        m_buffer[m_size] = 0;
        m_capacity       = nr;
    }
}
