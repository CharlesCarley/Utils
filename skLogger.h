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
#ifndef _skLogger_h_
#define _skLogger_h_

#include "Utils/Config/skConfig.h"
#include "Utils/skSingleton.h"

class skStream;

enum skLogDetail
{
    LD_ASSERT,
    LD_ERROR,
    LD_WARN,
    LD_INFO,
    LD_DEBUG,
    LD_VERBOSE,
};

enum LogFlag
{
    LF_STDOUT    = 1 << 0,
    LF_VS_DEBUG  = 1 << 1,
    LF_FILE      = 1 << 2,
    LF_TIMESTAMP = 1 << 3,
    LF_DETAIL    = 1 << 4,
    LF_ALL       = LF_STDOUT | LF_VS_DEBUG | LF_TIMESTAMP | LF_DETAIL,
};

class skLogListener
{
public:
    skLogListener()
    {
    }
    virtual ~skLogListener()
    {
    }

    virtual void messageLogged(int detail, int setflags, const char* logged, int length) = 0;
};

class skLogger : public skSingleton<skLogger>
{
public:
    SK_DECLARE_SINGLETON(skLogger)

private:
    skStream*      m_stream;    // Open ors LF_FILE
    int            m_flags;     // Default: LF_ALL
    int            m_detail;    // Default: LD_VERBOSE
    skLogListener* m_listener;  // just one.

public:
    skLogger();
    ~skLogger();

    inline void setDetail(int val)
    {
        m_detail = val;
    }

    inline int getDetail() const
    {
        return m_detail;
    }

    inline void setFlags(int val)
    {
        m_flags = val;
    }

    inline int getFlags() const
    {
        return m_flags;
    }

    inline void clearFlag(int flag)
    {
        m_flags &= ~flag;
    }

    void setListener(skLogListener* listener)
    {
        m_listener = listener;
    }

    void open(const char* logName);
    void logMessage(SKint32 detail, const char* msg, SKint32 len = 0) const;

private:
    void writeDetail(SKint32 detail) const;
    void writeTimeStamp() const;
    void writeMessage(const char* msg, SKint32 len = 0) const;
};

extern void skLogf(SKint32 detail, const char* format, ...);

#endif  //_skLogger_h_
