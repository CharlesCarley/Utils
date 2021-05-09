/*
-------------------------------------------------------------------------------
    Copyright (c) Charles Carley.

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
#include <cstdarg>
#include <cstdio>
#include "Utils/skDebugger.h"
#include "Utils/skFileStream.h"
#include "Utils/skPlatformHeaders.h"
#include "Utils/skString.h"
#include "Utils/skTimer.h"

skLogger::skLogger() :
    m_stream(nullptr),
    m_flags(0),
    m_detail(LD_VERBOSE),
    m_listener(nullptr)
{
    setFlags(LF_STDOUT);
}

skLogger::skLogger(const int flags) :
    m_stream(nullptr),
    m_flags(0),
    m_detail(LD_VERBOSE),
    m_listener(nullptr)
{
    setFlags(flags);
}

skLogger::~skLogger()
{
    close();
}

void skLogger::setFlags(const int val)
{
    m_flags = val;
    if (m_flags & LF_DISABLE_COLOR)
        skDebugger::setPrintFlag(skDebugger::DISABLE_COLOR);
}

void skLogger::open(const char* logName)
{
    m_flags |= LF_FILE;
    delete m_stream;

    m_stream = new skFileStream(logName, skStream::WRITE);
}

void skLogger::close()
{
    delete m_stream;
    m_stream = nullptr;
    m_flags &= ~LF_FILE;
}

void skLogger::writeDetail(SKint32 detail) const
{
    char ts[6];
    switch (detail)
    {
    case LD_ASSERT:
        skChar::copyn(ts, "A : ", 5);
        break;
    case LD_ERROR:
        skChar::copyn(ts, "E : ", 5);
        break;
    case LD_WARN:
        skChar::copyn(ts, "W : ", 5);
        break;
    case LD_INFO:
        skChar::copyn(ts, "I : ", 5);
        break;
    case LD_DEBUG:
        skChar::copyn(ts, "D : ", 5);
        break;
    case LD_VERBOSE:
    default:
        skChar::copyn(ts, "V : ", 5);
        break;
    }
    ts[4] = 0;
    writeMessage(ts, 4);
}

void skLogger::writeDetailColorOn(SKint32 detail)
{
    switch (detail)
    {
    case LD_ASSERT:
        skDebugger::writeColor(CS_WHITE, CS_RED);
        break;
    case LD_ERROR:
        skDebugger::writeColor(CS_RED, CS_BLACK);
        break;
    case LD_WARN:
        skDebugger::writeColor(CS_YELLOW, CS_BLACK);
        break;
    case LD_INFO:
        skDebugger::writeColor(CS_LIGHT_GREY, CS_BLACK);
        break;
    case LD_DEBUG:
        skDebugger::writeColor(CS_DARKGREEN, CS_BLACK);
        break;
    default:
    case LD_VERBOSE:
        skDebugger::writeColor(CS_WHITE, CS_BLACK);
        break;
    }
}

void skLogger::resetColor()
{
    skDebugger::writeColor(CS_WHITE, CS_BLACK);
}

void skLogger::writeTimeStamp() const
{
    char           ts[33];
    const SKuint32 br = skMin<SKuint32>(
        skGetTimeString(ts, 32, "%m/%d/%y @ %H:%M:%S :"), 32);
    SK_ASSERT(br < 32);

    ts[br] = 0;
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
            // either in the output or immediate window.
            OutputDebugString(msg);
        }
    }
#endif
}

void skLogger::logMessage(SKint32 detail, const char* msg, SKint32 len) const
{
    if (msg && m_detail >= detail)
    {
        if (!(m_flags & LF_DISABLE_COLOR))
            writeDetailColorOn(detail);

        if (m_flags & LF_DETAIL)
            writeDetail(detail);

        if (m_flags & LF_TIMESTAMP)
            writeTimeStamp();

        writeMessage(msg, len);

        resetColor();
    }
}

void skLogger::logStandard(SKint32 detail, const char* msg)
{
    if (!msg)
        return;

    writeDetailColorOn(detail);

    skDebugger::log(msg);
    resetColor();
}

void skLogd(SKint32 detail, const char* msg)
{
    const SKsize len = skChar::length(msg);
    if (len > 0)
    {
        const skLogger* log = skLogger::getSingletonPtr();
        if (log)
            log->logMessage(detail, msg, (SKint32)len);
        else
            skLogger::logStandard(detail, msg);
    }
}

void skLogi(const char* msg)
{
    skLogd(LD_INFO, msg);
}

void skLogf(SKint32 detail, const char* format, ...)
{
    const skLogger* log = skLogger::getSingletonPtr();

    bool doFormat = log != nullptr && log->getDetail() >= detail;
    if (!doFormat && !log)
        doFormat = true;

    if (format && doFormat)
    {
        va_list l1;
        char*   buffer = nullptr;
        va_start(l1, format);
        int size = std::vsnprintf(buffer, 0, format, l1);
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

                if (log)
                    log->logMessage(detail, buffer, size);
                else
                    skLogger::logStandard(detail, buffer);

                ::free(buffer);
            }
            else
                printf("Failed to allocate buffer\n");
        }
        else
            printf("Error: Log string length is < 0\n");
    }
}

SK_IMPLEMENT_SINGLETON(skLogger);
