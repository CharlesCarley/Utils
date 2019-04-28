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
#include "skLogger.h"
#include "skFileStream.h"
#include "skString.h"
#include "skPlatformHeaders.h"
#include "skDebugger.h"
#include <ctime>

#if SK_PLATFORM == SK_PLATFORM_ANDROID
    #include <android/log.h>
    #define skAndroidLog(...) ((void)__android_log_print(ANDROID_LOG_INFO, "sk_logger", __VA_ARGS__))
#endif



static const char LOGHEADER[] =
{
    0x3C, 0x21, 0x44, 0x4F, 0x43, 0x54, 0x59, 0x50, 0x45, 0x20, 0x68, 0x74, 0x6D, 0x6C, 0x20, 0x50, 0x55, 0x42, 0x4C, 0x49, 0x43, 0x20, 0x22, 0x2D, 0x2F, 0x2F,
    0x57, 0x33, 0x43, 0x2F, 0x2F, 0x44, 0x54, 0x44, 0x20, 0x58, 0x48, 0x54, 0x4D, 0x4C, 0x20, 0x31, 0x2E, 0x30, 0x20, 0x54, 0x72, 0x61, 0x6E, 0x73, 0x69, 0x74,
    0x69, 0x6F, 0x6E, 0x61, 0x6C, 0x2F, 0x2F, 0x45, 0x4E, 0x22, 0x20, 0x22, 0x68, 0x74, 0x74, 0x70, 0x3A, 0x2F, 0x2F, 0x77, 0x77, 0x77, 0x2E, 0x77, 0x33, 0x2E,
    0x6F, 0x72, 0x67, 0x2F, 0x54, 0x52, 0x2F, 0x78, 0x68, 0x74, 0x6D, 0x6C, 0x31, 0x2F, 0x44, 0x54, 0x44, 0x2F, 0x78, 0x68, 0x74, 0x6D, 0x6C, 0x31, 0x2D, 0x74,
    0x72, 0x61, 0x6E, 0x73, 0x69, 0x74, 0x69, 0x6F, 0x6E, 0x61, 0x6C, 0x2E, 0x64, 0x74, 0x64, 0x22, 0x3E, 0x0A, 0x3C, 0x68, 0x74, 0x6D, 0x6C, 0x20, 0x78, 0x6D,
    0x6C, 0x6E, 0x73, 0x3D, 0x22, 0x68, 0x74, 0x74, 0x70, 0x3A, 0x2F, 0x2F, 0x77, 0x77, 0x77, 0x2E, 0x77, 0x33, 0x2E, 0x6F, 0x72, 0x67, 0x2F, 0x31, 0x39, 0x39,
    0x39, 0x2F, 0x78, 0x68, 0x74, 0x6D, 0x6C, 0x22, 0x20, 0x3E, 0x3C, 0x68, 0x65, 0x61, 0x64, 0x3E, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x74, 0x69, 0x74, 0x6C,
    0x65, 0x3E, 0x45, 0x6E, 0x67, 0x69, 0x6E, 0x65, 0x20, 0x4C, 0x6F, 0x67, 0x3C, 0x2F, 0x74, 0x69, 0x74, 0x6C, 0x65, 0x3E, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C,
    0x73, 0x74, 0x79, 0x6C, 0x65, 0x20, 0x74, 0x79, 0x70, 0x65, 0x3D, 0x22, 0x74, 0x65, 0x78, 0x74, 0x2F, 0x63, 0x73, 0x73, 0x22, 0x3E, 0x0A, 0x20, 0x20, 0x20,
    0x20, 0x62, 0x6F, 0x64, 0x79, 0x20, 0x7B, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x62, 0x61, 0x63, 0x6B, 0x67, 0x72, 0x6F, 0x75, 0x6E, 0x64,
    0x3A, 0x20, 0x77, 0x68, 0x69, 0x74, 0x65, 0x3B, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x63, 0x6F, 0x6C, 0x6F, 0x72, 0x3A, 0x20, 0x62, 0x6C,
    0x61, 0x63, 0x6B, 0x3B, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x6D, 0x61, 0x72, 0x67, 0x69, 0x6E, 0x2D, 0x72, 0x69, 0x67, 0x68, 0x74, 0x3A,
    0x20, 0x32, 0x30, 0x70, 0x78, 0x3B, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x6D, 0x61, 0x72, 0x67, 0x69, 0x6E, 0x2D, 0x6C, 0x65, 0x66, 0x74,
    0x3A, 0x20, 0x32, 0x30, 0x70, 0x78, 0x3B, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x6D, 0x61, 0x72, 0x67, 0x69, 0x6E, 0x2D, 0x74, 0x6F, 0x70,
    0x3A, 0x20, 0x30, 0x70, 0x78, 0x3B, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x70, 0x61, 0x64, 0x64, 0x69, 0x6E, 0x67, 0x2D, 0x74, 0x6F, 0x70,
    0x3A, 0x20, 0x30, 0x70, 0x78, 0x3B, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x7D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x68, 0x31, 0x7B, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x74, 0x65, 0x78, 0x74, 0x2D, 0x61, 0x6C, 0x69, 0x67, 0x6E, 0x3A, 0x20, 0x63, 0x65, 0x6E, 0x74, 0x65, 0x72, 0x3B, 0x0A, 0x20, 0x20, 0x20,
    0x20, 0x20, 0x20, 0x20, 0x20, 0x66, 0x6F, 0x6E, 0x74, 0x2D, 0x73, 0x69, 0x7A, 0x65, 0x3A, 0x20, 0x31, 0x35, 0x30, 0x25, 0x3B, 0x0A, 0x20, 0x20, 0x20, 0x20,
    0x7D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x68, 0x72, 0x7B, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x68, 0x65, 0x69, 0x67, 0x68, 0x74, 0x3A, 0x20,
    0x30, 0x70, 0x78, 0x3B, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x62, 0x6F, 0x72, 0x64, 0x65, 0x72, 0x3A, 0x20, 0x6E, 0x6F, 0x6E, 0x65, 0x3B,
    0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x62, 0x6F, 0x72, 0x64, 0x65, 0x72, 0x2D, 0x74, 0x6F, 0x70, 0x3A, 0x20, 0x31, 0x70, 0x78, 0x20, 0x64,
    0x6F, 0x74, 0x74, 0x65, 0x64, 0x20, 0x67, 0x72, 0x61, 0x79, 0x3B, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x7D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x70, 0x72, 0x65, 0x7B,
    0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x62, 0x61, 0x63, 0x6B, 0x67, 0x72, 0x6F, 0x75, 0x6E, 0x64, 0x3A, 0x20, 0x23, 0x46, 0x39, 0x46, 0x39,
    0x46, 0x39, 0x3B, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x63, 0x6F, 0x6C, 0x6F, 0x72, 0x3A, 0x20, 0x62, 0x6C, 0x61, 0x63, 0x6B, 0x3B, 0x0A,
    0x20, 0x20, 0x20, 0x20, 0x7D, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x3C, 0x2F, 0x73, 0x74, 0x79, 0x6C, 0x65, 0x3E, 0x0A, 0x3C, 0x2F, 0x68, 0x65, 0x61, 0x64, 0x3E,
    0x3C, 0x62, 0x6F, 0x64, 0x79, 0x3E, 0x3C, 0x63, 0x65, 0x6E, 0x74, 0x65, 0x72, 0x3E, 0x0A, 0x3C, 0x68, 0x31, 0x3E, 0x45, 0x6E, 0x67, 0x69, 0x6E, 0x65, 0x20,
    0x4C, 0x6F, 0x67, 0x3C, 0x2F, 0x68, 0x31, 0x3E, 0x3C, 0x68, 0x72, 0x20, 0x2F, 0x3E, 0x3C, 0x74, 0x61, 0x62, 0x6C, 0x65, 0x20, 0x77, 0x69, 0x64, 0x74, 0x68,
    0x3D, 0x22, 0x39, 0x35, 0x25, 0x22, 0x3E, 0x3C, 0x74, 0x72, 0x3E, 0x3C, 0x74, 0x64, 0x3E, 0x0A, 0x3C, 0x70, 0x72, 0x65, 0x3E, 0x0A, 0x0A, 0x00
};// LogHeader.html

