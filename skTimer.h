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
#ifndef _skTimer_h_
#define _skTimer_h_

#include "skAllocator.h"

class skTimer
{
public:
    skTimer();
    ~skTimer();

    void    reset(void);
    SKulong getMilliseconds(void);
    SKulong getMicroseconds(void);

private:
    class skTimerPrivate* m_private;
};

extern SKulong skGetMilliseconds(void);
extern SKulong skGetMicroseconds(void);
extern SKulong skGetTickCount(void);
extern void    skSleep(SKuint32 ms);

// Fills the destination buffer up to bufSize.
// It internally passes fmt to strftime(http://www.cplusplus.com/reference/ctime/strftime/)
extern SKuint32 skGetTimeString(char* buffer, SKsize bufSize, const char* fmt);
extern SKuint32 skGetTimeString(char* buffer, SKsize bufSize, const char* fmt, const long long& timestamp);

#endif  //_skTimer_h_
