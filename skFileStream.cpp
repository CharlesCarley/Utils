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
#include <cstdio>
#include <cstdarg>
#include "skFileStream.h"
#include "skMinMax.h"
#include "skPlatformHeaders.h"



skFileStream::skFileStream() :
    m_handle(0)
{
}

skFileStream::skFileStream(const char* path, int mode) :
    m_handle(0)
{
    open(path, mode);
}


skFileStream::~skFileStream()
{
    skFileStream::close();
}

void skFileStream::open(const char* path, int mode)
{
    if (path)
    {
        if (isOpen())
            close();
 
        m_mode = mode;
        if (m_mode == READ)
            m_handle = fopen(path, "rb");
        else if (mode == WRITE)
            m_handle = fopen(path, "wb");
        else if (m_mode == READ_TEXT)
            m_handle = fopen(path, "r");
        else if (mode == WRITE_TEXT)
            m_handle = fopen(path, "w");
        else
            m_mode = SK_NPOS32;
    }
    else
        printf("Invalid path name.\n");
}

void skFileStream::close(void)
{
    if (m_handle)
    {
        fclose(static_cast<FILE*>(m_handle));
        m_handle = 0;
    }
    m_mode = SK_NPOS32;
}



bool skFileStream::eof(void) const
{
    if (!isOpen())
        return true;

    return feof(static_cast<FILE*>(m_handle));
}

SKsize skFileStream::read(void* dst, SKsize nr) const
{
    if (!dst || !canRead() || !isOpen())
        return SK_NPOS;

    return fread(dst, 1, nr, static_cast<FILE*>(m_handle));
}


bool skFileStream::seek(SKint64 offs, SKsize dir)
{
    bool result = isOpen();
    if (result && offs < SK_NPOS)
    {
        long way;
        if (dir == SEEK_END)
            way = SEEK_END;
        else if (dir == SEEK_CUR)
            way = SEEK_CUR;
        else
            way = SEEK_SET;

        result = fseek(static_cast<FILE*>(m_handle), (long)offs, way) == 0;
    }
    return result;
}


SKsize skFileStream::write(const void* src, SKsize nr)
{
    if (!src || nr <= 0 || !isOpen() || !canWrite())
        return SK_NPOS;

    return fwrite(src, 1, nr, static_cast<FILE*>(m_handle));
}


SKsize skFileStream::position(void) const
{
    return isOpen() ? (SKsize)ftell(static_cast<FILE*>(m_handle)) : SK_NPOS;
}


SKsize skFileStream::size(void) const
{
    SKsize size = SK_NPOS;

    if (isOpen())
    {
        FILE* fp  = static_cast<FILE*>(m_handle);
        long  loc = ftell(fp);
        fseek(fp, 0L, SEEK_END);
        size = (SKsize)ftell(fp);
        fseek(fp, loc, SEEK_SET);
    }
    return size;
}


SKsize skStream::writef(const char* format, ...)
{
    char*   buf = 0;
    std::va_list l1;
    int     s1, s2;
    SKsize  bw = 0;

    if (format != nullptr)
    {
        va_start(l1, format);
        s1 = std::vsnprintf(buf, 0, format, l1);
        va_end(l1);

        if (s1 > 0)
        {
            buf = (char*)::malloc(s1 + 1);
            if (buf)
            {
                va_start(l1, format);
                s2 = std::vsnprintf(buf, s1 + 1, format, l1);
                va_end(l1);
                if (s2 > 0)
                {
                    buf[s2] = 0;
                    bw = write(buf, s2);
                }
                else
                    printf("vsnprintf returned an error\n");
                free(buf);
            }
            else
                printf("Alloc failed: skStream::writef\n");
        }
    }
    return bw;
}
