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
#include "Utils/skLogger.h"
#include "Utils/skDebugger.h"
#include <cstdarg>
#include <cstdio>
#include "Utils/skFileStream.h"
#include "Utils/skPlatformHeaders.h"
#include "Utils/skTimer.h"


skLogger::skLogger() :
    m_stream(nullptr),
    m_flags(LF_ALL),
    m_detail(LD_VERBOSE),
    m_listener(nullptr)
{
}

skLogger::~skLogger()
{
    delete m_stream;
    m_stream = nullptr;
}



void skLogger::open(const char* logname)
{
    m_flags |= LF_FILE;
    delete m_stream;
    m_stream = new skFileStream(logname, skStream::WRITE);
}


void skLogger::writeDetail(SKint32 detail) const
{
    char ts[6];
    switch (detail)
    {
    case LD_ASSERT:
        strcpy(ts, "00: ");
        break;
    case LD_ERROR:
        strcpy(ts, "01: ");
        break;
    case LD_WARN:
        strcpy(ts, "02: ");
        break;
    case LD_INFO:
        strcpy(ts, "03: ");
        break;
    case LD_DEBUG:
        strcpy(ts, "04: ");
        break;
    case LD_VERBOSE:
    default:
        strcpy(ts, "05: ");
        break;
    }
    ts[4] = 0;
    writeMessage(ts, 4);
}


void skLogger::writeTimeStamp() const
{
    char       ts[33];
    const auto br = skMin<SKuint32>(skGetTimeString(ts, 32, "%m:%d:%C:%H:%M:%S:"), 32);
    ts[br]        = 0;
    writeMessage(ts, br);
}


void skLogger::writeMessage(const char* msg, SKint32 len) const
{
    if (m_flags & LF_STDOUT)
        fprintf(stdout, "%s", msg);

    if (len <= 0)
        len = (SKint32)strlen(msg);

    if (m_stream && m_flags & LF_FILE)
    {
        if (len > 0)
            m_stream->write(msg, (SKsize)len);
    }

    if (m_listener)
    {
        if (len > 0)
            m_listener->messageLogged(m_detail, m_flags, msg, len);
    }

#if SK_COMPILER == SK_COMPILER_MSVC
    if (m_flags & LF_VS_DEBUG)
    {
        if (IsDebuggerPresent())
        {
            OutputDebugString(msg);
        }
    }
#endif
}


void skLogger::logMessage(SKint32 detail, const char* msg, SKint32 len) const
{
    if (msg && m_detail >= detail)
    {
        if (m_flags & LF_TIMESTAMP)
            writeTimeStamp();

        if (detail == LD_WARN)
            skDebugger::writeColor(CS_YELLOW);

        if (detail <= LD_ERROR)
            skDebugger::writeColor(CS_RED);

        if (m_flags & LF_DETAIL)
            writeDetail(detail);

        writeMessage(msg, len);
        
        if (detail <= LD_WARN)
            skDebugger::writeColor(CS_WHITE);
    }
}


void skLogf(SKint32 detail, const char* format, ...)
{
    const skLogger* log = skLogger::getSingletonPtr();
    if (log)
    {
        if (format && log->getDetail() >= detail)
        {
            va_list l1;
            int     size;

            char* buffer = nullptr;
            va_start(l1, format);
            size = std::vsnprintf(buffer, 0, format, l1);
            va_end(l1);

            if (size > 0)
            {
                size   = skMin(size, 4095);
                buffer = (char*)::malloc((SKsize)size + 1);

                if (buffer)
                {
                    va_start(l1, format);
                    size = std::vsnprintf(buffer, (SKsize)size + 1, format, l1);
                    va_end(l1);

                    log->logMessage(detail, buffer, size);
                    ::free(buffer);
                }
                else
                    printf("Failed to allocate buffer\n");
            }
            else
                printf("Error: Log string length is < 0\n");
        }
    }
}

SK_IMPLEMENT_SINGLETON(skLogger);
