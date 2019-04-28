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
#include "skDebugger.h"
#include "Config/skConfig.h"
#include "skPlatformHeaders.h"

#ifdef SK_DEBUG

bool skDebugger::isDebugger(void)
{
#if SK_COMPILER == SK_COMPILER_MSVC
    return IsDebuggerPresent() != 0;
#else
    return false;
#endif
}



void skDebugger::breakProcess(void)
{
    if (!isDebugger())
        return;
#if SK_COMPILER == SK_COMPILER_MSVC
    //asm( "int $3" );
    _asm int 3;
#else
    asm("int $3");
#endif
}



#else//SK_DEBUG


bool skDebugger::isDebugger(void)
{
    return false;
}

void skDebugger::breakProcess(void)
{
}



#endif//SK_DEBUG


static char ReportBuf[SK_SBUF_SIZE + 1];


void skDebugger::report(const char* fmt, ...)
{

    int size;
    va_list lst;
    va_start(lst, fmt);
    size = skp_printf(ReportBuf, SK_SBUF_SIZE, fmt, lst);
    va_end(lst);

    if (size < 0) size = SK_SBUF_SIZE;

    if (size > 0)
    {
        ReportBuf[size] = 0;

#if SK_COMPILER == SK_COMPILER_MSVC

        if (IsDebuggerPresent())
            OutputDebugString(ReportBuf);
        else
#endif
            fprintf(stderr, "%s", ReportBuf);
    }
}
