/*
-------------------------------------------------------------------------------

    Copyright (c) 2019 Charles Carley.

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
#include "skHexPrint.h"
#include "skPlatformHeaders.h"

namespace skHexPrint
{
    enum PointerStackSize
    {
        PL8 = sizeof(void*) == 8,
        PL4 = sizeof(void*) == 4,
    };

    void writeColor(skConsoleColorSpace cs)
    {
        skDebugger::writeColor(cs);
    }

    void writeHex(const char* cp,
                  SKsize      offs,
                  SKsize      max,
                  SKuint64    mark,
                  SKuint32    flags)
    {
        SKuint8 c1, c2, c3, c4;
        SKsize  j, n = 0;

        union
        {
            SKuint8  b[4];
            SKuint64 i;
        } cmp;

        cmp.i = mark;

        if (!cp || offs == SK_NPOS32 || max == (SK_NPOS - 1))
            return;

        for (j = 0; j < 16; ++j)
        {
            if (j % 8 == 0)
                printf(" ");

            if (offs + j < max)
            {
                c1 = (SKuint8)(SKuint32)cp[offs + j];

                if (n)
                {
                    n--;
                    markColor(1, 1);
                }
                else
                {
                    if (mark < 256)
                        markColor(c1, cmp.i);
                    else if (mark < 65536)
                    {
                        if (n == 0)
                        {
                            c2 = (SKuint8)(SKuint32)cp[offs + (j + 1) % 16];

                            if (c1 == cmp.b[1] && c2 == cmp.b[0] ||
                                c1 == cmp.b[0] && c2 == cmp.b[1])
                            {
                                n = 1;  // bleed through x times
                                markColor(1, 1);
                            }
                            else
                            {
                                markColor(c1, cmp.i);
                                n = 0;
                            }
                        }
                    }
                    else
                    {
                        if (n == 0)
                        {
                            c2 = (SKuint8)(SKuint32)cp[offs + (j + 1) % 16];
                            c3 = (SKuint8)(SKuint32)cp[offs + (j + 2) % 16];
                            c4 = (SKuint8)(SKuint32)cp[offs + (j + 3) % 16];

                            if (c1 == cmp.b[3] && c2 == cmp.b[2] && c3 == cmp.b[1] && c4 == cmp.b[0] ||
                                c1 == cmp.b[0] && c2 == cmp.b[1] && c3 == cmp.b[2] && c4 == cmp.b[3])
                            {
                                n = 3;  // bleed through x times
                                markColor(1, 1);
                            }
                            else
                            {
                                markColor(c1, cmp.i);
                                n = 0;
                            }
                        }
                    }
                }
                printf("%02X ", c1);
            }
            else
                printf("   ");
        }

        writeColor(CS_WHITE);
    }

    void writeHex(const char* cp,
                  SKsize      offs,
                  SKsize      max,
                  SKuint32    flags)
    {
        SKuint8 c1;
        SKsize  j, n = 0;

        if (!cp || offs == SK_NPOS32 || max == (SK_NPOS - 1))
            return;

        for (j = 0; j < 16; ++j)
        {
            if (j % 8 == 0)
                printf(" ");

            if (offs + j < max)
            {
                c1 = (SKuint8)(SKuint32)cp[offs + j];
                printf("%02X ", c1);
            }
            else
                printf("   ");
        }

        writeColor(CS_WHITE);
    }

    void writeHex(const char* cp,
                  SKsize      size)
    {
        dumpHex((const void*)cp, 0, size, PF_DEFAULT, -1);
    }

    void writeAscii(const char* cp,
                    SKsize      offs,
                    SKsize      max,
                    SKuint64    mark,
                    SKuint32    flags)
    {
        SKuint8 c;
        SKsize  j;

        if (!cp || offs == SK_NPOS || max == SK_NPOS)
            return;

        if (flags & PF_COLORIZE)
            writeColor(CS_WHITE);

        printf(" |");
        for (j = 0; j < 16; ++j)
        {
            if (offs + j < max)
            {
                c = (SKuint8)cp[offs + j];

                if (flags & PF_COLORIZE)
                    markColor(c, mark);
                if (c >= 0x20 && c < 0x7F)
                    printf("%c", c);
                else
                    printf(".");
            }
            else
                printf(" ");
        }

        if (flags & PF_COLORIZE)
            writeColor(CS_WHITE);
        printf("|");
    }
    void markColor(SKuint32 c, SKuint64 mark)
    {
        if (c == mark)
            writeColor(CS_RED);
        else if (c == 0x00 || c == 0xCC)
            writeColor(CS_GREY);
        else
            writeColor(CS_WHITE);
    }

    void writeAddress(SKsize addr, int flags)
    {
        if (flags & PF_COLORIZE)
            writeColor(CS_LIGHT_GREY);
        if (PL8)
        {
            if (flags & PF_FULLADDR)
                printf("%016llX  ", (SKuint64)addr);
            else
                printf("%16llX  ", (SKuint64)addr);
        }
        else
        {
            if (flags & PF_FULLADDR)
                printf("%08X  ", (SKuint32)addr);
            else
                printf("%8X  ", (SKuint32)addr);
        }
    }

    void dumpHex(const void* ptr,
                 SKsize      offset,
                 SKsize      len,
                 int         flags,
                 SKuint64    mark,
                 bool        nl)
    {
        if (!ptr || offset == SK_NPOS || len == SK_NPOS)
            return;

        SKsize      i;
        const char* cp = (const char*)ptr;

        for (i = 0; i < len; i += 16)
        {
            if (flags & PF_ADDRESS)
                writeAddress((SKsize)(i + offset), flags);
            if (flags & PF_HEX)
            {
                if (flags & PF_COLORIZE)
                    writeHex(cp, i, len, mark, flags);
                else
                    writeHex(cp, i, len, flags);
            }
            if (flags & PF_ASCII)
                writeAscii(cp, i, len, mark, flags);
            if (nl)
                printf("\n");
        }
    }
}  // namespace skHexPrint
