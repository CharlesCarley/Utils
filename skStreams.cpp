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
#include "skStreams.h"
#include <cstdarg>
#include <cstdio>
#include "skPlatformHeaders.h"

skStream::skStream() :
    m_mode(UNDEFINED)
{
}

skStream::~skStream()
{
}

SKsize skStream::writef(const char* format, ...)
{
    char*        buf = nullptr;
    std::va_list l1;
    int          s1, s2;
    SKsize       bw = 0;

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
                    bw      = write(buf, s2);
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
