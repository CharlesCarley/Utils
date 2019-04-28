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


#include "Config/skConfig.h"

class skStream;


class skLogger
{
public:

    void openStream(skStream* stream);

    void logMessage(const char* msg, SKint32 len = 0);

    static skLogger& get(void);

private:
    skLogger();
    ~skLogger();

    skStream* m_stream;
};

extern void skLogf(const char* fmt, ...);
extern void skEnableLogging(bool v, const char* path);


#endif//_skLogger_h_
