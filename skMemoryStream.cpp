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
    if (buffer && size > 0 && size != SK_NPOS)
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

SKsize skMemoryStream::seek(SKint32 off, SKint32 way)
{
    if (way == SEEK_SET)
        m_pos = skClamp<SKsize>(off, 0, m_size);
    else if (way == SEEK_CUR)
        m_pos = skClamp<SKsize>(m_pos + off, 0, m_size);
    else if (way == SEEK_END)
        m_pos = m_size;

    return m_pos;
}

SKsize skMemoryStream::read(void* dest, SKsize nr) const
{
    if (m_mode == skStream::WRITE)
        return SK_NPOS;

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
        return SK_NPOS;

    if (m_pos > m_size)
        return 0;

    if (m_buffer == 0)
        reserve(m_pos + (nr));
    else if (m_pos + nr > m_capacity)
        reserve(m_pos + (nr > 65535 ? nr : nr + 65535));

    char* cp = &m_buffer[m_pos];
    skMemcpy(cp, (char*)src, nr);

    m_pos += nr;
    m_size += nr;
    return nr;
}

SKsize skMemoryStream::writef(const char* fmt, ...)
{
    static char tmp[1024];

    va_list lst;
    va_start(lst, fmt);
    int size = skp_printf(tmp, 1024, fmt, lst);
    va_end(lst);

    if (size > 0)
    {
        tmp[size] = 0;
        return write(tmp, size);
    }

    return SK_NPOS;
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