static const char LOGFOOTER[] =
{
    0x3C, 0x2F, 0x70, 0x72, 0x65, 0x3E, 0x3C, 0x2F, 0x74, 0x64, 0x3E, 0x3C, 0x2F, 0x74, 0x72, 0x3E, 0x3C, 0x2F, 0x74, 0x61, 0x62, 0x6C, 0x65, 0x3E, 0x3C, 0x2F,
    0x63, 0x65, 0x6E, 0x74, 0x65, 0x72, 0x3E, 0x3C, 0x2F, 0x62, 0x6F, 0x64, 0x79, 0x3E, 0x3C, 0x2F, 0x68, 0x74, 0x6D, 0x6C, 0x3E, 0x0A, 0x00
};// LogFooter.html



void skLogger::openStream(skStream* stream)
{
    if (m_stream)
    {
        m_stream->write(LOGFOOTER, sizeof(LOGFOOTER) - 1);
        delete m_stream;
    }

    m_stream = stream;
    if (m_stream)
        m_stream->write(LOGHEADER, sizeof(LOGHEADER) - 1);
}

void skLogger::logMessage(const char* msg, SKint32 len)
{
    char buf[SK_SBUF_SIZE];
    char tbuf[32];

    time_t t;
    time(&t);
    strftime(tbuf, 32, "%H:%M:%S", localtime(&t));

    skSprintf(buf, SK_SBUF_SIZE - 1, "%s - %s", tbuf, msg);

#if SK_PLATFORM == SK_PLATFORM_ANDROID
    skAndroidLog(msg);
#else
    skPrintf(msg);
#endif
    if (!m_stream)
        return;

    len = skStringUtils::length(buf);
    if (len <= 0)
        return;

    m_stream->write(buf, len);
}


skLogger& skLogger::get(void)
{
    static skLogger log;
    return log;
}

skLogger::skLogger()
{
    m_stream = 0;
}

skLogger::~skLogger()
{
    if (m_stream)
        m_stream->write(LOGFOOTER, sizeof(LOGFOOTER) - 1);
    delete m_stream;
    m_stream = 0;
}


static bool sk_log = false;


void skEnableLogging(bool v, const char* path)
{
    sk_log = v;
    if (v)
    {
        skFileStream* fs = new skFileStream();
        fs->open(path, skStream::WRITE);
        skLogger::get().openStream(fs);
    }
}


void skLogf(const char* fmt, ...)
{
    static char dst[SK_SBUF_SIZE + 1];

    int size;
    va_list lst;

    if (!fmt) return;

    va_start(lst, fmt);
    size = skp_printf(dst, SK_SBUF_SIZE, fmt, lst);
    va_end(lst);

    if (size < 0)  size = SK_SBUF_SIZE;

    dst[size] = 0;
    if (sk_log)
        skLogger::get().logMessage(dst, size);
    else
    {
#if SK_PLATFORM == SK_PLATFORM_ANDROID
        skAndroidLog(dst);
        skPrintf(dst);
#else
        skPrintf(dst);
#endif
    }
}
