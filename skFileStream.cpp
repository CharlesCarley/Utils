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
#include "skFileStream.h"
#include "skPlatformHeaders.h"

skFileStream::skFileStream() : m_handle(0), m_pos(SK_NPOS), m_size(SK_NPOS), m_mode(NO_INPUT)
{
}

skFileStream::~skFileStream()
{
    close();
}

void skFileStream::flush(void)
{
    if (m_mode == READ)
    {
        if (m_handle)
            fflush(static_cast<FILE*>(m_handle));
    }
}

void skFileStream::open(const char* path, Mode mode)
{
    if (mode == NO_INPUT || !path)
        return;

    if (m_mode != NO_INPUT)
        close();

    m_handle = fopen(path, mode == READ ? "rb" : mode == WRITE_TEXT ? "w" : "wb");
    m_mode = m_handle != 0 ? mode : NO_INPUT;

    if (m_mode != NO_INPUT && m_handle != 0)
        m_pos = 0;

    if (m_mode == WRITE)
        m_size = 0;
}

void skFileStream::close(void)
{
    if (m_handle)
        fclose(static_cast<FILE*>(m_handle));

    m_handle = 0;
    m_mode = NO_INPUT;
    m_size = m_pos = SK_NPOS;
}

bool skFileStream::isOpen(void) const
{
    return m_mode != NO_INPUT && m_handle != 0;
}

bool skFileStream::eof(void) const
{
    return !isOpen() || feof(static_cast<FILE*>(m_handle));
}

SKsize skFileStream::read(void* dst, SKsize nr) const
{
    if (m_mode == WRITE || !isOpen() || !dst || nr <= 0)
        return 0;

    SKsize br = fread(dst, 1, nr, static_cast<FILE*>(m_handle));
    m_pos += br;
    return br;
}

SKsize skFileStream::write(const void* src, SKsize nr)
{
    if (m_mode == READ || !isOpen() || !src || nr <= 0)
        return 0;

    SKsize bw = fwrite(src, 1, nr, static_cast<FILE*>(m_handle));
    m_pos += bw;
    m_size += bw;
    return bw;
}

SKsize skFileStream::position(void) const
{
    return m_pos;
}

SKsize skFileStream::size(void) const
{
    if (m_size == SK_NPOS)
    {
        long  loc;
        FILE* fp = static_cast<FILE*>(m_handle);

        // Grab the current position indicator
        loc = ftell(fp);
        fseek(fp, 0L, SEEK_END);

        m_size = (SKsize)ftell(fp);

        // Restore position indicator
        fseek(fp, loc, SEEK_SET);
    }
    return m_size;
}
