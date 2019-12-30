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
#include "skTimer.h"
#include <time.h>
#include "skMinMax.h"
#include "skPlatformHeaders.h"


#if SK_PLATFORM == SK_PLATFORM_WIN32

class skTimerPrivate
{
public:
    skTimerPrivate() :
        m_freq(0),
        m_beg(0),
        m_cur(0),
        m_mask(1)
    {
    }
    SKuint64 m_freq;
    SKuint64 m_beg;
    SKuint64 m_cur;
    SKuint32 m_mask;
};

skTimer::skTimer() :
    m_private(new skTimerPrivate())
{
    reset();
}

skTimer::~skTimer()
{
    delete m_private;
}

void skTimer::reset(void)
{
    QueryPerformanceFrequency((LARGE_INTEGER *)&m_private->m_freq);

    ULONG_PTR proc, sys;
    GetProcessAffinityMask(GetCurrentProcess(), (PDWORD_PTR)&proc, (PDWORD_PTR)&sys);
    proc              = skMax<ULONG_PTR>(proc, 1);
    m_private->m_mask = 1;

    while (m_private->m_mask & proc)
        m_private->m_mask <<= 1;

    QueryPerformanceCounter((LARGE_INTEGER *)&m_private->m_beg);
    m_private->m_cur = 0;
}

SKulong skTimer::getMilliseconds(void)
{
    HANDLE    curThread = GetCurrentThread();
    DWORD_PTR old       = SetThreadAffinityMask(curThread, m_private->m_mask);
    QueryPerformanceCounter((LARGE_INTEGER *)&m_private->m_cur);
    SetThreadAffinityMask(curThread, old);

    m_private->m_cur -= m_private->m_beg;
    return static_cast<SKulong>((double)m_private->m_cur * 1000 / (double)m_private->m_freq);
}

SKulong skTimer::getMicroseconds(void)
{
    HANDLE    curThread = GetCurrentThread();
    DWORD_PTR old       = SetThreadAffinityMask(curThread, m_private->m_mask);
    QueryPerformanceCounter((LARGE_INTEGER *)&m_private->m_cur);
    SetThreadAffinityMask(curThread, old);

    m_private->m_cur -= m_private->m_beg;
    return static_cast<SKulong>((double)m_private->m_cur * 1000000 / (double)m_private->m_freq);
}

void skSleep(SKuint32 ms)
{
    ::Sleep(ms);
}

#else

#include <sys/time.h>
#include <unistd.h>

class skTimerPrivate
{
public:
    skTimerPrivate()
    {
    }
    struct timeval start;
    struct timeval now;
};

skTimer::skTimer() :
    m_private(new skTimerPrivate())
{
    reset();
}

skTimer::~skTimer()
{
    delete m_private;
}

void skTimer::reset(void)
{
    gettimeofday(&m_private->start, NULL);
}

SKulong skTimer::getMilliseconds(void)
{
    gettimeofday(&m_private->now, NULL);
    return (m_private->now.tv_sec - m_private->start.tv_sec) * 1000 +
           (m_private->now.tv_usec - m_private->start.tv_usec) / 1000;
}

SKulong skTimer::getMicroseconds(void)
{
    gettimeofday(&m_private->now, NULL);
    return (m_private->now.tv_sec - m_private->start.tv_sec) * 1000000 +
           (m_private->now.tv_usec - m_private->start.tv_usec);
}

void skSleep(SKuint32 ms)
{
    usleep(ms);
}

#endif

SKulong skGetMilliseconds(void)
{
    static skTimer m_timer;
    return m_timer.getMilliseconds();
}

SKulong skGetMicroseconds(void)
{
    static skTimer m_timer;
    return m_timer.getMicroseconds();
}

SKulong skGetTickCount(void)
{
#if SK_PLATFORM == SK_PLATFORM_WIN32
    return ::GetTickCount();
#else
    static timeval now;
    gettimeofday(&now, NULL);
    return (now.tv_sec) / 1000;
#endif
}

SKuint32 skGetTimeString(char *buffer, SKsize bufSize, const char *fmt)
{
    if (!buffer || bufSize == 0 || bufSize > 64 || !fmt)
        return SK_NPOS32;

    time_t now;
    ::time(&now);

    return skGetTimeString(buffer, bufSize, fmt, now);
}


SKuint32 skGetTimeString(char *buffer, SKsize bufSize, const char *fmt, const long long &timestamp)
{
    if (!buffer || bufSize == 0 || bufSize > 64 || !fmt)
        return SK_NPOS32;

    SKuint64 bw = 0;
    tm *     tval;
    tval = ::localtime((time_t *)&timestamp);

    if (tval)
    {
        bw = ::strftime(buffer, bufSize, fmt, tval);
        if (bw != 0 && bw < bufSize)
            buffer[bw] = 0;
        else
            bw = SK_NPOS32;
    }
    else
        ::memset(buffer, 0, bufSize);
    return (SKuint32)bw;
}
