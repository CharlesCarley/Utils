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
#include <cstdarg>
#include <cstdio>
#include "Config/skConfig.h"
#include "skPlatformHeaders.h"
#include "skDisableWarnings.h"


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
    DebugBreak();
#else
    asm("int $3");
#endif
}

#else  // SK_DEBUG

bool skDebugger::isDebugger(void)
{
    return false;
}

void skDebugger::breakProcess(void)
{
}


#endif  // SK_DEBUG


static SKuint32     skPrintFlags             = 0;
static char         Buffer[SK_SBUF_SIZE + 1] = {0};
skConsoleColorSpace skDebugger::m_cacheFore  = CS_WHITE;
skConsoleColorSpace skDebugger::m_cacheBack  = CS_BLACK;


void skDebugger::setPrintFlag(SKuint32 flag)
{
    skPrintFlags = flag;
}


void skDebugger::report(const char* message, ...)
{
    if (message != nullptr)
    {
        va_list l1;
        int     s1, s2;

        char* buffer = nullptr;

        va_start(l1, message);
        s1 = std::vsnprintf(buffer, 0, message, l1);
        va_end(l1);

        if (s1 > 0)
        {
            buffer = (char*)malloc((SKsize)s1 + 1);
            if (buffer)
            {
                va_start(l1, message);
                s2 = std::vsnprintf(buffer, (SKsize)s1 + 1, message, l1);
                va_end(l1);
                skDebugger::log(buffer);
                free(buffer);
            }
        }
    }
}



void skDebugger::log(const char* message)
{
    if (message != nullptr)
    {
#if SK_COMPILER == SK_COMPILER_MSVC
        if (skPrintFlags & VS_DBG_OUTPUT_PANEL && IsDebuggerPresent())
            OutputDebugString(message);
        else
#endif
        {
            fputs(message, stdout);
        }
    }
}



#if SK_PLATFORM == SK_PLATFORM_WIN32
const unsigned char COLOR_TABLE[16][16] = {
    {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F},
    {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F},
    {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F},
    {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F},
    {0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F},
    {0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F},
    {0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F},
    {0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F},
    {0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F},
    {0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F},
    {0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF},
    {0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF},
    {0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF},
    {0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF},
    {0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF},
    {0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF},
};


unsigned char skDebugger::getColor(skConsoleColorSpace fore, skConsoleColorSpace back)
{
    return COLOR_TABLE[back][fore];
}

#else

#if SK_PLATFORM == SK_PLATFORM_LINUX || SK_PLATFORM == SK_PLATFORM_APPLE
unsigned char* skDebugger::getColor(skConsoleColorSpace fore, skConsoleColorSpace)
{
    static unsigned char color[2];
    switch (fore)
    {
    case CS_DARKGREEN:
        color[0] = 32;
        break;
    case CS_GREEN:
        color[0] = 92;
        break;
    case CS_CYAN:
        color[0] = 96;
        break;
    case CS_DARKCYAN:
        color[0] = 36;
        break;
    case CS_YELLOW:
        color[0] = 93;
        break;
    case CS_DARKYELLOW:
        color[0] = 33;
        break;
    case CS_RED:
        color[0] = 91;
        break;
    case CS_DARKRED:
        color[0] = 31;
        break;
    case CS_DARKBLUE:
        color[0] = 34;
        break;
    case CS_BLUE:
        color[0] = 94;
        break;
    case CS_DARKMAGENTA:
        color[0] = 35;
        break;
    case CS_MAGENTA:
        color[0] = 95;
        break;
    case CS_BLACK:
        color[0] = 30;
        break;
    case CS_LIGHT_GREY:
        color[0] = 37;
        break;
    case CS_GREY:
        color[0] = 90;
        break;
    case CS_WHITE:
        color[0] = 37;
        break;
    case CS_COLOR_MAX:
    default:
        break;
    }

    // leaving out the background color for now...
    color[1] = color[0];
    return color;
}
#endif
#endif


void skDebugger::writeColor(skConsoleColorSpace fg, skConsoleColorSpace bg)
{
    if ((skPrintFlags & DISABLE_COLOR) != 0)
        return;

    if (fg < 0 || fg > CS_COLOR_MAX || bg < 0 || bg > CS_COLOR_MAX ||
        (m_cacheFore == fg && m_cacheBack == bg))
        return;

    // Remember the last set color so the
    // colors are not unnecessarily being set.
    m_cacheFore = fg;
    m_cacheBack = bg;

#if SK_PLATFORM == SK_PLATFORM_LINUX || SK_PLATFORM == SK_PLATFORM_APPLE
    unsigned char* col = getColor(m_cacheFore, m_cacheBack);
    printf("\e[%im", col[0]);

#elif SK_PLATFORM == SK_PLATFORM_WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), getColor(m_cacheFore, m_cacheBack));
#else
    // untested
#endif
}


void skDebugger::clear(void)
{
#if SK_PLATFORM == SK_PLATFORM_WIN32
    system("cls");
#elif SK_PLATFORM == SK_PLATFORM_APPLE
    system("clear");
#elif SK_PLATFORM == SK_PLATFORM_LINUX
    printf("\33c");
#endif
}


void skDebugger::pause(void)
{
    if ((skPrintFlags & DISABLE_COLOR) == 0)
        writeColor(CS_WHITE);

    puts("\nPress enter to continue . . .");

    (void)getc(stdin);
    for (;;)
    {
        const int ch = getc(stdin);
        if (ch == '\n' || ch == '\r')
            break;
    }
}


void skConsolePrint(skConsoleColorSpace fg, const char* fmt, ...)
{
    if ((skPrintFlags & skDebugger::DISABLE_COLOR) == 0)
        skDebugger::writeColor(fg);

    if (!fmt)
        return;
    va_list lst;

    va_start(lst, fmt);
    int size = skp_printf(Buffer, SK_SBUF_SIZE, fmt, lst);
    va_end(lst);

    if (size < 0)
        size = 0;

    if (size > 0 && size < SK_SBUF_SIZE)
    {
        Buffer[size] = 0;
        puts(Buffer);
    }
}

void skConsoleClear(void)
{
    skDebugger::clear();
}

void skConsolePause(void)
{
    skDebugger::pause();
}

void skSetConsoleFg(int fgColor)
{
    skDebugger::writeColor((skConsoleColorSpace)fgColor, skDebugger::getCachedBackground());
}

void skSetConsoleBg(int bgColor)
{
    skDebugger::writeColor(skDebugger::getCachedForeground(), (skConsoleColorSpace)bgColor);
}
