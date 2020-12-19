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
#include "skCommandLineScanner.h"

using namespace skCommandLine;

// clang-format off
#define lowercase \
         'a' : case 'b' : case 'c' : case 'd' : case 'e' : case 'f' : \
    case 'g' : case 'h' : case 'i' : case 'j' : case 'k' : case 'l' : \
    case 'm' : case 'n' : case 'o' : case 'p' : case 'q' : case 'r' : \
    case 's' : case 't' : case 'u' : case 'v' : case 'w' : case 'x' : \
    case 'y' : case 'z'

#define uppercase \
         'A' : case 'B' : case 'C' : case 'D' : case 'E' : case 'F' : \
    case 'G' : case 'H' : case 'I' : case 'J' : case 'K' : case 'L' : \
    case 'M' : case 'N' : case 'O' : case 'P' : case 'Q' : case 'R' : \
    case 'S' : case 'T' : case 'U' : case 'V' : case 'W' : case 'X' : \
    case 'Y' : case 'Z'

#define digit \
         '0' : case '1' : case '2' : case '3' : \
    case '4' : case '5' : case '6' : case '7' : \
    case '8' : case '9'
// clang-format on

//

Scanner::Scanner() :
    m_pos(0)
{
    m_value.reserve(512);
}

void Scanner::clear()
{
    m_value.resize(0);
}

void Scanner::append(const char *arg)
{
    m_value.append(arg);
    m_value.append(' ');
}

void Scanner::lex(Token &tok)
{
    tok.clear();

    while (m_pos < m_value.size())
    {
        char ch = m_value.at(m_pos++);
        if (ch == '-')
        {
            const char nx = m_value.at(m_pos);
            if (nx >= '0' && nx <= '9')
            {
                tok.append(ch);
                continue;
            }
        }

        switch (ch)
        {
        case '-':
        {
            const char nx = m_value.at(m_pos);
            if (nx == '-')
                ++m_pos;
            tok.setType(nx == '-' ? (int)TOK_SWITCH_LONG : (int)TOK_SWITCH_SHORT);
            return;
        }
        case '"':
        case '\'':
        {
            // parse a standard string
            ch = m_value.at(m_pos++);
            while (ch != 0 && ch != '\'' && ch != '\"')
            {
                tok.append(ch);
                ch = m_value.at(m_pos++);
            }
            if (ch == '\'' || ch == '"')
            {
                if (!tok.getValue().empty())
                    tok.setType(TOK_IDENTIFIER);
            }
            return;
        }
        case '.':
        case '/':
        case digit:
        case uppercase:
        case lowercase:
            while (ch != 0 && ch != ' ')
            {
                tok.append(ch);
                ch = m_value.at(m_pos++);
            }
            if (ch == ' ')
                --m_pos;
            tok.setType(TOK_IDENTIFIER);
            return;
        case ' ':
        case '\t':
            break;
        default:
            tok.setType(TOK_ERROR);
            tok.append(m_value.substr(m_pos - 1, m_value.size()));
            return;
        }
    }
}
