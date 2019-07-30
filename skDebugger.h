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
#ifndef _skDebugger_h_
#define _skDebugger_h_


#include "Utils/Config/skConfig.h"

enum skConsoleColorSpace
{
    CS_BLACK = 0,
    CS_DARKBLUE,
    CS_DARKGREEN,
    CS_DARKCYAN,
    CS_DARKRED,
    CS_DARKMAGENTA,
    CS_DARKYELLOW,
    CS_LIGHT_GREY,
    CS_GREY,
    CS_BLUE,
    CS_GREEN,
    CS_CYAN,
    CS_RED,
    CS_MAGENTA,
    CS_YELLOW,
    CS_WHITE,
    CS_COLOR_MAX
};


class skDebugger
{
public:

    enum Flags
    {
        PF_DISABLE_COLOR    = (1 << 0),
        PF_DEBUG_WINDOW_OUT = (1 << 1)
    };

    static void setPrintFlag(SKuint32 flag);



    // Halts the console and waits for the enter key
    static void pause(void);

    // Clears the console 
    static void clear(void);



    // Sets the foreground and background color of the console 
    static void writeColor(skConsoleColorSpace fg, skConsoleColorSpace bg = CS_BLACK);


    static bool isDebugger(void);
    static void report(const char* msg, ...);
    static void breakProcess(void);



private:


#if SK_PLATFORM == SK_PLATFORM_WIN32
    
    static void*         m_stdout;
    static unsigned char COLOR_TABLE[16][16];
    static unsigned char getColor(skConsoleColorSpace fore, skConsoleColorSpace back = CS_BLACK);

#elif SK_PLATFORM == SK_PLATFORM_LINUX

    static unsigned char* getColor(skConsoleColorSpace fore, skConsoleColorSpace back = CS_BLACK);

#endif

};





class skColorPrinter
{
public:
    static void print(skConsoleColorSpace foreground, const char* msg, ...);
};



#ifdef SK_NO_DEBUGGER
#include <stdio.h>
#define skPrintf ::printf
#else
#define skPrintf skDebugger::report
#endif



#define skCPrintf(fg, msg, ...) skColorPrinter::print(fg, msg, __VA_ARGS__)
#define skClear skDebugger::clear
#define skPause skDebugger::pause


#endif  //_skDebugger_h_
