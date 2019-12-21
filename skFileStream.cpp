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
#include "skMinMax.h"
#include "skPlatformHeaders.h"



skFileStream::skFileStream() :
    m_handle(0),
    m_mode(NO_INPUT)
{
}

skFileStream::~skFileStream()
{
    skFileStream::close();
}

void skFileStream::flush(void)
{
    if (m_mode == WRITE && m_handle)
        fflush(static_cast<FILE*>(m_handle));
}

void skFileStream::open(const char* path, Mode mode)
{

    if (mode == NO_INPUT || !path)
        return;

    if (m_mode != NO_INPUT) 
        close();

    m_handle = fopen(path, mode == READ ? "rb" : mode == WRITE_TEXT ? "w" : "wb");
    m_mode   = m_handle != 0 ? mode : NO_INPUT;
}


void skFileStream::close(void)
{

    if (m_handle)
        fclose(static_cast<FILE*>(m_handle));

    m_handle = 0;
    m_mode   = NO_INPUT;
}

bool skFileStream::eof(void) const
{

    if (!isOpen())
        return true;
    return feof(static_cast<FILE*>(m_handle));
}

SKsize skFileStream::read(void* dst, SKsize nr) const
{
    
    if (m_mode == WRITE || dst == 0 || !isOpen())
        return 0;

	return fread(dst, 1, nr, static_cast<FILE*>(m_handle));
}


bool skFileStream::seek(SKint64 offs, SKsize dir)
{
    if (!isOpen() || offs == npos)
        return false;

    long way;
    if (dir == SEEK_END)
        way = SEEK_END;
    else if (dir == SEEK_CUR)
        way = SEEK_CUR;
    else
        way = SEEK_SET;

    if (fseek(static_cast<FILE*>(m_handle), (long)offs, way) == 0)
        return true;
    return false;
}

SKsize skFileStream::write(const void* src, SKsize nr)
{

    if (m_mode == READ || !isOpen() || !src || nr <= 0)
        return 0;
    return fwrite(src, 1, nr, static_cast<FILE*>(m_handle));
}

SKsize skFileStream::position(void) const
{
    return isOpen() ? (SKsize) ftell(static_cast<FILE*>(m_handle)) : npos;
}

SKsize skFileStream::size(void) const
{
    SKsize size = npos;
    if (isOpen())
    {
        FILE* fp = static_cast<FILE*>(m_handle);

        long loc = ftell(fp);
        fseek(fp, 0L, SEEK_END);

        size = (SKsize)ftell(fp);

        fseek(fp, loc, SEEK_SET);
    }
    return size;
}

SKsize skStream::writef(const char* fmt, ...)
{
    char    buf[1025];
    va_list lst;
    va_start(lst, fmt);
    int size = skp_printf(buf, 1024, fmt, lst);
    va_end(lst);

    if (size < 0)
        size = 1024;

    buf[size] = 0;
    return write(buf, size);
}